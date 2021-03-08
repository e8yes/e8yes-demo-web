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

def CrossEntropy(y_true, y_pred):
    return np.sum(-y_true*np.log(y_pred + 1e-5), axis=1)

def ArgmaxPolicyAccuracy(
    model: any,
    batch_gen: BatchGenerator,
    last_k_steps: int,
    require_shl: bool) -> float:
    boards, \
    game_phases, \
    next_move_stone_types, \
    shl_maps, \
    _, \
    policies, \
    _ = \
        batch_gen.NextBatch(
            batch_size=batch_gen.NumDataEntries(
                training_data=False, last_k_steps=last_k_steps),
            training_data=False,
            last_k_steps=last_k_steps)

    policy_preds = None
    if require_shl:
        policy_preds, _ = model(
            boards,
            game_phases,
            next_move_stone_types,
            shl_maps)
    else:
        policy_preds, _ = model(
            boards,
            game_phases,
            next_move_stone_types)

    action_ground_truth = np.argmax(a=policies, axis=1)

    action_preds = np.argmax(a=policy_preds, axis=1)

    shl_scores = np.sum(a=shl_maps, axis=3)
    flat_shl_scores = np.reshape(
        a=shl_scores, newshape=(-1, 11*11), order="F")
    shl_action_preds = np.argmax(a=flat_shl_scores, axis=1)

    num_correct_heuristics = np.sum(
        a=action_preds == action_ground_truth, axis=0)
    num_correct_shl = np.sum(
        a=shl_action_preds == action_ground_truth, axis=0)
    
    heuristics_ce = np.mean(
        CrossEntropy(policies[:, :121], policy_preds[:, :121]), axis=0)
    shl_ce = np.mean(
        CrossEntropy(policies[:, :121], flat_shl_scores), axis=0)

    return num_correct_heuristics / policies.shape[0], \
           num_correct_shl / policies.shape[0], \
           heuristics_ce, \
           shl_ce, \
           policies.shape[0]


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Evaluates the model by checking if the argmax action "
                    "aligns with the data set's.")
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

    heuristics_acc, shl_acc, \
    heuristics_ce, shl_ce, \
    n = ArgmaxPolicyAccuracy(
        model=model,
        batch_gen=gen,
        last_k_steps=k,
        require_shl=RequireShlFeatures(model_name))
    logging.info("ArgmaxPolicyAccuracy={0}, {1}; n={2}"\
        .format(heuristics_acc, shl_acc, n))
    logging.info("CrossEntropy={0}, {1}; n={2}"\
        .format(heuristics_ce, shl_ce, n))