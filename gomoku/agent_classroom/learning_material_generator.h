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

#ifndef LEARNING_MATERIAL_GENERATOR_H
#define LEARNING_MATERIAL_GENERATOR_H

#include <list>
#include <memory>
#include <optional>

#include "common/container/mutable_priority_queue.h"
#include "common/random/random_source.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/agent/search/mct_search.h"
#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"
#include "gomoku/logging/game_log_store.h"

namespace e8 {

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

} // namespace e8

#endif // LEARNING_MATERIAL_GENERATOR_H
