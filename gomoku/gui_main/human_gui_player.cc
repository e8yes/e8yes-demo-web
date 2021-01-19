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

#include <string>

#include "gomoku/game/game.h"
#include "gomoku/gui_main/human_gui_player.h"
#include "gomoku/gui_main/main_window.h"

namespace e8 {

HumanGuiPlayer::HumanGuiPlayer(MainWindow *main_window, PlayerSide const player_side)
    : main_window_(main_window), player_side_(player_side) {}

GomokuActionId HumanGuiPlayer::NextPlayerAction(GomokuBoardState const &board_state) {
    main_window_->ScheduleEnableActionInput(board_state);
    GomokuActionId action_id = main_window_->WaitForNextAction();
    main_window_->ScheduleDisableActionActionInput(board_state);

    return action_id;
}

void HumanGuiPlayer::OnGomokuGameBegin(GomokuBoardState const &board_state) {
    switch (player_side_) {
    case PS_PLAYER_A: {
        main_window_->ScheduleInit(board_state, "Player A");
        break;
    }
    case PS_PLAYER_B: {
        main_window_->ScheduleInit(board_state, "Player B");
        break;
    }
    }

    main_window_->ScheduleDisableActionActionInput(board_state);
}

void HumanGuiPlayer::BeforeGomokuActionApplied(GomokuBoardState const & /*board_state*/,
                                               PlayerSide const /*action_performed_by*/,
                                               GomokuActionId const & /*incoming_action_id*/) {}

void HumanGuiPlayer::AfterGomokuActionApplied(GomokuBoardState const &board_state) {
    main_window_->ScheduleDisableActionActionInput(board_state);
    main_window_->ScheduleUpdate(board_state);
}

void HumanGuiPlayer::OnGameEnded(GomokuBoardState const &board_state) {
    main_window_->ScheduleGameEnding(board_state);
}

bool HumanGuiPlayer::WantAnotherGame() { return false; }

} // namespace e8
