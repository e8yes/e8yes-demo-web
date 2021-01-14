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

#ifndef HUMAN_GUI_PLAYER_H
#define HUMAN_GUI_PLAYER_H

#include "gomoku/game/game.h"
#include "gomoku/gui_main/main_window.h"

namespace e8 {

/**
 * @brief The HumanGuiPlayer class A human Gomoku player who engages into the game through a Qt GUI.
 * It's assumed that the game is run in a different thread from the GUI's.
 */
class HumanGuiPlayer : public GomokuPlayerInterface {
  public:
    HumanGuiPlayer(MainWindow *main_window, PlayerSide const player_side);
    ~HumanGuiPlayer() = default;

    GomokuActionId NextPlayerAction(GomokuBoardState const &board_state) override;

    void OnGomokuGameBegin(GomokuBoardState const &board_state) override;

    void AfterGomokuActionApplied(GomokuBoardState const &board_state) override;

    void OnGameEnded(GomokuBoardState const &board_state) override;

    bool WantAnotherGame() override;

  private:
    MainWindow *const main_window_;
    PlayerSide const player_side_;
};

} // namespace e8

#endif // HUMAN_GUI_PLAYER_H
