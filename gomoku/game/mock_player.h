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

#ifndef MOCK_PLAYER_H
#define MOCK_PLAYER_H

#include <chrono>
#include <cstdint>
#include <memory>
#include <queue>

#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"

namespace e8 {

/**
 * @brief The MockPlayer class A player that returns a actions from a fixed sequence.
 */
class MockPlayer : public GomokuPlayerInterface {
  public:
    /**
     * @brief MockPlayer Mock a Gomoku player by playing the specified action_sequence then end the
     * game after waiting for wait_before_ended milliseconds.
     */
    MockPlayer(std::queue<GomokuActionId> const &action_sequence,
               std::chrono::milliseconds const wait_before_ended);

    GomokuActionId NextPlayerAction(GomokuBoardState const &board_state);

    void OnGomokuGameBegin(GomokuBoardState const &board_state);

    void BeforeGomokuActionApplied(GomokuBoardState const &board_state,
                                   PlayerSide const action_performed_by,
                                   GomokuActionId const &incoming_action_id);

    void AfterGomokuActionApplied(GomokuBoardState const &board_state);

    void OnGameEnded(GomokuBoardState const &board_state);

    bool WantAnotherGame();

  private:
    std::queue<GomokuActionId> action_sequence_;
    std::chrono::milliseconds wait_before_ended_;
};

/**
 * @brief DefaultMockPlayerA/B Default player that proceduces a fix action sequence in a 11x11 board
 * leading to a final board state:
 *
 *    0 1 2 3 4 5 6 7 8 9 10
 * 0  - - - - - - - - - - -
 * 1  - - - - - - - - - - -
 * 2  - - - - - - - - - - -
 * 3  - - - - - - - - - - -
 * 4  - - - - - o o o o - -
 * 5  - - - - - x - - - - -
 * 6  - - - - - x - - - - -
 * 7  - - - - - x - - - - -
 * 8  - - - - - x - - - - -
 * 9  - - - - - x - - - - -
 * 10 - - - - - - - - - - -
 *
 * Player A will win the game. And both players will wait for half a second before ending the game.
 */
std::shared_ptr<MockPlayer> DefaultMockPlayerA();
std::shared_ptr<MockPlayer> DefaultMockPlayerB();

} // namespace e8

#endif // MOCK_PLAYER_H
