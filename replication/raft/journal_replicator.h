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

#ifndef RAFT_JOURNAL_REPLICATOR_H
#define RAFT_JOURNAL_REPLICATOR_H

#include <unordered_map>

#include "common/thread/thread_pool.h"
#include "common/time_util/time_util.h"
#include "replication/raft/common_types.h"
#include "replication/raft/journal.h"
#include "replication/raft/peer_set.h"

namespace e8 {

/**
 * @brief The RaftJournalReplicator class Provides means of journal replication and is essential to
 * make each Raft peer's physical local journal agrees upon what the logical journal should be. Note
 * that, this object isn't thread safe and therefore shouldn't be called in diferent threads.
 */
class RaftJournalReplicator {
  public:
    /**
     * @brief The Progress struct How far each node replicates.
     */
    struct Progress {
        std::unordered_map<RaftMachineAddress, RaftLogOffset> replication_progresses;
    };

    /**
     * @brief RaftJournalReplicator Sets up which journal to replicate and to which set of peers.
     *
     * @param journal Which journal to replicate.
     * @param peers Which set of peers to synchronize journal for.
     * @param rpc_timeout_millis The *recommended* number of milliseconds to time out a
     * MergeLogEntries() RPC to make the Replicate() call block less.
     */
    RaftJournalReplicator(RaftJournal const *journal, RaftPeerSet const *peers,
                          TimeIntervalMillis rpc_timeout_millis);

    /**
     * @brief Replicate Broadcasts log entries around the peers, in a bandwidth saving manner.
     * During the process, each peer receives a MergeLogEntries() RPC and tries to reconcile with
     * caller node's journal. This function also keeps track of each node's replication progress, so
     * it could save RPCs if the peer's journal doesn't get modified by other nodes before the next
     * Replicate() call. This function is not meant to succeed definitively and does not attempt to
     * record error durign replication. It only reports what each node gets replicated. The
     * replication progress could stagnate if some error persists, but the Raft schema should
     * resolve the cause as soon as possible.
     *
     * @param from The caller node's machinea address.
     * @param publisher_term The term the caller node is currently holding when it decides to call
     * this function.
     *
     * @return How far each node replicates.
     */
    Progress Replicate(RaftMachineAddress from, RaftTerm publisher_term);

    /**
     * @brief ResetProgress The progress cache can cause error if it were written in previous terms.
     * Use this function before the node comes to be a leader to discard the progress cache.
     */
    void ResetProgress();

  private:
    RaftJournal const *journal_;
    RaftPeerSet const *peers_;
    Progress progress_;
    ThreadPool thread_pool_;

    TimeIntervalMillis const rpc_timeout_millis_;
};

} // namespace e8

#endif // RAFT_JOURNAL_REPLICATOR_H
