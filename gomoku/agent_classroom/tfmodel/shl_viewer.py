#!/bin/python3
from typing import List
from typing import Tuple
import argparse
import os
import time
import logging
import psycopg2 as pg

class LinkageInfo:
    def __init__(self,
                 raw_count: int,
                 holes: int,
                 blockage: int):
        self.raw_count = raw_count
        self.holes = holes
        self.blockage = blockage
    
    def __repr__(self):
        return "LinkageInfo=[raw_count={0}, holes={1}, blockage={2}]"\
            .format(self.raw_count, self.holes, self.blockage)

def LinkageCountsAtWithDirection(x: int, y: int,
                                 dx: int, dy: int,
                                 stone_type: str,
                                 board: List[List[str]]) -> Tuple[int, int, int]:
    raw_count = 0
    holes = 0
    blocked = False
    i = 1

    state = "count_piece"

    while state != "end":
        if state == "count_piece":
            if i > 6:
                state = "end"
                continue

            shifted_x = x + i*dx
            shifted_y = y + i*dy

            if shifted_x < 0 or shifted_x > 10 or \
               shifted_y < 0 or shifted_y > 10 or \
               (board[shifted_y][shifted_x] != "-" and
                board[shifted_y][shifted_x] != stone_type):
                state = "blocked"
                continue
            elif board[shifted_y][shifted_x] == "-":
                state = "punctuation"
                continue
            else:
                raw_count += 1
                i += 1
                continue
        elif state == "punctuation":
            i += 1

            shifted_x = x + i*dx
            shifted_y = y + i*dy

            if shifted_x < 0 or shifted_x > 10 or \
               shifted_y < 0 or shifted_y > 10 or \
               board[shifted_y][shifted_x] != stone_type or \
               board[shifted_y][shifted_x] == "-":
                state = "end"
                continue
            else:
                state = "count_holes"
                continue
        elif state == "blocked":
            blocked = True
            state = "end"
            continue
        elif state == "count_holes":
            holes += 1
            state = "count_piece"
            continue
        else:
            print(state)
            assert(False)

    return raw_count, holes, blocked


def LinkageAtWithDirection(x: int, y: int,
                           dx: int, dy: int,
                           stone_type: str,
                           board: List[List[str]]) -> LinkageInfo:
    raw_count1, holes1, blocked1 = LinkageCountsAtWithDirection(
        x, y, dx, dy, stone_type, board)
    
    raw_count2, holes2, blocked2 = LinkageCountsAtWithDirection(
        x, y, -dx, -dy, stone_type, board)

    blockage = 0
    if blocked1:
        blockage += 1
    if blocked2:
        blockage += 1

    return LinkageInfo(
        raw_count=raw_count1 + raw_count2,
        holes=holes1 + holes2,
        blockage=blockage)

def LinkagesAt(x: int, y: int,
               stone_type: str,
               board: List[List[str]]) -> List[LinkageInfo]:
    horizontal = LinkageAtWithDirection(
        x=x, y=y, dx=1, dy=0,
        stone_type=stone_type,
        board=board)

    vertical = LinkageAtWithDirection(
        x=x, y=y, dx=0, dy=1,
        stone_type=stone_type,
        board=board)

    diagnal = LinkageAtWithDirection(
        x=x, y=y, dx=1, dy=1,
        stone_type=stone_type,
        board=board)

    counter_diagnal = LinkageAtWithDirection(
        x=x, y=y, dx=1, dy=-1,
        stone_type=stone_type,
        board=board)

    return [horizontal, vertical, diagnal, counter_diagnal]

def LinkageToAdjustedCount(linkage: LinkageInfo,
                           stone_type: str,
                           next_move_stone_type: str) -> float:
    move_advantage = 1
    if next_move_stone_type != "-":
        if stone_type == next_move_stone_type:
            move_advantage = 1
        else:
            move_advantage = 0.5

    adjusted_count = \
        max(0, linkage.raw_count + move_advantage - 0.5*linkage.holes - linkage.blockage)

    return adjusted_count

def ShlCount(x: int, y: int,
             stone_type: str,
             next_move_stone_type: str,
             board: List[List[str]]) -> float:
    linkages = LinkagesAt(
        x, y, stone_type, board)

    adjusted_counts = list()
    for linkage in linkages:
        adjusted_count = LinkageToAdjustedCount(
            linkage, stone_type, next_move_stone_type)
        adjusted_counts.append(adjusted_count)
    
    adjusted_counts = sorted(adjusted_counts, reverse=True)
    
    return adjusted_counts[0]**2.5 + adjusted_counts[1]**2.5

def NonTopKSuppression(shl_map: List[List[float]], k: int) -> List[List[float]]:
    suppresed_shl_map = [[0.0 for _ in range(11)] for _ in range(11)]

    flattened = list()
    for y in range(11):
        for x in range(11):
            flattened.append(shl_map[y][x])
    
    flattened = sorted(flattened, reverse=True)

    top_k_value = flattened[k - 1]

    for y in range(11):
        for x in range(11):
            if shl_map[y][x] >= top_k_value:
                suppresed_shl_map[y][x] = shl_map[y][x]
    
    return suppresed_shl_map

def IsDoubleContour(x: int, y: int,
                    board: List[List[str]]):
    for delta_y in range(-2, 3):
        for delta_x in range(-2, 3):
            shifted_x = x + delta_x
            shifted_y = y + delta_y

            if shifted_x >= 0 and shifted_y >= 0 and \
               shifted_x <= 10 and shifted_y <= 10 and \
               board[shifted_y][shifted_x] != "-":
               return True

    return False

def ComputeShlMap(board: List[List[str]],
                  next_move_stone_type: str,
                  k: int) -> List[List[float]]:
    shl_map = [[0.0 for _ in range(11)] for _ in range(11)]
    
    for y in range(11):
        for x in range(11):
            if board[y][x] != "-":
                continue
            if not IsDoubleContour(x, y, board):
                continue
            shl_map[y][x] = \
                ShlCount(
                    x=x, y=y, 
                    stone_type="x", 
                    next_move_stone_type=next_move_stone_type,
                    board=board) + \
                ShlCount(
                    x=x, y=y,
                    stone_type="o",
                    next_move_stone_type=next_move_stone_type,
                    board=board)
    
    return NonTopKSuppression(shl_map, k)

def NormalizeShlMap(shl_map: List[List[float]]) -> List[List[float]]:
    normalized_shl_map = [[0.0 for _ in range(11)] for _ in range(11)]

    total_shl_score = 0.0
    for y in range(11):
        for x in range(11):
            total_shl_score += shl_map[y][x]
    
    if total_shl_score == 0.0:
        return normalized_shl_map

    for y in range(11):
        for x in range(11):
            normalized_shl_map[y][x] = \
                shl_map[y][x]/total_shl_score

    return normalized_shl_map

def PrintShlMap(shl_map: List[List[float]]):
    for y in range(11):
        for x in range(11):
            if shl_map[y][x] == 0:
                print("  -", end="")
            else:
                print("  {0:.0f}".format(shl_map[y][x]), end="")

        print("")

class ActionProb:
    def __init__(self,
                 action_number: int,
                 prob: float):
        self.action_number_ = action_number
        self.prob_ = prob
    
    def __lt__(self, rhs) -> bool:
        return self.prob_ < rhs.prob_

class ActionHistoryRecord:
    def __init__(self,
                 game_phase: str,
                 board: List[List[str]],
                 next_move_stone_type: str,
                 policy: List[ActionProb], 
                 action_number: int):
        self.game_phase_ = game_phase
        self.board_ = board
        self.next_move_stone_type_ = next_move_stone_type
        self.policy_ = policy
        self.action_number_ = action_number

def BoardData(serialized_board: bytes) -> List[List[str]]:
    board = [[str() for _ in range(11)] for _ in range(11)]
    for y in range(0, 11):
        for x in range(0, 11):
            if serialized_board[x + y*11] == 2:
                board[y][x] = 'o'
            elif serialized_board[x + y*11] == 1:
                board[y][x] = 'x'
            else:
                board[y][x] = '-'
    return board

def PolicyData(flat_policy: List[float]) -> List[ActionProb]:
    action_probs = list()

    for i in range(len(flat_policy)):
        action_probs.append(
            ActionProb(action_number=i, 
                       prob=flat_policy[i]))

    return sorted(action_probs, reverse=True)

def FetchActionHistory(game_id: int,
                       db_name: str,
                       db_host: str,
                       db_port: int,
                       db_user: str,
                       db_pass: str) -> List[ActionHistoryRecord]:
    conn = pg.connect(database=db_name,
                      host=db_host,
                      port=db_port,
                      user=db_user,
                      password=db_pass)
    cur = conn.cursor()
    cur.execute("SELECT * FROM gomoku_game_action gga WHERE gga.game_id={0}"\
        .format(game_id))
    rows = cur.fetchall()

    assert(len(rows) > 0)

    history: List[ActionHistoryRecord] = list()
    for row in rows:
        game_phase = str()
        if row[6] == 0:
            game_phase = "place 3 stones"
        elif row[6] == 1:
            game_phase = "swap2 decision"
        elif row[6] == 2:
            game_phase = "place 2 more stones"
        elif row[6] == 3:
            game_phase = "stone type decision"
        elif row[6] == 4:
            game_phase = "standard gomoku"
        
        next_move_stone_type = str()
        if row[4] == 1:
            next_move_stone_type = "x"
        elif row[4] == 2:
            next_move_stone_type = "o"
        else:
            next_move_stone_type = "-"

        record = ActionHistoryRecord(
            game_phase=game_phase,
            board=BoardData(bytes(row[5])),
            next_move_stone_type=next_move_stone_type,
            policy=PolicyData(row[7]),
            action_number=row[2])
        history.append(record)

    return history

def ActionRank(action_number: int, 
               policy: List[ActionProb]) -> int:
    for i in range(len(policy)):
        if policy[i].action_number_ == action_number:
            return i

    return None

def PrintNonBoardActions(game_phase: str, 
                         policy: List[ActionProb],
                         next_action: int):
    if game_phase == "swap2 decision":
        action_choose_black = 11*11
        action_choose_white = 11*11 + 1
        action_place_2_more_stones = 11*11 + 2

        print(
            "Choose black",
            ActionRank(
                action_number=action_choose_black, policy=policy) if not None else "",
            "*" if action_choose_black == next_action else "",
            "Choose white",
            ActionRank(
                action_number=action_choose_white, policy=policy) if not None else "",
            "*" if action_choose_white == next_action else "",
            "Place 2 more stones",
            ActionRank(
                action_number=action_place_2_more_stones, policy=policy) if not None else "",
            "*" if action_place_2_more_stones == next_action else "")
    elif game_phase == "stone type decision":
        action_choose_black = 11*11 + 3
        action_choose_white = 11*11 + 4

        print(
            "Choose black",
            ActionRank(
                action_number=action_choose_black, policy=policy) if not None else "",
            "*" if action_choose_black == next_action else "",
            "Choose white",
            ActionRank(
                action_number=action_choose_white, policy=policy) if not None else "",
            "*" if action_choose_white == next_action else "")

def PrintBoard(board: List[List[str]],
               shl_map: List[List[float]],
               next_action: int):
    for y in range(11):
        for x in range(11):
            pos_action_number = x + y*11

            annotation = " "
            if next_action == pos_action_number:
                annotation = "*"

            if board[y][x] == "-" and \
               shl_map is not None and \
               shl_map[y][x] != 0:
                print(" {0}{1}".format(annotation, round(shl_map[y][x]*10)), end="")
            else:
                print(" {0}{1}".format(annotation, board[y][x]), end="")

        print("")


def Replay(history: List[ActionHistoryRecord]):
    i = 0
    while True:
        os.system("clear")
        print("total_num_moves=", len(history))
        print("step=", i + 1)
        print("game_phase=", history[i].game_phase_)
        print("next_move_stone_type=", history[i].next_move_stone_type_)

        shl_map = ComputeShlMap(
            board=history[i].board_, 
            next_move_stone_type=history[i].next_move_stone_type_,
            k=5)
        normalized_shl_map = NormalizeShlMap(shl_map=shl_map)

        PrintBoard(
            history[i].board_,
            normalized_shl_map,
            history[i].action_number_)
        
        PrintNonBoardActions(
            history[i].game_phase_,
            history[i].policy_,
            history[i].action_number_)

        command = input("command=")
        if command == "n":
            if i < len(history) - 1:
                i += 1
        elif command == "p":
            if i >= 1:
                i -= 1
        elif command == "q":
            break

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Visually presents SHL features in a Gomoku game.")
    parser.add_argument("--game_id",
        type=str,
        help="ID of a game to present.")
    parser.add_argument("--db_host",
        type=str,
        help="Host name pointing to the database storing the latest game data.")
    parser.add_argument("--db_name",
        type=str,
        help="Name of the database storing the latest game data.")
    parser.add_argument("--db_user",
        type=str,
        help="User of the database that has access to the latest game data.")
    parser.add_argument("--db_pass",
        type=str,
        help="Password of the user that has access to the latest game data.")

    args = parser.parse_args()
    game_id = args.game_id
    db_host = args.db_host
    db_name = args.db_name
    db_user = args.db_user
    db_pass = args.db_pass

    if game_id is None:
        logging.error("Argument game_id is required.")
        parser.print_help()
        exit(-1)
    if db_host is None:
        logging.error("Argument db_host is required.")
        parser.print_help()
        exit(-1)
    if db_name is None:
        logging.error("Argument db_name is required.")
        parser.print_help()
        exit(-1)
    if db_user is None:
        logging.error("Argument db_user is required.")
        parser.print_help()
        exit(-1)
    if db_pass is None:
        logging.error("Argument db_pass is required.")
        parser.print_help()
        exit(-1)

    history = FetchActionHistory(
        game_id=game_id,
        db_name=db_name,
        db_host=db_host,
        db_port=5432,
        db_user=db_user,
        db_pass=db_pass)

    Replay(history)