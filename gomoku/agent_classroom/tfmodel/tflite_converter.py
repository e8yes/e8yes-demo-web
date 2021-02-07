#!/bin/python3

# This script convert a tensorflow saved model to an 8-bit quantized tflite model.

from typing import List
import argparse
import os
import logging
import tensorflow as tf

from batch_generator import BatchGenerator

gen = BatchGenerator(num_data_entries=None,
                     db_name="demoweb",
                     db_host="localhost",
                     db_port=5432,
                     db_user="postgres",
                     db_pass="password")

def ExampleDataEntries() -> List[any]:
    for i in range(10):
        boards, \
        game_phase_place_3_stones, \
        game_phase_swap2_decision, \
        game_phase_place_2_more_stones, \
        game_phase_stone_type_decision, \
        game_phase_standard_gomoku, \
        next_move_stone_types, \
        _, \
        _ = \
            gen.NextBatch(batch_size=100,
                          sample_from=0,
                          training_data=None,
                          enable_augmentation=True)

        print("step=", i)

        yield [boards,
               game_phase_place_3_stones,
               game_phase_swap2_decision,
               game_phase_place_2_more_stones,
               game_phase_stone_type_decision,
               game_phase_standard_gomoku,
               next_move_stone_types]

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Converts a tensorflow saved model to "
                    "an 8-bit quantized tflite model.")
    parser.add_argument("--model_input_path",
        type=str,
        help="The path to read the saved model from.")

    args = parser.parse_args()
    model_input_path = args.model_input_path
    if model_input_path is None:
        logging.error("Argument model_input_path is required.")
        parser.print_help()
        exit(-1)

    logging.info("Converting to a tf lite model and saving it...")
    converter = tf.lite.TFLiteConverter.from_saved_model(
        saved_model_dir=model_input_path,
        signature_keys=["inference"])
    converter.experimental_new_converter = True
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    converter.representative_dataset = ExampleDataEntries
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS]
    # converter.inference_input_type = tf.int8
    # converter.inference_output_type = tf.int8
    tflite_model = converter.convert()
    open("{0}.tflite".format(model_input_path), "wb").write(tflite_model)
