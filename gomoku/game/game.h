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

#ifndef GAME_H
#define GAME_H

#include <memory>

#include "gomoku/game/board_state.h"

namespace e8 {

/**
 * @brief The GomokuPlayerInterface class Represent an abstract Gomoku player.
 */
class GomokuPlayerInterface {
  public:
    GomokuPlayerInterface() = default;
    virtual ~GomokuPlayerInterface() = default;

    /**
     * @brief NextPlayerAction Obtain the next action from a player given the current board state.
     */
    virtual GomokuActionId NextPlayerAction(GomokuBoardState const &board_state) = 0;

    /**
     * @brief OnGomokuGameBegin Player will be notified when a new game begins.
     */
    virtual void OnGomokuGameBegin(GomokuBoardState const &board_state) = 0;

    /**
     * @brief BeforeGomokuActionApplied Player will be notified before a new action is going to be
     * applied to the board.
     */
    virtual void BeforeGomokuActionApplied(GomokuBoardState const &board_state,
                                           PlayerSide const action_performed_by,
                                           GomokuActionId const &incoming_action_id) = 0;

    /**
     * @brief AfterGomokuActionApplied Player will be notified when a new action is applied to the
     * board.
     */
    virtual void AfterGomokuActionApplied(GomokuBoardState const &board_state) = 0;

    /**
     * @brief OnGameEnded Player will be notified when the current game has ended with the given
     * board state.
     */
    virtual void OnGameEnded(GomokuBoardState const &board_state) = 0;

    /**
     * @brief WantAnotherGame Player will be asked if one wants another Gomoku game.
     */
    virtual bool WantAnotherGame() = 0;
};

/**
 * @brief The GomokuGame class The main class that lets players take turn and engage in Gomoku
 * games.
 */
class GomokuGame {
  public:
    /**
     * @brief GomokuGame Constructs a Gomoku game with the two specified players.
     */
    GomokuGame(std::shared_ptr<GomokuPlayerInterface> const &player_a,
               std::shared_ptr<GomokuPlayerInterface> const &player_b);
    GomokuGame(GomokuGame const &) = delete;
    GomokuGame(GomokuGame &&) = delete;
    ~GomokuGame() = default;

    /**
     * @brief Start Runs the game's main loop.
     */
    void Start();

  private:
    unsigned const kWidth = 11;
    unsigned const kHeight = 11;

    std::shared_ptr<GomokuPlayerInterface> player_a_;
    std::shared_ptr<GomokuPlayerInterface> player_b_;
};

} // namespace e8

#endif // GAME_H
