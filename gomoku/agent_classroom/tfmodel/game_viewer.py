#!/bin/python3
from typing import List
import os
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

def PrintBoard(board: List[List[str]],
               policy: List[ActionProb],
               next_action: int):
    for y in range(11):
        for x in range(11):
            action_number = x + y*11
            rank = ActionRank(
                action_number=action_number, policy=policy)
            
            annotation = " "

            if next_action == action_number:
                annotation = "*"

            if rank is not None and \
               rank < 5 and \
               board[y][x] == "-":
                print(" {0}{1}".format(annotation, rank + 1), end="")
            else:
                print(" {0}{1}".format(annotation, board[y][x]), end="")

        print("")
        

def Replay(history: List[ActionHistoryRecord]):
    print("num_moves=", len(history))

    i = 0
    while True:
        print("step=", i + 1)
        print("game_phase=", history[i].game_phase_)
        print("next_move_stone_type=", history[i].game_phase_)

        PrintBoard(
            history[i].board_, history[i].policy_, history[i].action_number_)

        command = input("command=")
        if command == "n":
            if i < len(history) - 1:
                i += 1
        elif command == "p":
            if i >= 1:
                i -= 1
        elif command == "q":
            break

        os.system("clear")

if __name__ == "__main__":
    game_id = 2642

    history = FetchActionHistory(
        game_id=game_id,
        db_name="demoweb",
        db_host="localhost",
        db_port=5432,
        db_user="postgres",
        db_pass="password")

    Replay(history)