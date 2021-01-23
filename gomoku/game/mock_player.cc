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

#include <chrono>
#include <memory>
#include <queue>
#include <thread>

#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"
#include "gomoku/game/mock_player.h"

namespace e8 {

MockPlayer::MockPlayer(std::queue<GomokuActionId> const &action_sequence,
                       std::chrono::milliseconds const wait_before_ended)
    : action_sequence_(action_sequence), wait_before_ended_(wait_before_ended) {}

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

bool MockPlayer::WantAnotherGame() {
    std::this_thread::sleep_for(wait_before_ended_);
    return false;
}

std::shared_ptr<MockPlayer> DefaultMockPlayerA() {
    GomokuBoardState board(/*width=*/11, /*height=*/11);

    std::queue<GomokuActionId> actions;
    actions.push(board.MovePositionToActionId(MovePosition(/*x=*/5, /*y=*/5)));
    actions.push(board.MovePositionToActionId(MovePosition(/*x=*/5, /*y=*/6)));
    actions.push(board.MovePositionToActionId(MovePosition(/*x=*/5, /*y=*/4)));
    actions.push(board.MovePositionToActionId(MovePosition(/*x=*/5, /*y=*/7)));
    actions.push(board.MovePositionToActionId(MovePosition(/*x=*/5, /*y=*/8)));
    actions.push(board.MovePositionToActionId(MovePosition(/*x=*/5, /*y=*/9)));

    return std::make_shared<MockPlayer>(actions, std::chrono::milliseconds(500));
}

std::shared_ptr<MockPlayer> DefaultMockPlayerB() {
    GomokuBoardState board(/*width=*/11, /*height=*/11);

    std::queue<GomokuActionId> actions;
    actions.push(board.Swap2DecisionToActionId(Swap2Decision::SW2D_CHOOSE_WHITE));
    actions.push(board.MovePositionToActionId(MovePosition(/*x=*/6, /*y=*/4)));
    actions.push(board.MovePositionToActionId(MovePosition(/*x=*/7, /*y=*/4)));
    actions.push(board.MovePositionToActionId(MovePosition(/*x=*/8, /*y=*/4)));

    return std::make_shared<MockPlayer>(actions, std::chrono::milliseconds(500));
}

} // namespace e8
