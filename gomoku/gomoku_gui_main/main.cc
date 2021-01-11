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

#include "gomoku/gomoku_agent/heuristics/light_rollout_evaluator.h"
#include "gomoku/gomoku_agent/mcts_agent_player.h"
#include "gomoku/gomoku_game/board_state.h"
#include "gomoku/gomoku_game/game.h"
#include "gomoku/gomoku_gui_main/human_gui_player.h"
#include "gomoku/gomoku_gui_main/main_window.h"

void RunGame(/*e8::MainWindow *player_a_window,*/ e8::MainWindow *player_b_window) {
    std::shared_ptr<e8::GomokuPlayerInterface> player_a =
        std::static_pointer_cast<e8::GomokuPlayerInterface>(std::make_shared<e8::MctsAgentPlayer>(
            std::make_shared<e8::GomokuLightRolloutEvaluator>()));

    std::shared_ptr<e8::GomokuPlayerInterface> player_b =
        std::static_pointer_cast<e8::GomokuPlayerInterface>(
            std::make_shared<e8::HumanGuiPlayer>(player_b_window, e8::PlayerSide::PS_PLAYER_B));

    e8::GomokuGame game(player_a, player_b);

    game.Start();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // e8::MainWindow player_a_window;
    e8::MainWindow player_b_window;

    std::thread game_thread(RunGame, /*player_a_window,*/ &player_b_window);

    // player_a_window.show();
    player_b_window.show();
    int exit_code = app.exec();

    game_thread.join();

    return exit_code;
}
