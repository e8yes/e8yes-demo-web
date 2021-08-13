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
#include <unordered_set>

#include "proto_cc/service_raft.grpc.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/context.h"
#include "replication/raft/election.h"
#include "replication/raft/journal.h"
#include "replication/raft/peer_set.h"
#include "replication/raft/persister.h"
#include "replication/raft/role_at_term.h"
#include "replication/raft/schedule.h"

namespace e8 {

std::shared_ptr<RaftContext>
CreateRaftContext(std::shared_ptr<RaftCommitListener> const &commit_listener,
                  RaftConfig const &config) {
    std::shared_ptr<RaftContext> context = std::make_shared<RaftContext>();

    context->me = config.me;
    context->peers = std::make_unique<RaftPeerSet>(config.peers);

    context->persister = std::make_shared<RaftPersister>(config.log_path);
    context->role_at_term = std::make_unique<RoleAtTerm>(context->persister);
    context->journal = std::make_unique<RaftJournal>(context->persister, commit_listener);

    RaftScheduleConfig schedule_config = FastElectionRaftScheduleConfig(config.unavailability);
    context->follower_schedule = std::make_unique<FollowerSchedule>(schedule_config);
    context->candidate_schedule = std::make_unique<CandidateSchedule>(schedule_config);
    context->leader_schedule = std::make_unique<LeaderSchedule>(schedule_config);

    context->voting_record = std::make_unique<RaftVotingRecord>();

    return context;
}

} // namespace e8
