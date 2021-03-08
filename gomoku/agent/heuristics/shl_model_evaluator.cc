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
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/heuristics/shl_feature.h"
#include "gomoku/agent/heuristics/shl_model_evaluator.h"
#include "gomoku/agent/heuristics/shl_rollout_evaluator.h"
#include "gomoku/agent/heuristics/tf_zero_prior_evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/game/board_state.h"
#include "third_party/tensorflow/c/c_api.h"
#include "third_party/tensorflow/c/tf_datatype.h"
#include "third_party/tensorflow/c/tf_status.h"
#include "third_party/tensorflow/c/tf_tensor.h"

namespace e8 {
namespace {

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

void WriteShlMap(GomokuBoardState const &state, std::vector<float> const &shl_map,
                 TF_Tensor *shl_map_tensor) {
    assert(TF_NumDims(shl_map_tensor) == 4);
    assert(TF_Dim(shl_map_tensor, /*dim_index=*/0) == 1);
    assert(TF_Dim(shl_map_tensor, /*dim_index=*/1) == state.Width());
    assert(TF_Dim(shl_map_tensor, /*dim_index=*/2) == state.Height());
    assert(TF_Dim(shl_map_tensor, /*dim_index=*/3) == 4);

    float *tensor_memory = static_cast<float *>(TF_TensorData(shl_map_tensor));

    for (int16_t y = 0; y < state.Height(); ++y) {
        for (int16_t x = 0; x < state.Width(); ++x) {
            for (int16_t i = 0; i < 4; ++i) {
                tensor_memory[i + (y + x * state.Height()) * 4] =
                    shl_map[i + (x + y * state.Width()) * 4];
            }
        }
    }
}

std::unordered_map<GomokuActionId, float> RenormalizePolicy(GomokuBoardState const &state,
                                                            TF_Tensor const *policy_tensor) {
    std::unordered_map<GomokuActionId, float> policy;

    assert(TF_NumDims(policy_tensor) == 2);
    assert(TF_Dim(policy_tensor, /*dim_index=*/0) == 1);
    auto [lo, hi] = state.ActionIdRange();
    assert(TF_Dim(policy_tensor, /*dim_index=*/1) == hi - lo + 1);

    // Re-normalizes and stores the policy into the map.
    float *policy_tensor_memory = static_cast<float *>(TF_TensorData(policy_tensor));

    float norm_factor = 0;
    for (auto const &[action_id, _] : state.LegalActions()) {
        norm_factor += policy_tensor_memory[action_id];
    }
    for (auto const &[action_id, _] : state.LegalActions()) {
        policy[action_id] = policy_tensor_memory[action_id] / norm_factor;
    }

    return policy;
}

} // namespace

struct GomokuShlModelEvaluator::GomokuShlModelEvaluatorInternal {
    GomokuShlModelEvaluatorInternal(std::string const &model_path);
    ~GomokuShlModelEvaluatorInternal();

    std::unordered_map<GomokuActionId, float> ModelBasedPolicy(GomokuBoardState const &state,
                                                               std::vector<float> const &shl_map);

    TF_Session *session = nullptr;
    TF_Graph *graph = nullptr;
    TF_Buffer *graph_def = nullptr;

    TF_Tensor *board_input_value = nullptr;
    TF_Tensor *game_phase_input_value = nullptr;
    TF_Tensor *next_move_stone_type_input_value = nullptr;
    TF_Tensor *shl_map_input_value = nullptr;

    GomokuShlRolloutEvaluator shl_rollout_evaluator;
};

GomokuShlModelEvaluator::GomokuShlModelEvaluatorInternal::GomokuShlModelEvaluatorInternal(
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

GomokuShlModelEvaluator::GomokuShlModelEvaluatorInternal::~GomokuShlModelEvaluatorInternal() {
    TF_DeleteTensor(board_input_value);
    TF_DeleteTensor(game_phase_input_value);
    TF_DeleteTensor(next_move_stone_type_input_value);
    TF_DeleteTensor(shl_map_input_value);

    TF_DeleteGraph(graph);
    TF_DeleteBuffer(graph_def);

    TF_Status *status = TF_NewStatus();
    TF_DeleteSession(session, status);
    assert(TF_GetCode(status) == TF_OK);
    TF_DeleteStatus(status);
}

std::unordered_map<GomokuActionId, float>
GomokuShlModelEvaluator::GomokuShlModelEvaluatorInternal::ModelBasedPolicy(
    GomokuBoardState const &state, std::vector<float> const &shl_map) {
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

        int64_t shl_map_dims[] = {1, state.Height(), state.Width(), 4};
        shl_map_input_value =
            TF_AllocateTensor(TF_FLOAT, shl_map_dims,
                              /*num_dims=*/sizeof(shl_map_dims) / sizeof(int64_t),
                              /*len=*/shl_map_dims[0] * shl_map_dims[1] * shl_map_dims[2] *
                                  shl_map_dims[3] * sizeof(float));
    }

    WriteBoard(state, board_input_value);
    WriteGamePhase(state, game_phase_input_value);
    WriteNextMoveStoneType(state, next_move_stone_type_input_value);
    WriteShlMap(state, shl_map, shl_map_input_value);

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

    TF_Operation *shl_map_input_op =
        TF_GraphOperationByName(graph, /*oper_name=*/"inference_shl_map");
    assert(shl_map_input_op != nullptr);
    TF_Output shl_map_input = TF_Output{shl_map_input_op, 0};

    TF_Output inputs[] = {board_input, game_phase_input, next_move_stone_type_input, shl_map_input};
    TF_Tensor *input_values[] = {board_input_value, game_phase_input_value,
                                 next_move_stone_type_input_value, shl_map_input_value};

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

    std::unordered_map<GomokuActionId, float> policy =
        RenormalizePolicy(state, /*policy_tensor=*/output_values[0]);

    TF_DeleteTensor(output_values[0]);
    TF_DeleteTensor(output_values[1]);

    return policy;
}

GomokuShlModelEvaluator::GomokuShlModelEvaluator(std::string const &model_path)
    : pimpl_(std::make_unique<GomokuShlModelEvaluatorInternal>(model_path)) {}

GomokuShlModelEvaluator::~GomokuShlModelEvaluator() {}

float GomokuShlModelEvaluator::EvaluateReward(GomokuBoardState const &state,
                                              std::optional<MctNodeId> parent_state_id,
                                              MctNodeId state_id) {
    return pimpl_->shl_rollout_evaluator.EvaluateReward(state, parent_state_id, state_id);
}

std::unordered_map<GomokuActionId, float> GomokuShlModelEvaluator::EvaluatePolicy(
    GomokuBoardState const &state, std::optional<MctNodeId> parent_state_id, MctNodeId state_id) {
    ShlFeatureBuilder const &feature_builder =
        pimpl_->shl_rollout_evaluator.GetFeatureBuilderForState(state, parent_state_id, state_id);
    std::vector<float> shl_map =
        feature_builder.TopKMapDense(/*top_k=*/15, /*normalized=*/true,
                                     /*next_move_stone_type=*/std::nullopt);

    return pimpl_->ModelBasedPolicy(state, shl_map);
}

float GomokuShlModelEvaluator::ExplorationFactor() const { return 3.0f; }

unsigned GomokuShlModelEvaluator::NumSimulations() const { return 1500; }

void GomokuShlModelEvaluator::ClearCache() { pimpl_->shl_rollout_evaluator.ClearCache(); }

} // namespace e8
