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
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "gomoku/agent/heuristics/light_rollout_evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/agent/search/mct_search.h"
#include "gomoku/agent_classroom/learning_material_generator.h"
#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"

namespace e8 {

LearningMaterialGeneratorSharedData::LearningMaterialGeneratorSharedData(
    GameLogPurpose game_purpose, std::optional<ModelId> model_id, unsigned target_num_games,
    std::unique_ptr<MctSearcher> &&searcher, GameLogStore *log_store)
    : game_purpose(game_purpose), model_id(model_id), target_num_games(target_num_games),
      searcher(std::move(searcher)), log_store(log_store), current_num_games(0) {}

LearningMaterialGeneratorSharedData::StepInfo::StepInfo(GameStepNumber step_number,
                                                        PlayerSide action_performer)
    : step_number(step_number), action_performer(action_performer) {}

LearningMaterialGenerator::LearningMaterialGenerator(
    std::shared_ptr<LearningMaterialGeneratorSharedData> const &shared_data,
    PlayerSide const player_side)
    : shared_data_(shared_data), player_side_(player_side), random_source_(/*seed=*/1031) {}

unsigned LearningMaterialGenerator::NumGamesProduced() const {
    return shared_data_->current_num_games;
}

void LearningMaterialGenerator::OnGomokuGameBegin(GomokuBoardState const & /*board_state*/) {
    shared_data_->searcher->Reset();

    if (!shared_data_->current_game_id.has_value()) {
        shared_data_->current_game_id = shared_data_->log_store->LogNewGeneratorGame(
            shared_data_->game_purpose, shared_data_->model_id, shared_data_->model_id);
    }
}

GomokuActionId LearningMaterialGenerator::NextPlayerAction(GomokuBoardState const &board_state) {
    std::unordered_map<GomokuActionId, float> policy =
        shared_data_->searcher->SearchFrom(board_state, /*temperature=*/1.0f);
    GomokuActionId action_id = SampleAction(policy, &random_source_);

    auto flattened_policy = FlattenPolicy(policy, board_state);
    GameStepNumber step_number = board_state.History().size();
    shared_data_->log_store->LogGameAction(
        *shared_data_->current_game_id, step_number, action_id, board_state.CurrentPlayerSide(),
        board_state.PlayerStoneType(board_state.CurrentPlayerSide()), board_state,
        board_state.CurrentGamePhase(), flattened_policy);

    shared_data_->steps.push_back(LearningMaterialGeneratorSharedData::StepInfo(
        step_number, board_state.CurrentPlayerSide()));

    return action_id;
}

void LearningMaterialGenerator::BeforeGomokuActionApplied(
    GomokuBoardState const &board_state, PlayerSide const action_performed_by,
    GomokuActionId const &incoming_action_id) {
    if (player_side_ == action_performed_by) {
        shared_data_->searcher->SelectAction(board_state, incoming_action_id);
    }
}

void LearningMaterialGenerator::AfterGomokuActionApplied(GomokuBoardState const & /*board_state*/) {
}

void LearningMaterialGenerator::OnGameEnded(GomokuBoardState const &board_state) {
    if (!shared_data_->current_game_id.has_value()) {
        return;
    }

    for (auto const &step : shared_data_->steps) {
        switch (board_state.CurrentGameResult()) {
        case GR_PLAYER_A_WIN: {
            if (step.action_performer == PlayerSide::PS_PLAYER_A) {
                shared_data_->log_store->LogGameActionValue(*shared_data_->current_game_id,
                                                            step.step_number,
                                                            /*final_value=*/1.0f);
            } else {
                shared_data_->log_store->LogGameActionValue(*shared_data_->current_game_id,
                                                            step.step_number,
                                                            /*final_value=*/-1.0f);
            }
            break;
        }
        case GR_PLAYER_B_WIN: {
            if (step.action_performer == PlayerSide::PS_PLAYER_B) {
                shared_data_->log_store->LogGameActionValue(*shared_data_->current_game_id,
                                                            step.step_number,
                                                            /*final_value=*/1.0f);
            } else {
                shared_data_->log_store->LogGameActionValue(*shared_data_->current_game_id,
                                                            step.step_number,
                                                            /*final_value=*/-1.0f);
            }
            break;
        }
        case GR_TIE: {
            shared_data_->log_store->LogGameActionValue(*shared_data_->current_game_id,
                                                        step.step_number,
                                                        /*final_value=*/0.0f);
            break;
        }
        case GR_UNDETERMINED: {
            assert(false);
            break;
        }
        }
    }

    shared_data_->log_store->LogGameEnd(*shared_data_->current_game_id,
                                        board_state.CurrentGameResult());

    shared_data_->current_game_id = std::nullopt;
    ++shared_data_->current_num_games;
    shared_data_->steps.clear();
}

bool LearningMaterialGenerator::WantAnotherGame() {
    return shared_data_->current_num_games < shared_data_->target_num_games;
}

} // namespace e8
