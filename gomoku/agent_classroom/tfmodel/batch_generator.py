from typing import Tuple
import psycopg2 as pg
import numpy as np

from deserializer import DeserializeRows

class BatchGenerator:
    def __init__(self,
                 board_size: int,
                 num_data_entries: int,
                 db_name: str,
                 db_host: str,
                 db_port: int,
                 db_user: str,
                 db_pass: str):
        self.board_size_ = board_size
        self.conn_ = pg.connect(
            database=db_name,
            host=db_host,
            port=db_port,
            user=db_user,
            password=db_pass)
        self.num_data_entries_ = num_data_entries
    
    def PartitionClause_(self, training_data: bool):
        if training_data is None:
            return " AND TRUE "
        elif training_data == True:
            return " AND MOD(gga.game_id, 10) < 9 "
        else:
            return " AND MOD(gga.game_id, 10) >= 9 "
    
    def StepFilterClause_(self, last_k: int):
        if last_k is None:
            return " AND TRUE "
        else:
            return " AND gga.step_number > gg.num_steps - " + str(last_k)

    def MostRecentDataSet_(self, last_k_steps: int):
        if self.num_data_entries_ is None:
            return "(SELECT gga.* FROM gomoku_game_action gga "\
                   "JOIN gomoku_game gg ON gg.id = gga.game_id "\
                   "WHERE gg.end_at IS NOT NULL {0} )"\
                       .format(self.StepFilterClause_(last_k=last_k_steps))
        else:
            return "(SELECT gga.* FROM gomoku_game_action gga " \
                   "JOIN gomoku_game gg ON gg.id = gga.game_id "\
                   "WHERE gg.end_at IS NOT NULL {0} "\
                   "ORDER BY game_id DESC, step_number DESC " \
                   "LIMIT {1})"\
                       .format(self.StepFilterClause_(last_k=last_k_steps),
                               self.num_data_entries_)
    
    def NumDataEntries(self,
                       data_source: int,
                       training_data: bool,
                       last_k_steps: int = None) -> int:
        cur = self.conn_.cursor()
        cur.execute(
            "SELECT COUNT(gga.*) FROM {0} AS gga "
            "JOIN gomoku_game gg ON gga.game_id = gg.id "
            "WHERE gg.game_purpose={1} {2} " \
                .format(self.MostRecentDataSet_(last_k_steps=last_k_steps),
                        data_source,
                        self.PartitionClause_(training_data=training_data)))

        row = cur.fetchall()
        return int(row[0][0])
    
    def NextBatch(self,
                  batch_size: int,
                  sample_from: int,
                  training_data: bool,
                  last_k_steps: int = None) -> Tuple[np.ndarray,
                                                     np.ndarray,
                                                     np.ndarray,
                                                     np.ndarray,
                                                     np.ndarray,
                                                     np.ndarray,
                                                     np.ndarray]:
        cur = self.conn_.cursor()
        cur.execute("SELECT gga.* FROM {0} AS gga "
                    "JOIN gomoku_game gg ON gga.game_id = gg.id "
                    "WHERE gg.game_purpose={1} {2} "
                    "ORDER BY RANDOM() ASC "
                    "LIMIT {3} ".\
            format(self.MostRecentDataSet_(last_k_steps=last_k_steps),
                   sample_from,
                   self.PartitionClause_(training_data=training_data),
                   batch_size))
        rows = cur.fetchall()

        boards,\
        game_phases,\
        next_move_stone_types,\
        shl_maps,\
        top_shl_features,\
        policies,\
        values = DeserializeRows(
            rows=rows, board_size=self.board_size_)

        return boards,\
               game_phases,\
               next_move_stone_types,\
               shl_maps,\
               top_shl_features,\
               policies,\
               values

if __name__ == "__main__":
    gen = BatchGenerator(num_data_entries=None,
                         board_size=11,
                         db_name="demoweb",
                         db_host="localhost",
                         db_port=5432,
                         db_user="postgres",
                         db_pass="password")

    print("total=", gen.NumDataEntries(data_source=1, training_data=None))
    print("training=", gen.NumDataEntries(data_source=1, training_data=True))
    print("testing=", gen.NumDataEntries(data_source=1, training_data=False))

    boards, \
    game_phases, \
    next_move_stone_types, \
    shl_maps, \
    top_shl_features,\
    policies, \
    values= \
        gen.NextBatch(batch_size=1,
                      sample_from=1,
                      training_data=True)

    for i in range(boards.shape[0]):
        np.set_printoptions(precision=1)

        print("=================entry", i + 1, "====================")
        print("board=\n", boards[i, :])
        print("game_phases=", game_phases[i])
        print("next_move_stone_types=", next_move_stone_types[i])
        print("shl_score=\n", np.sum(a=shl_maps[i, :, :, :], axis=2))
        print("top_shl_features=\n", top_shl_features[i])
        top_shl_features
        print("policies=", policies[i])
        action_number = np.argmax(policies[i])
        x = action_number % 11
        y = action_number // 11
        print("arg_max_policies=", x, y)
        print("values=", values[i])
