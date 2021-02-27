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

#ifndef LEARNING_MATERIAL_GENERATOR_H
#define LEARNING_MATERIAL_GENERATOR_H

#include <memory>
#include <optional>
#include <string>

#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/game/game_instance_container.h"
#include "gomoku/logging/common_types.h"
#include "gomoku/logging/game_log_store.h"

namespace e8 {

/**
 * @brief GenerateLearningMaterial Logs game action data generated from self playing. The actions
 * from the self playing is computed with a Monte Carlo tree searcher and an evaluator heuristics
 * supplied by the caller. The stochastic policy and final outcome of each action are logged, so
 * they can be used as the label for model training.
 *
 * @param log_purpose The purpose of the generated learning material.
 * @param model_id If the heuristics uses a model, the model ID can be stored in the logs.
 * @param evaluator The heuristics that aids the Monte Carlo tree searcher.
 * @param early_termination Terminate the self play by making the winning move if there exists one
 * for any one of the players.
 * @param schedule_id An unused schedule slot in the game intance container for this function to
 * launch games in.
 * @param target_num_games Target number of games to generate.
 * @param db_host_name Host name of the database to log the game data towards.
 * @param db_name Name of the database to log game data towards.
 * @param container A game instance container to for this function to launch game into.
 */
void GenerateLearningMaterial(GameLogPurpose log_purpose, std::optional<ModelId> model_id,
                              std::shared_ptr<GomokuEvaluatorInterface> const &evaluator,
                              bool early_termination, GameInstanceContainer::ScheduleId schedule_id,
                              unsigned target_num_games, std::string const &db_host_name,
                              std::string const &db_name, GameInstanceContainer *container);

} // namespace e8

#endif // LEARNING_MATERIAL_GENERATOR_H
