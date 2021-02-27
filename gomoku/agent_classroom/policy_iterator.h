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

#ifndef POLICY_ITERATOR_H
#define POLICY_ITERATOR_H

#include <optional>
#include <string>

#include "gomoku/game/game_instance_container.h"

namespace e8 {

/**
 * @brief IterateFromLastPolicy Improves the last created policy via policy iteration. It tries to
 * load the latest model then generate learning material through it. The model can then be trained
 * on the new, slightly corrected, learning material to achieve a better policy and value
 * estimation. If there isn't any previous model, it will create a new model with parameters
 * randomly initialized then optionally trained on the representative data set.
 *
 * @param schedule_id An unused schedule slot in the game intance container for this function to
 * launch games in.
 * @param model_name Name of the model to iterate policy on.
 * @param num_iterations How many more iterations of learning material generation and model training
 * needs to be run.
 * @param num_games_per_iteration The number of games to run before model update.
 * @param bootstrap_from_rep_data If there is no past model, this argument tells the function
 * whether to initialize the first model by training it on the representative data set.
 * @param source_tree_root The root directory pointing to the project's code repository. This
 * function will launch python programs in the code repository.
 * @param model_storage_path Where to store the improved models.
 * @param db_host_name Host name of the database to log the model metadata and learning material
 * towards.
 * @param db_name Name of the database to log the model metadata and learning material towards.
 * @param container A game instance container to for this function to launch game into.
 */
void IterateFromLastPolicy(GameInstanceContainer::ScheduleId schedule_id,
                           std::string const &model_name, unsigned num_iterations,
                           unsigned num_games_per_iteration, bool bootstrap_from_rep_data,
                           std::string const &source_tree_root,
                           std::string const &model_storage_path, std::string const &db_host_name,
                           std::string const &db_name, GameInstanceContainer *container);

} // namespace e8

#endif // POLICY_ITERATOR_H
