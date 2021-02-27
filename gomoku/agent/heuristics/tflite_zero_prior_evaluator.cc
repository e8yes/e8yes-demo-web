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
#include "gomoku/agent/heuristics/tflite_zero_prior_evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/game/board_state.h"
#include "third_party/tensorflow/lite/c/c_api.h"

namespace e8 {
namespace {

struct EvaluationResult {
    float reward;
    std::unordered_map<GomokuActionId, float> policy;
};

void WriteBoard(GomokuBoardState const &state, TfLiteTensor *board) {
    assert(TfLiteTensorNumDims(board) == 3);
    assert(TfLiteTensorDim(board, /*dim_index=*/0) == 1);
    assert(TfLiteTensorDim(board, /*dim_index=*/1) == state.Width());
    assert(TfLiteTensorDim(board, /*dim_index=*/2) == state.Height());

    uint8_t *tensor_memory = static_cast<uint8_t *>(TfLiteTensorData(board));

    for (int16_t y = 0; y < state.Height(); ++y) {
        for (int16_t x = 0; x < state.Width(); ++x) {
            tensor_memory[y + x * state.Height()] = *state.ChessPieceStateAt(MovePosition(x, y));
        }
    }
}

void WriteGamePhase(GomokuBoardState const &state, TfLiteTensor *game_phase) {
    assert(TfLiteTensorNumDims(game_phase) == 1);
    assert(TfLiteTensorDim(game_phase, /*dim_index=*/0) == 1);

    uint8_t *tensor_memory = static_cast<uint8_t *>(TfLiteTensorData(game_phase));
    *tensor_memory = state.CurrentGamePhase();
}

void WriteNextMoveStoneType(GomokuBoardState const &state, TfLiteTensor *next_move_stone_type) {
    assert(TfLiteTensorNumDims(next_move_stone_type) == 1);
    assert(TfLiteTensorDim(next_move_stone_type, /*dim_index=*/0) == 1);

    uint8_t *tensor_memory = static_cast<uint8_t *>(TfLiteTensorData(next_move_stone_type));
    *tensor_memory = state.PlayerStoneType(state.CurrentPlayerSide());
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

struct GomokuTfliteZeroPriorEvaluator::ModelBasedEvaluatorInternal {
    ModelBasedEvaluatorInternal(std::string const &model_path);
    ~ModelBasedEvaluatorInternal();

    EvaluationResult Fetch(MctNodeId const state_id, GomokuBoardState const &state);

    TfLiteModel *model;
    TfLiteInterpreterOptions *interpreter_options;
    TfLiteInterpreter *interpreter;

    int board_idx;
    int game_phase_idx;
    int next_move_stone_type_idx;

    int policy_idx;
    int value_idx;

    std::unordered_map<MctNodeId, EvaluationResult> cache;
};

GomokuTfliteZeroPriorEvaluator::ModelBasedEvaluatorInternal::ModelBasedEvaluatorInternal(
    std::string const &model_path) {
    model = TfLiteModelCreateFromFile(model_path.c_str());
    assert(model != nullptr);

    interpreter_options = TfLiteInterpreterOptionsCreate();
    TfLiteInterpreterOptionsSetNumThreads(interpreter_options, std::thread::hardware_concurrency());

    interpreter = TfLiteInterpreterCreate(model, interpreter_options);
    assert(interpreter != nullptr);

    board_idx = FindTensorIndexByName(/*name=*/"inference_boards:0", /*input=*/true, interpreter);
    assert(board_idx != -1);

    game_phase_idx = FindTensorIndexByName(
        /*name=*/"inference_game_phases:0", /*input=*/true, interpreter);
    assert(game_phase_idx != -1);

    next_move_stone_type_idx = FindTensorIndexByName(/*name=*/"inference_next_move_stone_types:0",
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

GomokuTfliteZeroPriorEvaluator::ModelBasedEvaluatorInternal::~ModelBasedEvaluatorInternal() {
    TfLiteInterpreterDelete(interpreter);
    TfLiteModelDelete(model);
    TfLiteInterpreterOptionsDelete(interpreter_options);
}

EvaluationResult
GomokuTfliteZeroPriorEvaluator::ModelBasedEvaluatorInternal::Fetch(MctNodeId const state_id,
                                                              GomokuBoardState const &state) {
    auto it = cache.find(state_id);
    if (it != cache.end()) {
        return it->second;
    }

    TfLiteTensor *board_features = TfLiteInterpreterGetInputTensor(interpreter, board_idx);
    WriteBoard(state, board_features);

    TfLiteTensor *game_phase = TfLiteInterpreterGetInputTensor(interpreter, game_phase_idx);
    WriteGamePhase(state, game_phase);

    TfLiteTensor *next_move_stone_type =
        TfLiteInterpreterGetInputTensor(interpreter, next_move_stone_type_idx);
    WriteNextMoveStoneType(state, next_move_stone_type);

    TfLiteStatus status = TfLiteInterpreterInvoke(interpreter);
    assert(status == TfLiteStatus::kTfLiteOk);

    TfLiteTensor const *policy = TfLiteInterpreterGetOutputTensor(interpreter, policy_idx);
    TfLiteTensor const *value = TfLiteInterpreterGetOutputTensor(interpreter, value_idx);

    EvaluationResult evaluation = ToEvaluationResult(state, policy, value);
    cache.insert(std::make_pair(state_id, evaluation));

    return evaluation;
}

GomokuTfliteZeroPriorEvaluator::GomokuTfliteZeroPriorEvaluator(std::string const &model_path)
    : pimpl_(std::make_unique<ModelBasedEvaluatorInternal>(model_path)) {}

GomokuTfliteZeroPriorEvaluator::~GomokuTfliteZeroPriorEvaluator() {}

float GomokuTfliteZeroPriorEvaluator::EvaluateReward(GomokuBoardState const &state,
                                                MctNodeId const state_id) {
    return pimpl_->Fetch(state_id, state).reward;
}

std::unordered_map<GomokuActionId, float>
GomokuTfliteZeroPriorEvaluator::EvaluatePolicy(GomokuBoardState const &state, MctNodeId const state_id) {
    return pimpl_->Fetch(state_id, state).policy;
}

float GomokuTfliteZeroPriorEvaluator::ExplorationFactor() const { return 5.0f; }

unsigned GomokuTfliteZeroPriorEvaluator::NumSimulations() const { return 2000; }

void GomokuTfliteZeroPriorEvaluator::ClearCache() { pimpl_->cache.clear(); }

} // namespace e8
