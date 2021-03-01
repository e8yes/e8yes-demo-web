import os
import tensorflow as tf

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

def RequireShlFeatures(model_name: str):
    return "shl" in model_name