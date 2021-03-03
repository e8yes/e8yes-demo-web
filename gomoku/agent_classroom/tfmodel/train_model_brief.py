#!/bin/python3

# This script briefly train an old model with a minibatch of the most 
# recently generated game data.

import argparse
import os
import logging
import tensorflow as tf
import numpy as np

from poly_functions import RequireShlFeatures
from poly_functions import ReadModelName
from poly_functions import ReadBoardSize
from poly_functions import LoadModel
from poly_functions import SaveModel
from poly_functions import TrainableVariables
from augmentation import AugmentData
from batch_generator import BatchGenerator

BATCH_SIZE = 50
VALIDATION_BATCH_SIZE = 5000

def Train(model_import_path: str,
          batch_gen: BatchGenerator,
          model_export_path: str) -> None:
    model = LoadModel(model_import_path=model_import_path)

    model_vars = TrainableVariables(model=model)

    optimizer = tf.optimizers.Adamax(learning_rate=1e-3)

    for i in range(20):
        boards, \
        game_phases, \
        next_move_stone_types, \
        shl_maps, \
        top_shl_features, \
        policies, \
        values = \
            batch_gen.NextBatch(batch_size=BATCH_SIZE,
                                training_data=True)

        boards, \
        game_phases, \
        next_move_stone_types, \
        shl_maps, \
        top_shl_features, \
        policies, \
        values = \
            AugmentData(
                boards=boards,
                game_phases=game_phases,
                next_move_stone_types=next_move_stone_types,
                shl_maps=shl_maps,
                top_shl_features=top_shl_features,
                policies=policies,
                values=values,
                percentage=0.1)

        with tf.GradientTape() as tape:
            loss = None
            policy_loss = None
            value_loss = None
            reg_loss = None

            if RequireShlFeatures(model_name):
                loss, policy_loss, value_loss, reg_loss = model.Loss(
                    boards,
                    game_phases,
                    next_move_stone_types,
                    shl_maps,
                    top_shl_features,
                    policies,
                    values)
            else:
                loss, policy_loss, value_loss, reg_loss = model.Loss(
                    boards,
                    game_phases,
                    next_move_stone_types,
                    policies,
                    values)

            grads = tape.gradient(target=loss, sources=model_vars)
            optimizer.apply_gradients(grads_and_vars=zip(grads, model_vars))

            logging.info("step={0} l={1} pl={2} vl={3} rl={4}"\
                .format(i,
                        loss.numpy(),
                        policy_loss.numpy(),
                        value_loss.numpy(),
                        reg_loss.numpy()))
        i += 1

    logging.info("Training loop has been terminated.")

    logging.info("Saving model...")
    SaveModel(model=model, model_export_path=model_export_path)

    boards, \
    game_phases, \
    next_move_stone_types, \
    shl_maps, \
    top_shl_features, \
    policies, \
    values = \
        batch_gen.NextBatch(batch_size=VALIDATION_BATCH_SIZE,
                            training_data=False)
    
    loss = None
    policy_loss = None
    value_loss = None
    reg_loss = None

    if RequireShlFeatures(model_name):
        loss, policy_loss, value_loss, reg_loss = model.Loss(
            boards,
            game_phases,
            next_move_stone_types,
            shl_maps,
            top_shl_features,
            policies,
            values)
    else:
        loss, policy_loss, value_loss, reg_loss = model.Loss(
            boards,
            game_phases,
            next_move_stone_types,
            policies,
            values)

    logging.info("validation: l={0} pl={1} vl={2} rl={3}"\
                .format(loss.numpy(),
                        policy_loss.numpy(),
                        value_loss.numpy(),
                        reg_loss.numpy()))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Briefly train an old model with a minibatchh of "\
                    "the most recently generated game data.")
    parser.add_argument("--model_input_path",
        type=str,
        help="A path to read the old model from. Do not specify a file name.")
    parser.add_argument("--model_output_path",
        type=str,
        help="A path to write the trained model to. Do not specify a file name.")
    parser.add_argument("--num_data_entries",
        type=str,
        help="The number of the latest game data entries used to train the model. "
             "If this argument is absent, it will use all the data.")
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
    model_input_path = args.model_input_path
    model_output_path = args.model_output_path
    num_data_entries = args.num_data_entries
    db_host = args.db_host
    db_name = args.db_name
    db_user = args.db_user
    db_pass = args.db_pass

    if model_input_path is None:
        logging.error("Argument model_input_path is required.")
        parser.print_help()
        exit(-1)
    if model_output_path is None:
        logging.error("Argument model_output_path is required.")
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

    logging.basicConfig(level=logging.INFO, 
                        format="%(asctime)s %(levelname)s %(message)s")

    model_name = ReadModelName(model_import_path=model_input_path)
    board_size = ReadBoardSize(model_name=model_name)

    batch_gen = BatchGenerator(
        board_size=board_size,
        num_data_entries=num_data_entries,
        db_name=db_name,
        db_host=db_host,
        db_port=5432,
        db_user=db_user,
        db_pass=db_pass)

    Train(model_import_path=model_input_path,
          batch_gen=batch_gen, 
          model_export_path=model_output_path)
