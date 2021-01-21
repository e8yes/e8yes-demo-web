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

#ifndef GAME_LOG_STORE_H
#define GAME_LOG_STORE_H

#include <cstdint>
#include <optional>
#include <unordered_map>

#include "gomoku/game/board_state.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

enum GameLogPurpose {
    GLP_REPRESENTATIVE_DATA,
    GLP_TRAINING_DATA,
    GLP_VALIDATION_DATA,
    GLP_ACTUAL_RUN,
};

using ModelId = int64_t;

using GameId = int64_t;
using GameStepNumber = unsigned;

/**
 * @brief The GameLogStore class Handles game data logging. The logs will be written to the
 * database.
 */
class GameLogStore {
  public:
    /**
     * @brief GameLogStore All the logs will be written to the specified connection reservoir
     * target.
     */
    GameLogStore(ConnectionReservoirInterface *conns);

    /**
     * @brief LogNewGeneratorGame Creates a new game entry and returns an ID pointing to the entry.
     */
    GameId LogNewGeneratorGame(GameLogPurpose game_purpose,
                               std::optional<ModelId> player_a_model_id,
                               std::optional<ModelId> player_b_model_id);

    /**
     * @brief LogGameAction A game action at the specified step_number can only be logged once.
     * Action value, otherwise, can be amended later on.
     */
    void LogGameAction(GameId game_id, GameStepNumber step_number, GomokuActionId action_number,
                       PlayerSide action_performed_by_player, StoneType action_stone_type,
                       GomokuBoardState const &board_before, GamePhase game_phase,
                       std::vector<float> const &stochastic_policy);

    /**
     * @brief LogActionStepValue Amend the action value for the specified action step in the game.
     */
    void LogGameActionValue(GameId game_id, GameStepNumber step_number, float final_value);

    /**
     * @brief LogGameEnd Amend the game result to the specified game.
     */
    void LogGameEnd(GameId game_id, GameResult game_result);

  private:
    ConnectionReservoirInterface *const conns_;
};

} // namespace e8

#endif // GAME_LOG_STORE_H
