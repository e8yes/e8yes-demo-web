from typing import List
import os
import re
import tensorflow as tf

import cnn_resnet_shared_tower_model
import cnn_shared_model
import cnn_shared_with_shl_model

def ReadModelName(model_import_path: str) -> str:
    for entry in os.listdir(model_import_path):
        entry_path = os.path.join(model_import_path, entry)
        if os.path.isdir(entry_path):
            return entry
    return None

def LoadModel(model_import_path: str) -> any:
    model_name = ReadModelName(model_import_path=model_import_path)
    model = tf.saved_model.load(
        export_dir=os.path.join(model_import_path, model_name))
    return model

def SaveModel(model: any, model_export_path: str):
    model_name = model.Name().numpy().decode("UTF-8")
    tf.saved_model.save(
        obj=model,
        export_dir=os.path.join(model_export_path, model_name),
        signatures={"inference": model.__call__,
                    "loss": model.Loss})

def ReadBoardSize(model_name: str) -> int:
    match = re.findall(pattern=r"_i\d+", string=model_name)
    assert(len(match) == 1)
    board_size = re.findall(pattern=r"\d+", string=match[0])
    return int(board_size[0])

def RequireShlFeatures(model_name: str) -> bool:
    return "with_shl" in model_name

def TrainableVariables(model: any) -> List[tf.Variable]:
    model_name = model.Name().numpy().decode("UTF-8")

    if "gomoku_cnn_shared_tower" in model_name:
        return cnn_resnet_shared_tower_model.\
            CollectGomokuCnnResNetSharedTowerVariables(model)
    elif "gomoku_cnn_shared_with_shl" in model_name:
        return cnn_shared_with_shl_model.\
            TrainableVariables(model)
    elif "gomoku_cnn_shared" in model_name:
        return cnn_shared_model.TrainableVariables(model)
    
    assert(False)