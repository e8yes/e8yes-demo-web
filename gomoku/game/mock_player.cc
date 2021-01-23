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

#include <queue>

#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"
#include "gomoku/game/mock_player.h"

namespace e8 {

MockPlayer::MockPlayer(std::queue<GomokuActionId> const &action_sequence)
    : action_sequence_(action_sequence) {}

GomokuActionId MockPlayer::NextPlayerAction(GomokuBoardState const & /*board_state*/) {
    GomokuActionId action_id = action_sequence_.front();
    action_sequence_.pop();
    return action_id;
}

void MockPlayer::OnGomokuGameBegin(GomokuBoardState const & /*board_state*/) {}

void MockPlayer::BeforeGomokuActionApplied(GomokuBoardState const & /*board_state*/,
                                           PlayerSide const /*action_performed_by*/,
                                           GomokuActionId const & /*incoming_action_id*/) {}

void MockPlayer::AfterGomokuActionApplied(GomokuBoardState const & /*board_state*/) {}

void MockPlayer::OnGameEnded(GomokuBoardState const & /*board_state*/) {}

bool MockPlayer::WantAnotherGame() { return false; }

} // namespace e8
