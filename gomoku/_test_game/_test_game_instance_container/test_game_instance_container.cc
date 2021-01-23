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

#include "common/unit_test_util/unit_test_util.h"
#include "gomoku/game/game.h"
#include "gomoku/game/game_instance_container.h"
#include "gomoku/game/mock_player.h"

bool ScheduleAndFetchGameTest() {
    auto game =
        std::make_unique<e8::GomokuGame>(e8::DefaultMockPlayerA(), e8::DefaultMockPlayerB());

    e8::GameInstanceContainer *container = e8::DefaultGameInstanceContainer();
    e8::GameInstanceContainer::ScheduleId schedule_id = container->ScheduleToRun(std::move(game));

    std::shared_ptr<e8::GomokuGame> fetched_game = container->ScheduledGame(schedule_id);
    TEST_CONDITION(fetched_game != nullptr);

    std::this_thread::sleep_for(std::chrono::milliseconds(1100));

    fetched_game = container->ScheduledGame(schedule_id);
    TEST_CONDITION(fetched_game == nullptr);

    return true;
}

int main() {
    e8::BeginTestSuite("game_instance_container");
    e8::RunTest("ScheduleAndFetchGameTest", ScheduleAndFetchGameTest);
    e8::EndTestSuite();
    return 0;
}
