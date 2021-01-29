# This script generates an untrained gomoku model.
import argparse
import os
import logging
import tensorflow as tf

from cnn_resnet_shared_tower_model import GomokuCnnResNetSharedTower

def GenerateModel(model_output_path: str):
    logging.info("Constructing the graph...")

    tf.summary.trace_on(graph=True, profiler=False) 

    model = GomokuCnnResNetSharedTower()

    board_features = tf.constant(
        value=[[[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=tf.float32)
    
    game_phase_place_3_stones = tf.constant(value=[1], dtype=tf.float32)
    game_phase_swap2_decision = tf.constant(value=[0], dtype=tf.float32)
    game_phase_place2_more_stones = tf.constant(value=[0], dtype=tf.float32)
    game_phase_stone_type_decision = tf.constant(value=[0], dtype=tf.float32)
    game_phase_standard_gomoku = tf.constant(value=[0], dtype=tf.float32)

    next_move_stone_type = tf.constant(value=[1], dtype=tf.float32)

    policy, value, _ = model(board_features,
                             game_phase_place_3_stones,
                             game_phase_swap2_decision,
                             game_phase_place2_more_stones,
                             game_phase_stone_type_decision,
                             game_phase_standard_gomoku,
                             next_move_stone_type)
    logging.info("policy=\n{0}".format(policy.numpy()))
    logging.info("value=\n{0}".format(value.numpy()))

    model_full_path = os.path.join(model_output_path, model.Name())

    logging.info("Saving the graph into jounrnal...")
    writer = tf.summary.create_file_writer(logdir=os.path.join(model_full_path, "logs"))
    with writer.as_default():
        tf.summary.trace_export(
            name=model.Name(),
            step=0,
            profiler_outdir=os.path.join("logs"))

    logging.info("Saving the model...")
    tf.saved_model.save(obj=model,
                        export_dir=model_full_path,
                        signatures={"serving_default": model.__call__})

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, 
                        format="%(asctime)s %(levelname)s %(message)s")

    parser = argparse.ArgumentParser(
        description="Generate an empty Gomoku tensorflow model")
    parser.add_argument("--model_output_path",
        type=str,
        help="A path to write the model to. Do not specify a file name.")

    args = parser.parse_args()
    model_output_path = args.model_output_path
    if model_output_path is None:
        logging.error("Argument model_output_path is required.")
        parser.print_help()
        exit(-1)

    GenerateModel(model_output_path=model_output_path)
