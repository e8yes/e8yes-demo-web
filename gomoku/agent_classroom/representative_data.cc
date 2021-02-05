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

#include <memory>
#include <string>

#include "gomoku/agent/heuristics/light_rollout_evaluator.h"
#include "gomoku/agent_classroom/learning_material_generator.h"
#include "gomoku/agent_classroom/representative_data.h"
#include "gomoku/game/game_instance_container.h"

namespace e8 {

void GenerateRepresentativeData(GameInstanceContainer::ScheduleId schedule_id,
                                unsigned target_num_games, std::string const &db_host_name,
                                std::string const &db_name, GameInstanceContainer *container) {
    auto evaluator = std::make_shared<GomokuLightRolloutEvaluator>();
    GenerateLearningMaterial(GameLogPurpose::GLP_REPRESENTATIVE_DATA, /*model_id=*/std::nullopt,
                              evaluator, schedule_id, target_num_games, db_host_name, db_name,
                              container);
}

} // namespace e8
