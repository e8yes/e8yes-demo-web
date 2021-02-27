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
#include "gomoku/agent/heuristics/tf_zero_prior_evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/game/board_state.h"
#include "third_party/tensorflow/c/c_api.h"
#include "third_party/tensorflow/c/tf_datatype.h"
#include "third_party/tensorflow/c/tf_status.h"
#include "third_party/tensorflow/c/tf_tensor.h"

namespace e8 {
namespace {

struct EvaluationResult {
    float reward;
    std::unordered_map<GomokuActionId, float> policy;
};

void WriteBoard(GomokuBoardState const &state, TF_Tensor *board) {
    assert(TF_NumDims(board) == 3);
    assert(TF_Dim(board, /*dim_index=*/0) == 1);
    assert(TF_Dim(board, /*dim_index=*/1) == state.Width());
    assert(TF_Dim(board, /*dim_index=*/2) == state.Height());

    uint8_t *tensor_memory = static_cast<uint8_t *>(TF_TensorData(board));

    for (int16_t y = 0; y < state.Height(); ++y) {
        for (int16_t x = 0; x < state.Width(); ++x) {
            tensor_memory[y + x * state.Height()] = *state.ChessPieceStateAt(MovePosition(x, y));
        }
    }
}

void WriteGamePhase(GomokuBoardState const &state, TF_Tensor *game_phase) {
    assert(TF_NumDims(game_phase) == 1);
    assert(TF_Dim(game_phase, /*dim_index=*/0) == 1);

    uint8_t *tensor_memory = static_cast<uint8_t *>(TF_TensorData(game_phase));
    *tensor_memory = state.CurrentGamePhase();
}

void WriteNextMoveStoneType(GomokuBoardState const &state, TF_Tensor *next_move_stone_type) {
    assert(TF_NumDims(next_move_stone_type) == 1);
    assert(TF_Dim(next_move_stone_type, /*dim_index=*/0) == 1);

    uint8_t *tensor_memory = static_cast<uint8_t *>(TF_TensorData(next_move_stone_type));
    *tensor_memory = state.PlayerStoneType(state.CurrentPlayerSide());
}

EvaluationResult ToEvaluationResult(GomokuBoardState const &state, TF_Tensor const *policy_tensor,
                                    TF_Tensor const *value_tensor) {
    EvaluationResult evaluation;

    assert(TF_NumDims(policy_tensor) == 2);
    assert(TF_Dim(policy_tensor, /*dim_index=*/0) == 1);
    auto [lo, hi] = state.ActionIdRange();
    assert(TF_Dim(policy_tensor, /*dim_index=*/1) == hi - lo + 1);

    // Re-normalizes and stores the policy into the map.
    float *policy = static_cast<float *>(TF_TensorData(policy_tensor));

    float norm_factor = 0;
    for (auto const &[action_id, _] : state.LegalActions()) {
        norm_factor += policy[action_id];
    }
    for (auto const &[action_id, _] : state.LegalActions()) {
        evaluation.policy[action_id] = policy[action_id] / norm_factor;
    }

    assert(TF_NumDims(value_tensor) == 1);
    assert(TF_Dim(value_tensor, /*dim_index=*/0) == 1);

    // Value prediction can be treated as the expected reward.
    float *values = static_cast<float *>(TF_TensorData(value_tensor));
    evaluation.reward = values[0];

    return evaluation;
}

} // namespace

struct GomokuTfZeroPriorEvaluator::TfModelBasedEvaluatorInternal {
    TfModelBasedEvaluatorInternal(std::string const &model_path);
    ~TfModelBasedEvaluatorInternal();

    EvaluationResult Fetch(MctNodeId const state_id, GomokuBoardState const &state);

    TF_Session *session = nullptr;
    TF_Graph *graph = nullptr;
    TF_Buffer *graph_def = nullptr;

    TF_Tensor *board_input_value = nullptr;
    TF_Tensor *game_phase_input_value = nullptr;
    TF_Tensor *next_move_stone_type_input_value = nullptr;

    std::unordered_map<MctNodeId, EvaluationResult> cache;
};

GomokuTfZeroPriorEvaluator::TfModelBasedEvaluatorInternal::TfModelBasedEvaluatorInternal(
    std::string const &model_path) {
    graph = TF_NewGraph();
    graph_def = TF_NewBuffer();

    TF_SessionOptions *session_options = TF_NewSessionOptions();
    TF_Status *status = TF_NewStatus();
    char const *tags[] = {"serve"};
    session =
        TF_LoadSessionFromSavedModel(session_options, /*run_options=*/nullptr, model_path.c_str(),
                                     tags, /*tags_len=*/1, graph, graph_def, status);
    assert(TF_GetCode(status) == TF_OK);
    TF_DeleteStatus(status);
    TF_DeleteSessionOptions(session_options);
}

GomokuTfZeroPriorEvaluator::TfModelBasedEvaluatorInternal::~TfModelBasedEvaluatorInternal() {
    TF_DeleteTensor(board_input_value);
    TF_DeleteTensor(game_phase_input_value);
    TF_DeleteTensor(next_move_stone_type_input_value);

    TF_DeleteGraph(graph);
    TF_DeleteBuffer(graph_def);

    TF_Status *status = TF_NewStatus();
    TF_DeleteSession(session, status);
    assert(TF_GetCode(status) == TF_OK);
    TF_DeleteStatus(status);
}

EvaluationResult
GomokuTfZeroPriorEvaluator::TfModelBasedEvaluatorInternal::Fetch(MctNodeId const state_id,
                                                                  GomokuBoardState const &state) {
    auto it = cache.find(state_id);
    if (it != cache.end()) {
        return it->second;
    }

    if (board_input_value == nullptr) {
        // Allocate tensors when it's the first time running inference.
        int64_t board_dims[] = {1, state.Height(), state.Width()};
        board_input_value =
            TF_AllocateTensor(TF_UINT8, board_dims,
                              /*num_dims=*/sizeof(board_dims) / sizeof(int64_t),
                              /*len=*/board_dims[0] * board_dims[1] * board_dims[2]);

        int64_t game_phase_dims[] = {1};
        game_phase_input_value =
            TF_AllocateTensor(TF_UINT8, game_phase_dims,
                              /*num_dims=*/sizeof(game_phase_dims) / sizeof(int64_t),
                              /*len=*/game_phase_dims[0]);

        int64_t next_move_stone_type_dims[] = {1};
        next_move_stone_type_input_value =
            TF_AllocateTensor(TF_UINT8, next_move_stone_type_dims,
                              /*num_dims=*/sizeof(next_move_stone_type_dims) / sizeof(int64_t),
                              /*len=*/next_move_stone_type_dims[0]);
    }

    WriteBoard(state, board_input_value);
    WriteGamePhase(state, game_phase_input_value);
    WriteNextMoveStoneType(state, next_move_stone_type_input_value);

    TF_Operation *board_input_op = TF_GraphOperationByName(graph, /*oper_name=*/"inference_boards");
    assert(board_input_op != nullptr);
    TF_Output board_input = TF_Output{board_input_op, 0};

    TF_Operation *game_phase_input_op =
        TF_GraphOperationByName(graph, /*oper_name=*/"inference_game_phases");
    assert(game_phase_input_op != nullptr);
    TF_Output game_phase_input = TF_Output{game_phase_input_op, 0};

    TF_Operation *next_move_stone_type_input_op =
        TF_GraphOperationByName(graph, /*oper_name=*/"inference_next_move_stone_types");
    assert(next_move_stone_type_input_op != nullptr);
    TF_Output next_move_stone_type_input = TF_Output{next_move_stone_type_input_op, 0};

    TF_Output inputs[] = {board_input, game_phase_input, next_move_stone_type_input};
    TF_Tensor *input_values[] = {board_input_value, game_phase_input_value,
                                 next_move_stone_type_input_value};

    TF_Operation *output_op =
        TF_GraphOperationByName(graph, /*oper_name=*/"StatefulPartitionedCall");
    TF_Output policy_output = TF_Output{output_op, 0};
    TF_Output value_output = TF_Output{output_op, 1};

    TF_Output outputs[] = {policy_output, value_output};
    TF_Tensor *output_values[2];

    TF_Status *status = TF_NewStatus();
    TF_SessionRun(session, nullptr, inputs, input_values,
                  /*ninputs=*/sizeof(input_values) / sizeof(TF_Tensor *), outputs, output_values,
                  /*noutputs=*/sizeof(outputs) / sizeof(TF_Output),
                  /*target_opers=*/nullptr, /*ntargets=*/0,
                  /*metadata=*/nullptr, status);
    assert(TF_GetCode(status) == TF_OK);
    TF_DeleteStatus(status);

    EvaluationResult evaluation = ToEvaluationResult(state, /*policy_tensor=*/output_values[0],
                                                     /*policy_tensor=*/output_values[1]);
    cache.insert(std::make_pair(state_id, evaluation));

    TF_DeleteTensor(output_values[0]);
    TF_DeleteTensor(output_values[1]);

    return evaluation;
}

GomokuTfZeroPriorEvaluator::GomokuTfZeroPriorEvaluator(std::string const &model_path)
    : pimpl_(std::make_unique<TfModelBasedEvaluatorInternal>(model_path)) {}

GomokuTfZeroPriorEvaluator::~GomokuTfZeroPriorEvaluator() {}

float GomokuTfZeroPriorEvaluator::EvaluateReward(GomokuBoardState const &state,
                                                  MctNodeId const state_id) {
    return pimpl_->Fetch(state_id, state).reward;
}

std::unordered_map<GomokuActionId, float>
GomokuTfZeroPriorEvaluator::EvaluatePolicy(GomokuBoardState const &state,
                                            MctNodeId const state_id) {
    return pimpl_->Fetch(state_id, state).policy;
}

float GomokuTfZeroPriorEvaluator::ExplorationFactor() const { return 4.0f; }

unsigned GomokuTfZeroPriorEvaluator::NumSimulations() const { return 2048; }

void GomokuTfZeroPriorEvaluator::ClearCache() { pimpl_->cache.clear(); }

} // namespace e8
