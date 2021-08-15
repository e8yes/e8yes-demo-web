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
#include <memory>
#include <string>
#include <unordered_set>

#include "replication/raft/commit_pusher.h"
#include "replication/raft/common_types.h"
#include "replication/raft/context.h"
#include "replication/raft/election.h"
#include "replication/raft/journal.h"
#include "replication/raft/journal_replicator.h"
#include "replication/raft/peer_set.h"
#include "replication/raft/persister.h"
#include "replication/raft/role_at_term.h"
#include "replication/raft/schedule.h"

namespace e8 {

std::unique_ptr<RaftContext> CreateRaftContext(RaftCommitListener *commit_listener,
                                               RaftConfig const &config) {
    auto context = std::make_unique<RaftContext>();

    RaftScheduleConfig schedule_config = FastElectionRaftScheduleConfig(config.unavailability);
    context->follower_schedule = std::make_unique<FollowerSchedule>(schedule_config);
    context->candidate_schedule = std::make_unique<CandidateSchedule>(schedule_config);
    context->leader_schedule = std::make_unique<LeaderSchedule>(schedule_config);

    context->me = config.me;
    context->peers = std::make_unique<RaftPeerSet>(config.peers, config.quorum_size);

    context->persister = std::make_unique<RaftPersister>(config.log_path);

    context->role_at_term = std::make_unique<RoleAtTerm>(context->persister.get());

    context->journal = std::make_unique<RaftJournal>(context->persister.get(), commit_listener);
    context->journal_replicator = std::make_unique<RaftJournalReplicator>(
        context->journal.get(), context->peers.get(), schedule_config.heartbeat_millis);
    context->commit_pusher = std::make_unique<RaftCommitPusher>(
        context->journal.get(), context->peers.get(), schedule_config.heartbeat_millis);

    context->voting_record = std::make_unique<RaftVotingRecord>();
    context->election_committee = std::make_unique<RaftElectionCommittee>(
        context->peers.get(), schedule_config.election_timeout_millis);

    return context;
}

} // namespace e8
