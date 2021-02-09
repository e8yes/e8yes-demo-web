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
#include <chrono>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "gomoku/agent/heuristics/light_rollout_evaluator.h"
#include "gomoku/agent/heuristics/model_based_evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/agent/search/mct_search.h"
#include "gomoku/agent_classroom/learning_material_generator.h"
#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"
#include "gomoku/game/game_instance_container.h"
#include "gomoku/logging/common_types.h"
#include "gomoku/logging/game_log_store.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/pooled_connection_reservoir.h"

namespace e8 {
namespace {

/**
 * @brief The LearningMaterialGeneratorSharedData struct Data shared between the two
 * LearningMaterialGenerator players so that only one copy of data is required to be maintained.
 * Both LearningMaterialGenerator players will share the Monte Carlo Searcher states during data
 * generation.
 */
struct LearningMaterialGeneratorSharedData {
    LearningMaterialGeneratorSharedData(GameLogPurpose game_purpose,
                                        std::optional<ModelId> model_id, unsigned target_num_games,
                                        std::unique_ptr<MctSearcher> &&searcher,
                                        GameLogStore *log_store);

    struct StepInfo {
        StepInfo(GameStepNumber step_number, PlayerSide action_performer);

        GameStepNumber const step_number;
        PlayerSide const action_performer;
    };

    GameLogPurpose const game_purpose;
    std::optional<ModelId> const model_id;
    unsigned const target_num_games;
    std::unique_ptr<MctSearcher> searcher;
    GameLogStore *const log_store;

    unsigned current_num_games;
    std::optional<GameId> current_game_id;
    std::list<StepInfo> steps;
};

/**
 * @brief The LearningMaterialGenerator class A Gomoku game player that produces example game and
 * action data by self playing then store them into the specified log store.
 */
class LearningMaterialGenerator : public GomokuPlayerInterface {
  public:
    /**
     * @brief LearningMaterialGenerator Constructs a data generator player to play and log a
     * specified number games.
     *
     * @param target_num_games
     */
    LearningMaterialGenerator(
        std::shared_ptr<LearningMaterialGeneratorSharedData> const &shared_data,
        PlayerSide const player_side);
    ~LearningMaterialGenerator() override = default;

    /**
     * @brief NumGamesProduced Returns the number of games that has been logged into the game log
     * store.
     */
    unsigned NumGamesProduced() const;

    GomokuActionId NextPlayerAction(GomokuBoardState const &board_state) override;

    void OnGomokuGameBegin(GomokuBoardState const &board_state) override;

    void BeforeGomokuActionApplied(GomokuBoardState const &board_state,
                                   PlayerSide const action_performed_by,
                                   GomokuActionId const &incoming_action_id) override;

    void AfterGomokuActionApplied(GomokuBoardState const &board_state) override;

    void OnGameEnded(GomokuBoardState const &board_state) override;

    bool WantAnotherGame() override;

  private:
    std::shared_ptr<LearningMaterialGeneratorSharedData> shared_data_;
    PlayerSide const player_side_;
    RandomSource random_source_;
};

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
    : shared_data_(shared_data), player_side_(player_side), random_source_() {}

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

    // Selects the action based on the policy distribution in the first 10 moves so as to explore
    // the state space. It will then always pick the best action after the first 10 moves.
    GomokuActionId action_id;
    if (board_state.History().size() < 10) {
        action_id = SampleAction(policy, &random_source_);
    } else {
        action_id = BestAction(policy);
    }

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

} // namespace

void GenerateLearningMaterial(GameLogPurpose log_purpose, std::optional<ModelId> model_id,
                              std::shared_ptr<GomokuEvaluatorInterface> const &evaluator,
                              GameInstanceContainer::ScheduleId schedule_id,
                              unsigned target_num_games, std::string const &db_host_name,
                              std::string const &db_name, GameInstanceContainer *container) {
    PooledConnectionReservoir conns(
        ConnectionFactory(ConnectionFactory::PQ, db_host_name, db_name));

    GameLogStore log_store(&conns);

    auto searcher = std::make_unique<MctSearcher>(evaluator, /*print_stats=*/true);
    auto generator_data = std::make_shared<LearningMaterialGeneratorSharedData>(
        log_purpose, model_id, target_num_games, std::move(searcher), &log_store);

    auto generator_game = std::make_unique<GomokuGame>(
        std::make_shared<LearningMaterialGenerator>(generator_data, PlayerSide::PS_PLAYER_A),
        std::make_shared<LearningMaterialGenerator>(generator_data, PlayerSide::PS_PLAYER_B));
    container->ScheduleToRun(schedule_id, std::move(generator_game));

    while (container->ScheduledGame(schedule_id) != nullptr) {
        // Wait for the generator to hit the target number games.
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

} // namespace e8
