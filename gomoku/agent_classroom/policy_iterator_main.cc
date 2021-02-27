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
#include "gomoku/agent_classroom/policy_iterator.h"
#include "gomoku/game/game_instance_container.h"

static char const kModelNameFlag[] = "model_name";
static char const kDbHostNameFlag[] = "db_host_name";
static char const kDbNameFlag[] = "db_name";
static char const kNumIterationsFlag[] = "num_iterations";
static char const kNumGamesPerIterationFlag[] = "num_games_per_iteration";
static char const kBootstrapFromRepDataFlag[] = "bootstrap_from_rep_data";
static char const kSourceTreeRootFlag[] = "source_tree_root";
static char const kModelStoragePathFlag[] = "model_storage_path";

int main(int argc, char *argv[]) {
    e8::Argv(argc, argv);

    std::string model_name =
        e8::ReadFlag(kModelNameFlag, std::string(), e8::FromString<std::string>);
    std::string db_host_name =
        e8::ReadFlag(kDbHostNameFlag, std::string(), e8::FromString<std::string>);
    std::string db_name = e8::ReadFlag(kDbNameFlag, std::string(), e8::FromString<std::string>);
    unsigned num_iterations =
        e8::ReadFlag(kNumIterationsFlag, unsigned(0), e8::FromString<unsigned>);
    unsigned num_games_per_iteration =
        e8::ReadFlag(kNumGamesPerIterationFlag, unsigned(0), e8::FromString<unsigned>);
    bool bootstrap_from_rep_data =
        e8::ReadFlag(kBootstrapFromRepDataFlag, bool(false), e8::FromString<bool>);
    std::string source_tree_root =
        e8::ReadFlag(kSourceTreeRootFlag, std::string(), e8::FromString<std::string>);
    std::string model_storage_path =
        e8::ReadFlag(kModelStoragePathFlag, std::string(), e8::FromString<std::string>);

    assert(!model_name.empty());
    assert(!db_host_name.empty());
    assert(!db_name.empty());
    assert(num_iterations > 0);
    assert(num_games_per_iteration > 0);
    assert(!source_tree_root.empty());
    assert(!model_storage_path.empty());

    e8::GameInstanceContainer::ScheduleId schedule_id =
        e8::AllocateGameInstanceContainerScheduleId();
    e8::IterateFromLastPolicy(schedule_id, model_name, num_iterations, num_games_per_iteration,
                              bootstrap_from_rep_data, source_tree_root, model_storage_path,
                              db_host_name, db_name, e8::DefaultGameInstanceContainer());

    return 0;
}
