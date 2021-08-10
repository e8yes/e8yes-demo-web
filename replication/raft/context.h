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

#ifndef RAFT_CONTEXT_H
#define RAFT_CONTEXT_H

#include <memory>
#include <string>
#include <unordered_set>

#include "replication/raft/common_types.h"
#include "replication/raft/journal.h"
#include "replication/raft/persister.h"
#include "replication/raft/role_at_term.h"
#include "replication/raft/schedule.h"

namespace e8 {

/**
 * @brief The RaftConfig struct Defines the environment that Raft will work on.
 */
struct RaftConfig {
    // The machine address of the local node.
    RaftMachineAddress me;

    // The machine addresses of all the Raft peers, including the local node.
    std::unordered_set<RaftMachineAddress> peers;

    // File path to the local file system for storing Raft persistent states.
    std::string log_path;

    // The acceptable scale of unavailability. To achieve minimum down time, set this to 1.9.
    // However, higher scale reduces network traffic and computation.
    float unavailability;
};

/**
 * @brief The RaftContext struct Data and objects shared by both the Raft foreground and background.
 */
struct RaftContext {
    RaftMachineAddress me;
    std::unordered_set<RaftMachineAddress> peers;

    std::shared_ptr<RaftPersister> persister;
    std::unique_ptr<RoleAtTerm> role_at_term;
    std::unique_ptr<RaftJournal> journal;

    std::unique_ptr<FollowerSchedule> follower_schedule;
    std::unique_ptr<CandidateSchedule> candidate_schedule;
    std::unique_ptr<LeaderSchedule> leader_schedule;
};

/**
 * @brief CreateRaftContext Creates a new Raft context to be used by a background task and a
 * foreground task.
 *
 * @param commit_listener Custom object that responds to the committed commands.
 * @param config Defines the environment that Raft will work on.
 *
 * @return It should always return a valid context.
 */
std::shared_ptr<RaftContext>
CreateRaftContext(std::shared_ptr<RaftCommitListener> const &commit_listener,
                  RaftConfig const &config);

} // namespace e8

#endif // RAFT_CONTEXT_H
