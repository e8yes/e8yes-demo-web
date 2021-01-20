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
#include <thread>

#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/mcts_agent_player.h"
#include "gomoku/game/board_state.h"
#include "gomoku/gui_main/agent_gui_player.h"
#include "gomoku/gui_main/main_window.h"

namespace e8 {

AgentGuiPlayer::AgentGuiPlayer(MainWindow *main_window, PlayerSide const player_side,
                               std::shared_ptr<MctSearcher> const &searcher)
    : MctsAgentPlayer(player_side, searcher), main_window_(main_window), player_side_(player_side) {
}

void AgentGuiPlayer::OnGomokuGameBegin(GomokuBoardState const &board_state) {
    MctsAgentPlayer::OnGomokuGameBegin(board_state);

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

void AgentGuiPlayer::AfterGomokuActionApplied(GomokuBoardState const &board_state) {
    MctsAgentPlayer::AfterGomokuActionApplied(board_state);

    main_window_->ScheduleDisableActionActionInput(board_state);
    main_window_->ScheduleUpdate(board_state);
}

void AgentGuiPlayer::OnGameEnded(GomokuBoardState const &board_state) {
    MctsAgentPlayer::OnGameEnded(board_state);

    main_window_->ScheduleGameEnding(board_state);

    std::this_thread::sleep_for(std::chrono::seconds(2));
}

bool AgentGuiPlayer::WantAnotherGame() { return true; }

} // namespace e8
