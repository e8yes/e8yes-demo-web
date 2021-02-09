/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>

#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/heuristics/model_based_evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/game/board_state.h"
#include "third_party/tensorflow/lite/c/c_api.h"

namespace e8 {
namespace {

struct EvaluationResult {
    float reward;
    std::unordered_map<GomokuActionId, float> policy;
};

void ExtractBoardFeatures(GomokuBoardState const &state, TfLiteTensor *board_features) {
    assert(TfLiteTensorNumDims(board_features) == 3);
    assert(TfLiteTensorDim(board_features, /*dim_index=*/0) == 1);
    assert(TfLiteTensorDim(board_features, /*dim_index=*/1) == state.Width());
    assert(TfLiteTensorDim(board_features, /*dim_index=*/2) == state.Height());

    float *tensor_memory = static_cast<float *>(TfLiteTensorData(board_features));

    for (int16_t y = 0; y < state.Height(); ++y) {
        for (int16_t x = 0; x < state.Width(); ++x) {
            switch (*state.ChessPieceStateAt(MovePosition(x, y))) {
            case ST_NONE: {
                tensor_memory[y + x * state.Height()] = 0.0f;
                break;
            }
            case ST_BLACK: {
                tensor_memory[y + x * state.Height()] = -1.0f;
                break;
            }
            case ST_WHITE: {
                tensor_memory[y + x * state.Height()] = 1.0f;
                break;
            }
            default: {
                assert(false);
                break;
            }
            }
        }
    }
}

void Fill(GomokuBoardState const &state, float val, TfLiteTensor *tensor) {
    assert(TfLiteTensorNumDims(tensor) == 3);
    assert(TfLiteTensorDim(tensor, /*dim_index=*/0) == 1);
    assert(TfLiteTensorDim(tensor, /*dim_index=*/1) == state.Width());
    assert(TfLiteTensorDim(tensor, /*dim_index=*/2) == state.Height());

    float *tensor_memory = static_cast<float *>(TfLiteTensorData(tensor));

    for (int16_t y = 0; y < state.Height(); ++y) {
        for (int16_t x = 0; x < state.Width(); ++x) {
            tensor_memory[x + y * state.Width()] = val;
        }
    }
}

void ExtractGamePhaseFeature(GomokuBoardState const &state, TfLiteTensor *game_phase_place_3_stones,
                             TfLiteTensor *game_phase_swap2_decision,
                             TfLiteTensor *game_phase_place_2_more_stones,
                             TfLiteTensor *game_phase_stone_type_decision,
                             TfLiteTensor *game_phase_standard_gomoku) {

    switch (state.CurrentGamePhase()) {
    case GP_PLACE_3_STONES: {
        Fill(state, /*val=*/1.0f, game_phase_place_3_stones);

        Fill(state, /*val=*/0.0f, game_phase_swap2_decision);
        Fill(state, /*val=*/0.0f, game_phase_place_2_more_stones);
        Fill(state, /*val=*/0.0f, game_phase_stone_type_decision);
        Fill(state, /*val=*/0.0f, game_phase_standard_gomoku);
        break;
    }
    case GP_SWAP2_DECISION: {
        Fill(state, /*val=*/1.0f, game_phase_swap2_decision);

        Fill(state, /*val=*/0.0f, game_phase_place_3_stones);
        Fill(state, /*val=*/0.0f, game_phase_place_2_more_stones);
        Fill(state, /*val=*/0.0f, game_phase_stone_type_decision);
        Fill(state, /*val=*/0.0f, game_phase_standard_gomoku);
        break;
    }
    case GP_SWAP2_PLACE_2_STONES: {
        Fill(state, /*val=*/1.0f, game_phase_place_2_more_stones);

        Fill(state, /*val=*/0.0f, game_phase_place_3_stones);
        Fill(state, /*val=*/0.0f, game_phase_swap2_decision);
        Fill(state, /*val=*/0.0f, game_phase_stone_type_decision);
        Fill(state, /*val=*/0.0f, game_phase_standard_gomoku);
        break;
    }
    case GP_STONE_TYPE_DECISION: {
        Fill(state, /*val=*/1.0f, game_phase_stone_type_decision);

        Fill(state, /*val=*/0.0f, game_phase_place_3_stones);
        Fill(state, /*val=*/0.0f, game_phase_swap2_decision);
        Fill(state, /*val=*/0.0f, game_phase_place_2_more_stones);
        Fill(state, /*val=*/0.0f, game_phase_standard_gomoku);
        break;
    }
    case GP_STANDARD_GOMOKU: {
        Fill(state, /*val=*/1.0f, game_phase_standard_gomoku);

        Fill(state, /*val=*/0.0f, game_phase_place_3_stones);
        Fill(state, /*val=*/0.0f, game_phase_swap2_decision);
        Fill(state, /*val=*/0.0f, game_phase_place_2_more_stones);
        Fill(state, /*val=*/0.0f, game_phase_stone_type_decision);
        break;
    }
    default: {
        assert(false);
        break;
    }
    }
}

void ExtractNextMoveStoneTypeFeature(GomokuBoardState const &state,
                                     TfLiteTensor *next_move_stone_type) {
    switch (state.PlayerStoneType(state.CurrentPlayerSide())) {
    case ST_NONE: {
        Fill(state, /*val=*/0.0f, next_move_stone_type);
        break;
    }
    case ST_BLACK: {
        Fill(state, /*val=*/-1.0f, next_move_stone_type);
        break;
    }
    case ST_WHITE: {
        Fill(state, /*val=*/1.0f, next_move_stone_type);
        break;
    }
    default: {
        assert(false);
        break;
    }
    }
}

EvaluationResult ToEvaluationResult(GomokuBoardState const &state,
                                    TfLiteTensor const *policy_tensor,
                                    TfLiteTensor const *value_tensor) {
    EvaluationResult evaluation;

    assert(TfLiteTensorNumDims(policy_tensor) == 2);
    assert(TfLiteTensorDim(policy_tensor, /*dim_index=*/0) == 1);
    auto [lo, hi] = state.ActionIdRange();
    assert(TfLiteTensorDim(policy_tensor, /*dim_index=*/1) == hi - lo + 1);

    // Re-normalizes and stores the policy into the map.
    float *policy = static_cast<float *>(TfLiteTensorData(policy_tensor));

    float norm_factor = 0;
    for (auto const &[action_id, _] : state.LegalActions()) {
        norm_factor += policy[action_id];
    }
    for (auto const &[action_id, _] : state.LegalActions()) {
        evaluation.policy[action_id] = policy[action_id] / norm_factor;
    }

    assert(TfLiteTensorNumDims(value_tensor) == 1);
    assert(TfLiteTensorDim(value_tensor, /*dim_index=*/0) == 1);

    // Value prediction can be treated as the expected reward.
    float *values = static_cast<float *>(TfLiteTensorData(value_tensor));
    evaluation.reward = values[0];

    return evaluation;
}

int FindTensorIndexByName(std::string const &name, bool input, TfLiteInterpreter *interpreter) {
    unsigned num_tensors = input ? TfLiteInterpreterGetInputTensorCount(interpreter)
                                 : TfLiteInterpreterGetOutputTensorCount(interpreter);

    for (unsigned i = 0; i < num_tensors; ++i) {
        TfLiteTensor const *tensor = input ? TfLiteInterpreterGetInputTensor(interpreter, i)
                                           : TfLiteInterpreterGetOutputTensor(interpreter, i);
        std::string tensor_name = TfLiteTensorName(tensor);
        if (name == tensor_name) {
            return i;
        }
    }

    return -1;
}

} // namespace

struct GomokuModelBasedEvaluator::ModelBasedEvaluatorInternal {
    ModelBasedEvaluatorInternal(std::string const &model_path);
    ~ModelBasedEvaluatorInternal();

    EvaluationResult Fetch(MctNodeId const state_id, GomokuBoardState const &state);

    TfLiteModel *model;
    TfLiteInterpreterOptions *interpreter_options;
    TfLiteInterpreter *interpreter;

    int board_features_idx;
    int game_phase_place_3_stones_idx;
    int game_phase_swap2_decision_idx;
    int game_phase_place_2_more_stones_idx;
    int game_phase_stone_type_decision_idx;
    int game_phase_standard_gomoku_idx;
    int next_move_stone_type_idx;

    int policy_idx;
    int value_idx;

    std::unordered_map<MctNodeId, EvaluationResult> cache;
};

GomokuModelBasedEvaluator::ModelBasedEvaluatorInternal::ModelBasedEvaluatorInternal(
    std::string const &model_path) {
    model = TfLiteModelCreateFromFile(model_path.c_str());
    assert(model != nullptr);

    interpreter_options = TfLiteInterpreterOptionsCreate();
    TfLiteInterpreterOptionsSetNumThreads(interpreter_options, std::thread::hardware_concurrency());

    interpreter = TfLiteInterpreterCreate(model, interpreter_options);
    assert(interpreter != nullptr);

    board_features_idx =
        FindTensorIndexByName(/*name=*/"inference_board_features:0", /*input=*/true, interpreter);
    assert(board_features_idx != -1);

    game_phase_place_3_stones_idx = FindTensorIndexByName(
        /*name=*/"inference_game_phase_place_3_stones:0", /*input=*/true, interpreter);
    assert(game_phase_place_3_stones_idx != -1);

    game_phase_swap2_decision_idx = FindTensorIndexByName(
        /*name=*/"inference_game_phase_swap2_decision:0", /*input=*/true, interpreter);
    assert(game_phase_swap2_decision_idx != -1);

    game_phase_place_2_more_stones_idx = FindTensorIndexByName(
        /*name=*/"inference_game_phase_place_2_more_stones:0", /*input=*/true, interpreter);
    assert(game_phase_place_2_more_stones_idx != -1);

    game_phase_stone_type_decision_idx = FindTensorIndexByName(
        /*name=*/"inference_game_phase_stone_type_decision:0", /*input=*/true, interpreter);
    assert(game_phase_stone_type_decision_idx != -1);

    game_phase_standard_gomoku_idx = FindTensorIndexByName(
        /*name=*/"inference_game_phase_standard_gomoku:0", /*input=*/true, interpreter);
    assert(game_phase_standard_gomoku_idx != -1);

    next_move_stone_type_idx = FindTensorIndexByName(/*name=*/"inference_next_move_stone_type:0",
                                                     /*input=*/true, interpreter);
    assert(next_move_stone_type_idx != -1);

    policy_idx = FindTensorIndexByName(/*name=*/"StatefulPartitionedCall:0",
                                       /*input=*/false, interpreter);
    assert(policy_idx != -1);

    value_idx = FindTensorIndexByName(/*name=*/"StatefulPartitionedCall:1",
                                      /*input=*/false, interpreter);
    assert(value_idx != -1);

    TfLiteStatus status = TfLiteInterpreterAllocateTensors(interpreter);
    assert(status == TfLiteStatus::kTfLiteOk);
}

GomokuModelBasedEvaluator::ModelBasedEvaluatorInternal::~ModelBasedEvaluatorInternal() {
    TfLiteInterpreterDelete(interpreter);
    TfLiteModelDelete(model);
    TfLiteInterpreterOptionsDelete(interpreter_options);
}

EvaluationResult
GomokuModelBasedEvaluator::ModelBasedEvaluatorInternal::Fetch(MctNodeId const state_id,
                                                              GomokuBoardState const &state) {
    auto it = cache.find(state_id);
    if (it != cache.end()) {
        return it->second;
    }

    TfLiteTensor *board_features = TfLiteInterpreterGetInputTensor(interpreter, board_features_idx);
    ExtractBoardFeatures(state, board_features);

    TfLiteTensor *game_phase_place_3_stones =
        TfLiteInterpreterGetInputTensor(interpreter, game_phase_place_3_stones_idx);
    TfLiteTensor *game_phase_swap2_decision =
        TfLiteInterpreterGetInputTensor(interpreter, game_phase_swap2_decision_idx);
    TfLiteTensor *game_phase_place_2_more_stones =
        TfLiteInterpreterGetInputTensor(interpreter, game_phase_place_2_more_stones_idx);
    TfLiteTensor *game_phase_stone_type_decision =
        TfLiteInterpreterGetInputTensor(interpreter, game_phase_stone_type_decision_idx);
    TfLiteTensor *game_phase_standard_gomoku =
        TfLiteInterpreterGetInputTensor(interpreter, game_phase_standard_gomoku_idx);
    ExtractGamePhaseFeature(state, game_phase_place_3_stones, game_phase_swap2_decision,
                            game_phase_place_2_more_stones, game_phase_stone_type_decision,
                            game_phase_standard_gomoku);

    TfLiteTensor *next_move_stone_type =
        TfLiteInterpreterGetInputTensor(interpreter, next_move_stone_type_idx);
    ExtractNextMoveStoneTypeFeature(state, next_move_stone_type);

    TfLiteStatus status = TfLiteInterpreterInvoke(interpreter);
    assert(status == TfLiteStatus::kTfLiteOk);

    TfLiteTensor const *policy = TfLiteInterpreterGetOutputTensor(interpreter, policy_idx);
    TfLiteTensor const *value = TfLiteInterpreterGetOutputTensor(interpreter, value_idx);

    EvaluationResult evaluation = ToEvaluationResult(state, policy, value);
    cache.insert(std::make_pair(state_id, evaluation));

    return evaluation;
}

GomokuModelBasedEvaluator::GomokuModelBasedEvaluator(std::string const &model_path)
    : pimpl_(std::make_unique<ModelBasedEvaluatorInternal>(model_path)) {}

GomokuModelBasedEvaluator::~GomokuModelBasedEvaluator() {}

float GomokuModelBasedEvaluator::EvaluateReward(GomokuBoardState const &state,
                                                MctNodeId const state_id) {
    return pimpl_->Fetch(state_id, state).reward;
}

std::unordered_map<GomokuActionId, float>
GomokuModelBasedEvaluator::EvaluatePolicy(GomokuBoardState const &state, MctNodeId const state_id) {
    return pimpl_->Fetch(state_id, state).policy;
}

float GomokuModelBasedEvaluator::ExplorationFactor() const { return 1.0f; }

unsigned GomokuModelBasedEvaluator::NumSimulations() const { return 2000; }

void GomokuModelBasedEvaluator::ClearCache() { pimpl_->cache.clear(); }

std::string GomokuModelBasedEvaluator::ModelName() const {
    // TODO: Do not hardcode the model name.
    return "gomoku_cnn_shared_tower_11_11_b2";
}

} // namespace e8
