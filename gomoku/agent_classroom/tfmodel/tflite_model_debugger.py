import numpy as np
import tflite_runtime.interpreter as tflite
import platform
import time

def FindTensorIndex(key: str, details: any) -> int:
    for slot in details:
        if slot["name"] == key:
            return slot["index"]
    
    raise "key not found."

if __name__ == "__main__":
    model_path = "/home/davis/saved_models/1/gomoku_cnn_shared_tower_11_11_b2.tflite"

    interpreter = tflite.Interpreter(model_path=model_path)
    interpreter.allocate_tensors()

    board_features = np.array(
        object=[[[0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0],
                 [0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 1, 1, 1, -1, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=np.float32)
    
    game_phase_place_3_stones = np.array(
        object=[[[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=np.float32)
    game_phase_swap2_decision = np.array(
        object=[[[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=np.float32)
    game_phase_place_2_more_stones = np.array(
        object=[[[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=np.float32)
    game_phase_stone_type_decision = np.array(
        object=[[[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=np.float32)
    game_phase_standard_gomoku = np.array(
        object=[[[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]]],
        dtype=np.float32)
    next_move_stone_type = np.array(
        object=[[[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]]],
        dtype=np.float32)

    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()

    print(input_details)
    print(output_details)

    interpreter.set_tensor(
        tensor_index=FindTensorIndex(
            key="inference_board_features:0", details=input_details), 
        value=board_features)

    interpreter.set_tensor(
        tensor_index=FindTensorIndex(
            key="inference_game_phase_place_3_stones:0", details=input_details), 
        value=game_phase_place_3_stones)

    interpreter.set_tensor(
        tensor_index=FindTensorIndex(
            key="inference_game_phase_swap2_decision:0", details=input_details), 
        value=game_phase_swap2_decision)

    interpreter.set_tensor(
        tensor_index=FindTensorIndex(
            key="inference_game_phase_place_2_more_stones:0", details=input_details), 
        value=game_phase_place_2_more_stones)

    interpreter.set_tensor(
        tensor_index=FindTensorIndex(
            key="inference_game_phase_stone_type_decision:0", details=input_details), 
        value=game_phase_stone_type_decision)

    interpreter.set_tensor(
        tensor_index=FindTensorIndex(
            key="inference_game_phase_standard_gomoku:0", details=input_details), 
        value=game_phase_standard_gomoku)

    interpreter.set_tensor(
        tensor_index=FindTensorIndex(
            key="inference_next_move_stone_type:0", details=input_details), 
        value=next_move_stone_type)

    interpreter.invoke()

    policy = interpreter.get_tensor(
        tensor_index=FindTensorIndex(key="StatefulPartitionedCall:0", details=output_details))
    value = interpreter.get_tensor(
        tensor_index=FindTensorIndex(key="StatefulPartitionedCall:1", details=output_details))
    policy_logits = interpreter.get_tensor(
        tensor_index=FindTensorIndex(key="StatefulPartitionedCall:2", details=output_details))

    print("policy=", policy)
    i = np.argmax(policy)
    x = i % 11
    y = i // 11
    print(i, x, y)
    print("value=", value)
    print("policy_logits=", policy_logits)
