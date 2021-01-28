# This script generates an untrained gomoku model.
import argparse
import os
import logging
import tensorflow as tf

from cnn_resnet_shared_tower_model import GomokuCnnResNetSharedTower

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

    logging.info("Constructing the graph...")

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
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]],
               [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, -1, 1, 1, 1, 1, -1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=tf.float32)
    
    game_phase_place_3_stones = tf.constant(value=[1, 0], dtype=tf.float32)
    game_phase_swap2_decision = tf.constant(value=[0, 0], dtype=tf.float32)
    game_phase_place2_more_stones = tf.constant(value=[0, 0], dtype=tf.float32)
    game_phase_stone_type_decision = tf.constant(value=[0, 0], dtype=tf.float32)
    game_phase_standard_gomoku = tf.constant(value=[0, 1], dtype=tf.float32)

    next_move_stone_type = tf.constant(value=[1, -1], dtype=tf.float32)

    policy, value, _ = model(board_features,
                             game_phase_place_3_stones,
                             game_phase_swap2_decision,
                             game_phase_place2_more_stones,
                             game_phase_stone_type_decision,
                             game_phase_standard_gomoku,
                             next_move_stone_type)
    logging.info("policy=\n{0}".format(policy.numpy()))
    logging.info("value=\n{0}".format(value.numpy()))

    logging.info("Saving the model...")
    model_full_path = os.path.join(model_output_path, model.Name())
    tf.saved_model.save(obj=model, export_dir=model_full_path)

    logging.info("Converting to a tf lite model and saving it...")
    converter = tf.lite.TFLiteConverter.from_saved_model(model_full_path)
    converter.experimental_new_converter = True
    converter.optimizations = [tf.lite.Optimize.OPTIMIZE_FOR_LATENCY]
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS]
    tflite_model = converter.convert()
    open("{0}.tflite".format(model_full_path), "wb").write(tflite_model)
