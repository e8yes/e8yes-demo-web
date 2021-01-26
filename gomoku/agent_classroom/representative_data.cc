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
#include <string>
#include <thread>

#include "gomoku/agent/heuristics/light_rollout_evaluator.h"
#include "gomoku/agent/search/mct_search.h"
#include "gomoku/agent_classroom/learning_material_generator.h"
#include "gomoku/agent_classroom/representative_data.h"
#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"
#include "gomoku/game/game_instance_container.h"
#include "gomoku/logging/game_log_store.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/pooled_connection_reservoir.h"

namespace e8 {

void GenerateRepresentativeData(GameInstanceContainer::ScheduleId schedule_id,
                                unsigned target_num_games, std::string const &db_host_name,
                                std::string const &db_name, GameInstanceContainer *container) {
    PooledConnectionReservoir conns(
        ConnectionFactory(ConnectionFactory::PQ, db_host_name, db_name));

    GameLogStore log_store(&conns);

    auto searcher = std::make_unique<MctSearcher>(std::make_shared<GomokuLightRolloutEvaluator>(),
                                                  /*print_stats=*/true);
    auto generator_data = std::make_shared<LearningMaterialGeneratorSharedData>(
        GameLogPurpose::GLP_REPRESENTATIVE_DATA, /*model_id=*/std::nullopt, target_num_games,
        std::move(searcher), &log_store);

    auto generator_game = std::make_unique<GomokuGame>(
        std::make_shared<LearningMaterialGenerator>(generator_data, PlayerSide::PS_PLAYER_A),
        std::make_shared<LearningMaterialGenerator>(generator_data, PlayerSide::PS_PLAYER_B));
    container->ScheduleToRun(schedule_id, std::move(generator_game));

    while (container->ScheduledGame(schedule_id) != nullptr) {
        // Wait for the generator to hit the target number games.
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

} // namespace e8
