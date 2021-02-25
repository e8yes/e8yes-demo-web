#!/bin/python3
from typing import List
import argparse
import os
import time
import logging
import psycopg2 as pg

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
               policy: List[ActionProb],
               next_action: int):
    for y in range(11):
        for x in range(11):
            pos_action_number = x + y*11
            rank = ActionRank(
                action_number=pos_action_number, policy=policy)
            
            annotation = " "
            if next_action == pos_action_number:
                annotation = "*"

            if rank is not None and \
               rank < 5 and \
               board[y][x] == "-":
                print(" {0}{1}".format(annotation, rank + 1), end="")
            else:
                print(" {0}{1}".format(annotation, board[y][x]), end="")

        print("")
        

def Replay(history: List[ActionHistoryRecord]):
    i = 0
    preview_mode = True
    while True:
        os.system("clear")
        print("total_num_moves=", len(history))
        print("step=", i + 1)
        print("game_phase=", history[i].game_phase_)
        print("next_move_stone_type=", history[i].next_move_stone_type_)

        PrintBoard(
            history[i].board_,
            history[i].policy_,
            history[i].action_number_)
        
        PrintNonBoardActions(
            history[i].game_phase_,
            history[i].policy_,
            history[i].action_number_)

        if preview_mode:
            time.sleep(2)
            i += 1

            if i == len(history):
                preview_mode = False
                i = 0

            continue

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
        description="Visually presents a Gomoku game.")
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