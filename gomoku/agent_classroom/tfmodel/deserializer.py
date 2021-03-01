from typing import List
from typing import Tuple
import numpy as np

def DeserializeBoardData(serialized_board: any,
                         input_size: int) -> np.ndarray:
    serialized_board = bytes(serialized_board)

    board = np.zeros(
        shape=(input_size, input_size), dtype=np.uint8)
    for y in range(0, input_size):
        for x in range(0, input_size):
            board[x, y] = int(serialized_board[x + y*input_size])

    return board

def DeserializeShlMap(flat_shl_map: List[float],
                      input_size: int) -> np.ndarray:
    shl_map = np.zeros(
        shape=(input_size, input_size, 4), dtype=np.float32)

    for y in range(0, input_size):
        for x in range(0, input_size):
            src = (x + y*input_size)*4
            shl_map[x, y, 0] = flat_shl_map[src + 0]
            shl_map[x, y, 1] = flat_shl_map[src + 1]
            shl_map[x, y, 2] = flat_shl_map[src + 2]
            shl_map[x, y, 3] = flat_shl_map[src + 3]

    return shl_map

def DeserializeRows(rows: List[any],
                    input_size: int) \
    -> Tuple[np.ndarray,
             np.ndarray,
             np.ndarray,
             np.ndarray,
             np.ndarray,
             np.ndarray]:
    actual_batch_size = len(rows)

    boards = np.zeros(
        shape=(actual_batch_size, input_size, input_size), dtype=np.uint8)
    game_phases = np.zeros(
        shape=(actual_batch_size), dtype=np.uint8)
    next_move_stone_types = np.zeros(
        shape=(actual_batch_size), dtype=np.uint8)
    shl_maps = np.zeros(
        shape=(actual_batch_size, input_size, input_size, 4), dtype=np.float32)

    policies = np.zeros(
        shape=(actual_batch_size, input_size*input_size + 5), dtype=np.float32)
    values = np.zeros(
        shape=(actual_batch_size), dtype=np.float32)

    for i in range(len(rows)):
        boards[i, :, :] = DeserializeBoardData(
            serialized_board=rows[i][5], input_size=input_size)
        game_phases[i] = int(rows[i][6])
        next_move_stone_types[i] = int(rows[i][4])

        shl_maps[i, :, :, :] = DeserializeShlMap(
            flat_shl_map=rows[i][7], input_size=input_size)
        
        policies[i, :] = np.array(rows[i][8])
        values[i] = float(rows[i][9])
    
    return boards,\
           game_phases,\
           next_move_stone_types,\
           shl_maps,\
           policies,\
           values