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

#include <cassert>
#include <string>

#include "common/flags/parse_flags.h"
#include "gomoku/agent_classroom/representative_data.h"
#include "gomoku/game/game_instance_container.h"

static char const kDbHostNameFlag[] = "db_host_name";
static char const kDbNameFlag[] = "db_name";
static char const kTargetNumGamesFlag[] = "target_num_games";

int main(int argc, char *argv[]) {
    e8::Argv(argc, argv);

    std::string db_host_name =
        e8::ReadFlag(kDbHostNameFlag, std::string(), e8::FromString<std::string>);
    std::string db_name = e8::ReadFlag(kDbNameFlag, std::string(), e8::FromString<std::string>);
    unsigned target_num_games =
        e8::ReadFlag(kTargetNumGamesFlag, unsigned(0), e8::FromString<unsigned>);

    assert(!db_host_name.empty());
    assert(!db_name.empty());
    assert(target_num_games > 0);

    e8::GameInstanceContainer::ScheduleId schedule_id =
        e8::AllocateGameInstanceContainerScheduleId();
    e8::GenerateRepresentativeData(schedule_id, target_num_games, db_host_name, db_name,
                                   e8::DefaultGameInstanceContainer());

    return 0;
}
