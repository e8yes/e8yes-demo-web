import psycopg2 as pg
import numpy as np

def ExtractBoardData(serialized_board: bytes) -> np.ndarray:
    board = np.zeros(shape=(11, 11))
    for y in range(0, 11):
        for x in range(0, 11):
            if serialized_board[x + y*11] == 2:
                board[x, y] = 1
            elif serialized_board[x + y*11] == 1:
                board[x, y] = -1
            else:
                board[x, y] = 0
    return board

class BatchGenerator:
    def __init__(self,
                 num_data_entries: int,
                 db_name: str,
                 db_host: str,
                 db_port: int,
                 db_user: str,
                 db_pass: str):
        self.conn_ = pg.connect(database=db_name,
                                host=db_host,
                                port=db_port,
                                user=db_user,
                                password=db_pass)

        self.num_data_entries_ = num_data_entries

        self.offset_ = 0

        self.seed_ = 378126
    
    def PartitionClause_(self, training_data: bool):
        if training_data is None:
            return " AND TRUE "
        elif training_data == True:
            return " AND MOD(HASH_BIGINT(gga.game_id + {0}), 10) < 8 "\
                .format(self.seed_)
        else:
            return " AND MOD(HASH_BIGINT(gga.game_id + {0}), 10) >= 8 "\
                .format(self.seed_)
    
    def MostRecentDataSet_(self):
        if self.num_data_entries_ is None:
            return " gomoku_game_action "
        else:
            return "(SELECT * FROM gomoku_game_action " \
                   "ORDER BY game_id DESC, step_number DESC " \
                   "LIMIT {0})".format(self.num_data_entries_)
    
    def NumDataEntries(self, training_data: bool) -> int:
        cur = self.conn_.cursor()
        cur.execute(
            "SELECT COUNT(gga.*) FROM {0} AS gga "
            "WHERE TRUE {1} " \
                .format(self.MostRecentDataSet_(),
                        self.PartitionClause_(training_data=training_data)))
        row = cur.fetchall()
        return int(row[0][0])
    
    def NextBatch(self,
                  batch_size: int,
                  sample_from: int,
                  training_data: bool,
                  enable_augmentation: bool) -> np.ndarray:
        cur = self.conn_.cursor()
        cur.execute("SELECT gga.* FROM {0} AS gga "
                    "JOIN gomoku_game ga ON gga.game_id = ga.id "
                    "WHERE ga.game_purpose={1} {2} "
                    "ORDER BY HASH_BIGINT(gga.game_id * 1031 + gga.step_number) ASC "
                    "LIMIT {3} "
                    "OFFSET {4}".\
            format(self.MostRecentDataSet_(),
                   sample_from,
                   self.PartitionClause_(training_data=training_data),
                   batch_size,
                   self.offset_))
        rows = cur.fetchall()

        self.offset_ = (self.offset_ + batch_size) % \
            self.NumDataEntries(training_data=training_data)

        boards = np.zeros(
            shape=(batch_size, 11, 11), dtype=np.float32)
        game_phase_place_3_stones = np.zeros(
            shape=(batch_size), dtype=np.float32)
        game_phase_swap2_decision = np.zeros(
            shape=(batch_size), dtype=np.float32)
        game_phase_place2_more_stones = np.zeros(
            shape=(batch_size), dtype=np.float32)
        game_phase_stone_type_decision = np.zeros(
            shape=(batch_size), dtype=np.float32)
        game_phase_standard_gomoku = np.zeros(
            shape=(batch_size), dtype=np.float32)
        stone_types = np.zeros(
            shape=(batch_size), dtype=np.float32)

        policies = np.zeros(
            shape=(batch_size, 11*11 + 5), dtype=np.float32)
        values = np.zeros(
            shape=(batch_size), dtype=np.float32)

        for i in range(len(rows)):
            boards[i, :, :] = ExtractBoardData(bytes(rows[i][5]))

            if rows[i][4] == 1:
                stone_types[i] = -1
            elif rows[i][4] == 2:
                stone_types[i] = 1

            if rows[i][6] == 0:
                game_phase_place_3_stones[i] = 1
            elif rows[i][6] == 1:
                game_phase_swap2_decision[i] = 1
            elif rows[i][6] == 2:
                game_phase_place2_more_stones[i] = 1
            elif rows[i][6] == 3:
                game_phase_stone_type_decision[i] = 1
            elif rows[i][6] == 4:
                game_phase_standard_gomoku[i] = 1
            
            policies[i, :] = np.array(rows[i][7])
            values[i] = rows[i][8]

        if enable_augmentation:
            boards_r90 = np.rot90(boards, k=1, axes=(1, 2))
            boards_r180 = np.rot90(boards_r90, k=1, axes=(1, 2))
            boards_r270 = np.rot90(boards_r180, k=1, axes=(1, 2))

            boards_flip = np.flip(boards, axis=1)
            boards_flip_r90 = \
                np.rot90(boards_flip, k=1, axes=(1, 2))
            boards_flip_r180 = \
                np.rot90(boards_flip_r90, k=1, axes=(1, 2))
            boards_flip_r270 = \
                np.rot90(boards_flip_r180, k=1, axes=(1, 2))

            boards = np.concatenate(
                (boards,
                 boards_r90,
                 boards_r180,
                 boards_r270,
                 boards_flip,
                 boards_flip_r90,
                 boards_flip_r180,
                 boards_flip_r270),
                axis=0)

            stone_types = np.concatenate(
                (stone_types,
                 stone_types,
                 stone_types,
                 stone_types,
                 stone_types,
                 stone_types,
                 stone_types,
                 stone_types),
                axis=0)
            
            game_phase_place_3_stones = np.concatenate(
                (game_phase_place_3_stones,
                 game_phase_place_3_stones,
                 game_phase_place_3_stones,
                 game_phase_place_3_stones,
                 game_phase_place_3_stones,
                 game_phase_place_3_stones,
                 game_phase_place_3_stones,
                 game_phase_place_3_stones),
                axis=0)

            game_phase_swap2_decision = np.concatenate(
                (game_phase_swap2_decision,
                 game_phase_swap2_decision,
                 game_phase_swap2_decision,
                 game_phase_swap2_decision,
                 game_phase_swap2_decision,
                 game_phase_swap2_decision,
                 game_phase_swap2_decision,
                 game_phase_swap2_decision),
                axis=0)
            
            game_phase_place2_more_stones = np.concatenate(
                (game_phase_place2_more_stones,
                 game_phase_place2_more_stones,
                 game_phase_place2_more_stones,
                 game_phase_place2_more_stones,
                 game_phase_place2_more_stones,
                 game_phase_place2_more_stones,
                 game_phase_place2_more_stones,
                 game_phase_place2_more_stones),
                axis=0)

            game_phase_stone_type_decision = np.concatenate(
                (game_phase_stone_type_decision,
                 game_phase_stone_type_decision,
                 game_phase_stone_type_decision,
                 game_phase_stone_type_decision,
                 game_phase_stone_type_decision,
                 game_phase_stone_type_decision,
                 game_phase_stone_type_decision,
                 game_phase_stone_type_decision),
                axis=0)

            game_phase_standard_gomoku = np.concatenate(
                (game_phase_standard_gomoku,
                 game_phase_standard_gomoku,
                 game_phase_standard_gomoku,
                 game_phase_standard_gomoku,
                 game_phase_standard_gomoku,
                 game_phase_standard_gomoku,
                 game_phase_standard_gomoku,
                 game_phase_standard_gomoku),
                axis=0)

            policies = np.concatenate(
                (policies, policies, policies, policies, 
                 policies, policies, policies, policies),
                axis=0)

            values = np.concatenate(
                (values, values, values, values, 
                 values, values, values, values),
                axis=0)

        return boards, \
               game_phase_place_3_stones, \
               game_phase_swap2_decision, \
               game_phase_place2_more_stones, \
               game_phase_stone_type_decision, \
               game_phase_standard_gomoku, \
               stone_types, \
               policies, \
               values

if __name__ == "__main__":
    gen = BatchGenerator(num_data_entries=None,
                         db_name="demoweb",
                         db_host="localhost",
                         db_port=5432,
                         db_user="postgres",
                         db_pass="password")

    print("total=", gen.NumDataEntries(training_data=None))
    print("training=", gen.NumDataEntries(training_data=True))
    print("testing=", gen.NumDataEntries(training_data=False))

    boards, \
    game_phase_place_3_stones, \
    game_phase_swap2_decision, \
    game_phase_place2_more_stones, \
    game_phase_stone_type_decision, \
    game_phase_standard_gomoku, \
    next_move_stone_types, \
    policies, \
    values= \
        gen.NextBatch(batch_size=10,
                      sample_from=0,
                      training_data=True,
                      enable_augmentation=False)

    for i in range(10):
        print("=================entry", i + 1, "====================")
        print("board=\n", boards[i, :])
        print("game_phase_place_3_stones=", game_phase_place_3_stones[i])
        print("game_phase_swap2_decision=", game_phase_swap2_decision[i])
        print("game_phase_place2_more_stones=", game_phase_place2_more_stones[i])
        print("game_phase_stone_type_decision=", game_phase_stone_type_decision[i])
        print("game_phase_standard_gomoku=", game_phase_standard_gomoku[i])
        print("next_move_stone_types=", next_move_stone_types[i])
        print("policies=", policies[i])
        action_number = np.argmax(policies[i])
        x = action_number % 11
        y = action_number // 11
        print("arg_max_policies=", x, y)
        print("values=", values[i])
