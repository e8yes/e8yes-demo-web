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

#ifndef AGENT_GUI_PLAYER_H
#define AGENT_GUI_PLAYER_H

#include <memory>

#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/mcts_agent_player.h"
#include "gomoku/game/board_state.h"
#include "gomoku/gui_main/main_window.h"

namespace e8 {

/**
 * @brief The AgentGuiPlayer class An artificial intelligent Gomoku agent who engages into the game
 * with presentation through a Qt GUI. It's assumed that the game is run in a different thread from
 * the GUI's.
 */
class AgentGuiPlayer : public MctsAgentPlayer {
  public:
    AgentGuiPlayer(MainWindow *main_window, PlayerSide const player_side,
                   std::shared_ptr<GomokuEvaluatorInterface> const &evaluator);
    ~AgentGuiPlayer() override = default;

    void OnGomokuGameBegin(GomokuBoardState const &board_state) override;

    void AfterGomokuActionApplied(GomokuBoardState const &board_state) override;

    void OnGameEnded(GomokuBoardState const &board_state) override;

    bool WantAnotherGame() override;

  private:
    MainWindow *const main_window_;
    PlayerSide const player_side_;
};

} // namespace e8

#endif // AGENT_GUI_PLAYER_H
