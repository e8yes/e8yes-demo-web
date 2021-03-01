from typing import List
from typing import Tuple
import numpy as np

def DeserializeBoardData(serialized_board: any,
                         board_size: int) -> np.ndarray:
    serialized_board = bytes(serialized_board)

    board = np.zeros(
        shape=(board_size, board_size), dtype=np.uint8)
    for y in range(0, board_size):
        for x in range(0, board_size):
            board[x, y] = int(serialized_board[x + y*board_size])

    return board

def DeserializeShlMap(flat_shl_map: List[float],
                      board_size: int) -> np.ndarray:
    shl_map = np.zeros(
        shape=(board_size, board_size, 4), dtype=np.float32)

    for y in range(0, board_size):
        for x in range(0, board_size):
            src = (x + y*board_size)*4
            shl_map[x, y, 0] = flat_shl_map[src + 0]
            shl_map[x, y, 1] = flat_shl_map[src + 1]
            shl_map[x, y, 2] = flat_shl_map[src + 2]
            shl_map[x, y, 3] = flat_shl_map[src + 3]

    return shl_map

def DeserializeRows(rows: List[any],
                    board_size: int) \
    -> Tuple[np.ndarray,
             np.ndarray,
             np.ndarray,
             np.ndarray,
             np.ndarray,
             np.ndarray,
             np.ndarray]:
    actual_batch_size = len(rows)

    boards = np.zeros(
        shape=(actual_batch_size, board_size, board_size), dtype=np.uint8)
    game_phases = np.zeros(
        shape=(actual_batch_size), dtype=np.uint8)
    next_move_stone_types = np.zeros(
        shape=(actual_batch_size), dtype=np.uint8)
    shl_maps = np.zeros(
        shape=(actual_batch_size, board_size, board_size, 4), dtype=np.float32)
    top_shl_features = np.zeros(
        shape=(actual_batch_size, 40), dtype=np.float32)

    policies = np.zeros(
        shape=(actual_batch_size, board_size*board_size + 5), dtype=np.float32)
    values = np.zeros(
        shape=(actual_batch_size), dtype=np.float32)

    for i in range(len(rows)):
        boards[i, :, :] = DeserializeBoardData(
            serialized_board=rows[i][5], board_size=board_size)
        game_phases[i] = int(rows[i][6])
        next_move_stone_types[i] = int(rows[i][4])

        shl_maps[i, :, :, :] = DeserializeShlMap(
            flat_shl_map=rows[i][7], board_size=board_size)
        top_shl_features[i, :] = np.array(object=rows[i][8])
        
        policies[i, :] = np.array(object=rows[i][9])
        values[i] = float(rows[i][10])
    
    return boards,\
           game_phases,\
           next_move_stone_types,\
           shl_maps,\
           top_shl_features,\
           policies,\
           values