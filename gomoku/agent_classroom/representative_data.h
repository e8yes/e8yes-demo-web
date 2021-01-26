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

#ifndef REPRESENTATIVE_DATA_H
#define REPRESENTATIVE_DATA_H

#include <string>

#include "gomoku/game/game_instance_container.h"

namespace e8 {

/**
 * @brief GenerateRepresentativeData Generates representative game data by self playing through a
 * Monte Carlo Tree searcher and a handed coded heuristics.
 *
 * @param schedule_id An unused schedule slot in the game intance container for this function to
 * launch games in.
 * @param target_num_games Target number of games to generate.
 * @param db_host_name Host name of the database to log the game data towards.
 * @param db_name Name of the database to log game data towards.
 * @param container An game instance container to for this function to launch game into.
 */
void GenerateRepresentativeData(GameInstanceContainer::ScheduleId schedule_id,
                                unsigned target_num_games, std::string const &db_host_name,
                                std::string const &db_name, GameInstanceContainer *container);

} // namespace e8

#endif // REPRESENTATIVE_DATA_H
