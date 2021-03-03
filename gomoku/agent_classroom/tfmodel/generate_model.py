#!/bin/python3

# This script generates an untrained gomoku model.
import argparse
import os
import logging
import tensorflow as tf
import numpy as np

from poly_functions import RequireShlFeatures
from cnn_resnet_shared_tower_model import GomokuCnnResNetSharedTowerModel
from cnn_shared_model import GomokuCnnSharedModel
from cnn_shared_with_shl_model import GomokuCnnShlSharedModel

def GenerateModel(model_class: str, model_output_path: str):
    logging.info("Constructing the graph...")

    tf.summary.trace_on(graph=True, profiler=False) 

    model = None
    if model_class == "cnn_resnet_shared_tower":
        model = GomokuCnnResNetSharedTowerModel()
    elif model_class == "cnn_shared_with_shl":
        model = GomokuCnnShlSharedModel()
    elif model_class == "cnn_shared":
        model = GomokuCnnSharedModel()
    else:
        raise "Unkown model_class=" + str(model_class)

    boards = tf.constant(
        value=[[[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]],
               [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=tf.uint8)
    game_phases = tf.constant(value=[0, 4], dtype=tf.uint8)
    next_move_stone_types = tf.constant(value=[2, 1], dtype=tf.uint8)

    primary_shl_count_black = tf.constant(
        value=[[[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]],
               [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=tf.float32)
    secondary_shl_count_black = tf.constant(
        value=[[[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]],
               [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=tf.float32)
    
    primary_shl_count_white = tf.constant(
        value=[[[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]],
               [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=tf.float32)
    secondary_shl_count_white = tf.constant(
        value=[[[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]],
               [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0],
                [0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=tf.float32)
    
    shl_map = np.concatenate(
        (np.reshape(a=primary_shl_count_black, newshape=(-1, 11, 11, 1)),
         np.reshape(a=secondary_shl_count_black, newshape=(-1, 11, 11, 1)),
         np.reshape(a=primary_shl_count_white, newshape=(-1, 11, 11, 1)),
         np.reshape(a=secondary_shl_count_white, newshape=(-1, 11, 11, 1))),
        axis=3)
    
    shl_top_shl_features = np.ones(shape=(2, 60), dtype=np.float32)

    ground_truth_policy = tf.constant(
        value=[[0.007936508]*126,
               [0.007936508]*126],
        dtype=tf.float32)
    ground_truth_value = tf.constant(
        value=[0, 0],
        dtype=tf.float32)

    policy = None
    value = None

    if RequireShlFeatures(model_name=model_class):
        policy, value = model(
            boards,
            game_phases,
            next_move_stone_types,
            shl_map,
            shl_top_shl_features)
    else:
        policy, value = model(
            boards,
            game_phases,
            next_move_stone_types)
    
    logging.info("policy=\n{0}".format(policy.numpy()))
    logging.info("value=\n{0}".format(value.numpy()))

    loss = None
    policy_loss = None
    value_loss = None
    reg_loss = None

    if RequireShlFeatures(model_name=model_class):
        loss, policy_loss, value_loss, reg_loss = model.Loss(
            boards,
            game_phases,
            next_move_stone_types,
            shl_map,
            shl_top_shl_features,
            ground_truth_policy,
            ground_truth_value)
    else:
        loss, policy_loss, value_loss, reg_loss = model.Loss(
            boards,
            game_phases,
            next_move_stone_types,
            ground_truth_policy,
            ground_truth_value)
    
    logging.info("loss={0}, policy_loss={1}, value_loss={2}, reg_loss={3}"\
        .format(loss.numpy(), 
                policy_loss.numpy(), 
                value_loss.numpy(),
                reg_loss.numpy()))

    model_full_path = os.path.join(
        model_output_path,
        model.Name().numpy().decode("UTF-8"))

    logging.info("Saving the graph into jounrnal...")
    log_path = os.path.join(model_full_path, "logs")
    writer = tf.summary.create_file_writer(logdir=log_path)
    with writer.as_default():
        tf.summary.trace_export(
            name=model.Name().numpy().decode("UTF-8"),
            step=0,
            profiler_outdir=log_path)

    logging.info("Saving the model...")
    tf.saved_model.save(obj=model,
                        export_dir=model_full_path,
                        signatures={"inference": model.__call__,
                                    "loss": model.Loss,
                                    "name": model.Name})

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, 
                        format="%(asctime)s %(levelname)s %(message)s")

    parser = argparse.ArgumentParser(
        description="Generate an empty Gomoku tensorflow model")
    parser.add_argument("--model_class",
        type=str,
        help="The type of model to generate.")
    parser.add_argument("--model_output_path",
        type=str,
        help="A path to write the model to. Do not specify a file name.")

    args = parser.parse_args()
    model_class = args.model_class
    model_output_path = args.model_output_path

    if model_class is None:
        logging.error("Argument model_class is required.")
        parser.print_help()
        exit(-1)

    if model_output_path is None:
        logging.error("Argument model_output_path is required.")
        parser.print_help()
        exit(-1)

    GenerateModel(
        model_class=model_class, model_output_path=model_output_path)
