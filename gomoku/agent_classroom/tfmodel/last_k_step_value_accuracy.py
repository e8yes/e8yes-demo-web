#!/bin/python3

import argparse
import os
import logging
import tensorflow as tf
import numpy as np

from batch_generator import BatchGenerator
from poly_functions import RequireShlFeatures
from poly_functions import LoadModel
from poly_functions import ReadModelName
from poly_functions import ReadBoardSize

def LateGameValueAccuracy(
    model: any,
    last_k_steps: int,
    batch_gen: BatchGenerator,
    require_shl: bool) -> float:
    boards, \
    game_phases, \
    next_move_stone_types, \
    shl_maps, \
    top_shl_features, \
    _, \
    values = \
        batch_gen.NextBatch(
            batch_size=100000,
            sample_from=0,
            training_data=False,
            last_k_steps=last_k_steps)

    value_pred = None
    if require_shl:
        _, value_pred = model(
            boards,
            game_phases,
            next_move_stone_types,
            shl_maps,
            top_shl_features)
    else:
        _, value_pred = model(
            boards,
            game_phases,
            next_move_stone_types)

    value_pred = value_pred.numpy()

    no_tie_values = values[values != 0]
    no_tie_value_pred = value_pred[values != 0]

    no_tie_value_pred[no_tie_value_pred >= 0] = 1
    no_tie_value_pred[no_tie_value_pred < 0] = -1

    return np.sum(no_tie_values == no_tie_value_pred)/no_tie_values.shape[0], \
           no_tie_values.shape[0]


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Evaluates the model by checking the outcome prediction "
                    "accuracy over the last K states in the validation games.")
    parser.add_argument("--model_input_path",
        type=str,
        help="A path to the model to be evaluated. Do not specify a file name.")
    parser.add_argument("--k",
        type=int,
        help="Parameter of how many states to look back from the game's end state.")
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

    logging.basicConfig(level=logging.INFO, 
                        format="%(asctime)s %(levelname)s %(message)s")

    args = parser.parse_args()
    model_input_path = args.model_input_path
    k = args.k
    db_host = args.db_host
    db_name = args.db_name
    db_user = args.db_user
    db_pass = args.db_pass

    if model_input_path is None:
        logging.error("Argument model_input_path is required.")
        parser.print_help()
        exit(-1)
    if k is None:
        logging.error("Argument k is required.")
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

    model = LoadModel(model_import_path=model_input_path)
    model_name = ReadModelName(
        model_import_path=model_input_path)
    board_size = ReadBoardSize(model_name=model_name)

    gen = BatchGenerator(
        board_size=board_size,
        num_data_entries=None,
        db_name=db_name,
        db_host=db_host,
        db_port=5432,
        db_user=db_user,
        db_pass=db_pass)

    acc, n = LateGameValueAccuracy(
        model=model,
        last_k_steps=k,
        batch_gen=gen,
        require_shl=RequireShlFeatures(model_name))
    logging.info("LateGameValueAccuracy={0}, n={1}".format(acc, n))