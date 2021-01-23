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

#include <queue>

#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"

namespace e8 {

/**
 * @brief The MockPlayer class A player that returns a actions from a fixed sequence.
 */
class MockPlayer : public GomokuPlayerInterface {
  public:
    MockPlayer(std::queue<GomokuActionId> const &action_sequence);

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
};

} // namespace e8

#endif // MOCK_PLAYER_H
