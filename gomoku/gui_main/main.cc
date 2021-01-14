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

#include <QApplication>
#include <memory>
#include <thread>

#include "gomoku/agent/heuristics/light_rollout_evaluator.h"
#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"
#include "gomoku/gui_main/agent_gui_player.h"
#include "gomoku/gui_main/human_gui_player.h"
#include "gomoku/gui_main/main_window.h"

void RunGame(e8::MainWindow *player_a_window, e8::MainWindow *player_b_window) {
    std::shared_ptr<e8::GomokuPlayerInterface> player_a =
        std::static_pointer_cast<e8::GomokuPlayerInterface>(std::make_shared<e8::AgentGuiPlayer>(
            player_a_window, e8::PlayerSide::PS_PLAYER_A,
            std::make_shared<e8::GomokuLightRolloutEvaluator>()));

    //    std::shared_ptr<e8::GomokuPlayerInterface> player_b =
    //        std::static_pointer_cast<e8::GomokuPlayerInterface>(
    //            std::make_shared<e8::HumanGuiPlayer>(player_b_window,
    //            e8::PlayerSide::PS_PLAYER_B));

    std::shared_ptr<e8::GomokuPlayerInterface> player_b =
        std::static_pointer_cast<e8::GomokuPlayerInterface>(std::make_shared<e8::AgentGuiPlayer>(
            player_b_window, e8::PlayerSide::PS_PLAYER_B,
            std::make_shared<e8::GomokuLightRolloutEvaluator>()));

    e8::GomokuGame game(player_a, player_b);

    game.Start();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    e8::MainWindow player_a_window;
    e8::MainWindow player_b_window;

    std::thread game_thread(RunGame, &player_a_window, &player_b_window);

    player_a_window.show();
    player_b_window.show();
    int exit_code = app.exec();

    game_thread.join();

    return exit_code;
}
