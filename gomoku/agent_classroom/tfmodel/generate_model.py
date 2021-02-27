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
from cnn_shl_shared_model import GomokuCnnShlSharedModel

def GenerateModel(model_name: str, model_output_path: str):
    logging.info("Constructing the graph...")

    tf.summary.trace_on(graph=True, profiler=False) 

    model = None
    if "cnn_resnet_shared_tower" in model_name:
        model = GomokuCnnResNetSharedTowerModel()
    elif "cnn_shared" in model_name:
        model = GomokuCnnSharedModel()
    elif "cnn_shl_shared" in model_name:
        model = GomokuCnnShlSharedModel()
    else:
        raise "Unkown model_name=" + str(model_name)

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

    ground_truth_policy = tf.constant(
        value=[[0.007936508]*126,
               [0.007936508]*126],
        dtype=tf.float32)
    ground_truth_value = tf.constant(
        value=[0, 0],
        dtype=tf.float32)

    policy = None
    value = None

    if RequireShlFeatures(model_name=model_name):
        policy, value = model(
            boards,
            game_phases,
            next_move_stone_types,
            primary_shl_count_black,
            secondary_shl_count_black,
            primary_shl_count_white,
            secondary_shl_count_white)
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

    if RequireShlFeatures(model_name=model_name):
        loss, policy_loss, value_loss, reg_loss = model.Loss(
            boards,
            game_phases,
            next_move_stone_types,
            primary_shl_count_black,
            secondary_shl_count_black,
            primary_shl_count_white,
            secondary_shl_count_white,
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
    parser.add_argument("--model_name",
        type=str,
        help="The type of model to generate.")
    parser.add_argument("--model_output_path",
        type=str,
        help="A path to write the model to. Do not specify a file name.")

    args = parser.parse_args()
    model_name = args.model_name
    model_output_path = args.model_output_path

    if model_name is None:
        logging.error("Argument model_name is required.")
        parser.print_help()
        exit(-1)

    if model_output_path is None:
        logging.error("Argument model_output_path is required.")
        parser.print_help()
        exit(-1)

    GenerateModel(
        model_name=model_name, model_output_path=model_output_path)
