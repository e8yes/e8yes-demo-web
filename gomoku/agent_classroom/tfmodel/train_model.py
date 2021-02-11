#!/bin/python3

# This script train an old model with the most recently generated game data.

import argparse
import os
import logging
import tensorflow as tf
import numpy as np
from batch_generator import BatchGenerator
from cnn_resnet_shared_tower_model import CollectGomokuCnnResNetSharedTowerVariables
from cnn_resnet_shared_tower_model import GomokuCnnResNetSharedTower
from cnn_resnet_shared_tower_model import GomokuCnnResNetSharedTowerModelName

def DataSetLoss(model: GomokuCnnResNetSharedTower,
                data_source: int,
                training_data: bool,
                batch_gen: BatchGenerator):
    batch_size = 1000
    num_batches = \
        test_batch_gen.NumDataEntries(
            data_source=data_source,
            training_data=training_data) // batch_size + 1

    total_loss = 0
    total_policy_loss = 0
    total_value_loss = 0
    num_data_points = 0

    for _ in range(num_batches):
        boards, \
        game_phase_place_3_stones, \
        game_phase_swap2_decision, \
        game_phase_place_2_more_stones, \
        game_phase_stone_type_decision, \
        game_phase_standard_gomoku, \
        next_move_stone_types, \
        policies, \
        values = \
            batch_gen.NextBatch(batch_size=batch_size,
                                sample_from=data_source,
                                training_data=training_data,
                                enable_augmentation=False)

        loss, policy_loss, value_loss = model.Loss(
            boards,
            game_phase_place_3_stones,
            game_phase_swap2_decision,
            game_phase_place_2_more_stones,
            game_phase_stone_type_decision,
            game_phase_standard_gomoku,
            next_move_stone_types,
            policies,
            values)

        batch_num_data_points = boards.shape[0]
        total_loss += batch_num_data_points*loss.numpy()
        total_policy_loss += batch_num_data_points*policy_loss.numpy()
        total_value_loss += batch_num_data_points*value_loss.numpy()
        num_data_points += batch_num_data_points

    return total_loss / num_data_points, \
           total_policy_loss / num_data_points, \
           total_value_loss / num_data_points, \
           num_data_points

def Train(model_import_path: str,
          data_source: int,
          batch_gen: BatchGenerator, 
          test_batch_gen: BatchGenerator,
          model_export_path: str) -> None:
    src = os.path.join(
        model_import_path, GomokuCnnResNetSharedTowerModelName())
    model = tf.saved_model.load(export_dir=src)

    optimizer = tf.optimizers.Adamax()

    model_vars = CollectGomokuCnnResNetSharedTowerVariables(model=model)

    kTolerance = 2
    kBatchSize = 50

    best_loss = float("inf")
    tolerance = kTolerance
    evaluateAfterNumUpdates = \
        batch_gen.NumDataEntries(data_source=data_source,
                                 training_data=True) // kBatchSize // 5
    if evaluateAfterNumUpdates == 0:
        evaluateAfterNumUpdates = 1

    i = 0
    while True:
        if i % evaluateAfterNumUpdates == 0:
            training_loss, training_policy_loss, training_value_loss, training_n = \
                DataSetLoss(model=model,
                            data_source=data_source,
                            training_data=True,
                            batch_gen=test_batch_gen)

            test_loss, test_policy_loss, test_value_loss, testing_n = \
                DataSetLoss(model=model,
                            data_source=data_source,
                            training_data=False,
                            batch_gen=test_batch_gen)

            logging.info("iteration={0} "\
                         "training=[l={1}, pl={2}, vl={3}, n={4}] "\
                         "testing=[l={5}, pl={6}, vl={7}, n={8}]"\
                .format(i,
                        training_loss,
                        training_policy_loss,
                        training_value_loss,
                        training_n,
                        test_loss,
                        test_policy_loss,
                        test_value_loss,
                        testing_n))
            
            if test_loss < best_loss:
                best_loss = test_loss
                tolerance = kTolerance

                logging.info("Found a candidate model.")
                dst = os.path.join(
                    model_export_path, GomokuCnnResNetSharedTowerModelName())
                tf.saved_model.save(
                    obj=model,
                    export_dir=dst,
                    signatures={"inference": model.__call__,
                                "loss": model.Loss})
            else:
                tolerance -= 1

                logging.info("iteration={0} remaining_tolerance={1}.".format(i, tolerance))

                if tolerance == 0:
                    break

        boards, \
        game_phase_place_3_stones, \
        game_phase_swap2_decision, \
        game_phase_place_2_more_stones, \
        game_phase_stone_type_decision, \
        game_phase_standard_gomoku, \
        next_move_stone_types, \
        policies, \
        values = \
            batch_gen.NextBatch(batch_size=kBatchSize,
                                sample_from=data_source,
                                training_data=True,
                                enable_augmentation=True)

        with tf.GradientTape() as tape:
            loss, _, _ = model.Loss(
                boards,
                game_phase_place_3_stones,
                game_phase_swap2_decision,
                game_phase_place_2_more_stones,
                game_phase_stone_type_decision,
                game_phase_standard_gomoku,
                next_move_stone_types,
                policies,
                values)
            grads = tape.gradient(target=loss, sources=model_vars)
            optimizer.apply_gradients(grads_and_vars=zip(grads, model_vars))

        i += 1

    logging.info("Training loop has been terminated.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Train an old model with the most recently generated game data.")
    parser.add_argument("--model_input_path",
        type=str,
        help="A path to read the old model from. Do not specify a file name.")
    parser.add_argument("--model_output_path",
        type=str,
        help="A path to write the trained model to. Do not specify a file name.")
    parser.add_argument("--data_source",
        type=str,
        help="An integer indicating from which data source the model will be trained.")
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
    data_source = args.data_source
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
    if data_source is None:
        logging.error("Argument data_source is required.")
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

    batch_gen = BatchGenerator(num_data_entries=num_data_entries,
                               db_name=db_name,
                               db_host=db_host,
                               db_port=5432,
                               db_user=db_user,
                               db_pass=db_pass)

    test_batch_gen = BatchGenerator(num_data_entries=num_data_entries,
                                    db_name=db_name,
                                    db_host=db_host,
                                    db_port=5432,
                                    db_user=db_user,
                                    db_pass=db_pass)

    Train(model_import_path=model_input_path,
          data_source=data_source,
          batch_gen=batch_gen, 
          test_batch_gen=test_batch_gen,
          model_export_path=model_output_path)
