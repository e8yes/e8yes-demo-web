#!/bin/python3

import os
import tensorflow as tf
import numpy as np
from batch_generator import BatchGenerator
from cnn_resnet_shared_tower_model import GomokuCnnResNetSharedTower
from cnn_resnet_shared_tower_model import GomokuCnnResNetSharedTowerModelName

def LateGameValueAccuracy(
    model: GomokuCnnResNetSharedTower,
    last_k_steps: int,
    batch_gen: BatchGenerator) -> float:
    boards, \
    game_phase_place_3_stones, \
    game_phase_swap2_decision, \
    game_phase_place_2_more_stones, \
    game_phase_stone_type_decision, \
    game_phase_standard_gomoku, \
    next_move_stone_types, \
    _, \
    values = \
        batch_gen.NextBatch(batch_size=50000,
                            sample_from=1,
                            training_data=False,
                            enable_augmentation=False,
                            last_k_steps=last_k_steps)

    _, value_pred, _ = model(boards,
                             game_phase_place_3_stones,
                             game_phase_swap2_decision,
                             game_phase_place_2_more_stones,
                             game_phase_stone_type_decision,
                             game_phase_standard_gomoku,
                             next_move_stone_types)

    value_pred = value_pred.numpy()

    no_tie_values = values[values != 0]
    no_tie_value_pred = value_pred[values != 0]

    no_tie_value_pred[no_tie_value_pred >= 0] = 1
    no_tie_value_pred[no_tie_value_pred < 0] = -1

    return np.sum(no_tie_values == no_tie_value_pred)/no_tie_values.shape[0], \
           no_tie_values.shape[0]


if __name__ == "__main__":
    model_import_path = "/home/davis/saved_models/experimental"

    gen = BatchGenerator(num_data_entries=40000,
                         db_name="demoweb",
                         db_host="localhost",
                         db_port=5432,
                         db_user="postgres",
                         db_pass="password")
    
    src = os.path.join(
        model_import_path, GomokuCnnResNetSharedTowerModelName())
    model = tf.saved_model.load(export_dir=src)

    acc, n = LateGameValueAccuracy(
        model=model, last_k_steps=1, batch_gen=gen)
    print("LateGameValueAccuracy=", acc, ", n=", n)