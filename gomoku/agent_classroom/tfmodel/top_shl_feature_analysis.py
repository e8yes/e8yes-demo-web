import numpy as np
from sklearn.ensemble import GradientBoostingRegressor

from batch_generator import BatchGenerator
from augmentation import AugmentData

batch_gen = BatchGenerator(
        board_size=11,
        num_data_entries=None,
        db_name="demoweb",
        db_host="localhost",
        db_port=5432,
        db_user="postgres",
        db_pass="password")

boards, \
game_phases, \
next_move_stone_types, \
shl_maps, \
top_shl_features, \
policies, \
values = \
    batch_gen.NextBatch(batch_size=100000,
                        training_data=True)

_, \
_, \
next_move_stone_types, \
_, \
top_shl_features, \
_, \
values = \
    AugmentData(
        boards=boards,
        game_phases=game_phases,
        next_move_stone_types=next_move_stone_types,
        shl_maps=shl_maps,
        top_shl_features=top_shl_features,
        policies=policies,
        values=values,
        percentage=1)

_, \
_, \
test_next_move_stone_types, \
_, \
test_top_shl_features, \
_, \
test_values = \
    batch_gen.NextBatch(batch_size=100000,
                        training_data=False)

def ToX(top_shl_features, next_move_stone_types):
    next_move_one_hot = np.zeros(
        (next_move_stone_types.shape[0], 3),
        dtype=np.float32)
    next_move_one_hot[next_move_stone_types == 0, 0] = 1
    next_move_one_hot[next_move_stone_types == 1, 1] = 1
    next_move_one_hot[next_move_stone_types == 2, 2] = 1

    return np.concatenate(
        (top_shl_features, next_move_one_hot), axis=1)


train_x = ToX(top_shl_features, next_move_stone_types)
train_y = values

test_x = ToX(
    test_top_shl_features, test_next_move_stone_types)
test_y = test_values

model = GradientBoostingRegressor(n_estimators=15, max_depth=8)
model.fit(train_x, train_y)

print("feature_importance=", model.feature_importances_)

train_pred_y = model.predict(train_x)
test_pred_y = model.predict(test_x)

def Mse(value_pred, value):
    diff = value_pred - value
    return np.mean(a=diff*diff, axis=0), diff.shape[0]

def Accuracy(value_preds, values):
    no_tie_values = values[values != 0]
    no_tie_value_preds = value_preds[values != 0]

    no_tie_value_preds[no_tie_value_preds >= 0] = 1
    no_tie_value_preds[no_tie_value_preds < 0] = -1

    return np.sum(no_tie_values == no_tie_value_preds)/no_tie_values.shape[0], \
           no_tie_values.shape[0]

train_mse, train_n = Mse(train_pred_y, train_y)
print("Train MSE=", train_mse, "n=", train_n)

train_acc, train_n = Accuracy(train_pred_y, train_y)
print("Train Accuracy=", train_acc, "n=", train_n)

test_mse, test_n = Mse(test_pred_y, test_y)
print("Test MSE=", test_mse, "n=", test_n)

test_acc, test_n = Accuracy(test_pred_y, test_y)
print("Test Accuracy=", test_acc, "n=", test_n)