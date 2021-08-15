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

#ifndef RAFT_COMMIT_PUSHER_H
#define RAFT_COMMIT_PUSHER_H

#include "common/thread/thread_pool.h"
#include "common/time_util/time_util.h"
#include "replication/raft/common_types.h"
#include "replication/raft/journal.h"
#include "replication/raft/journal_replicator.h"
#include "replication/raft/peer_set.h"

namespace e8 {

/**
 * @brief The RaftCommitPusher class Notifies a commit progress that is safe for the peer at the
 * moment, so peers can push commited logs to their state mahcine.
 */
class RaftCommitPusher {
  public:
    /**
     * @brief RaftCommitPusher Constructs commit pusher to push commits both locally and among the
     * peers.
     *
     * @param journal The local journal which will receives the PushCommitProgress() call.
     * @param peers Peers that will be notified about their safe commit progress.
     * @param rpc_timeout_millis The *recommended* number of milliseconds to time out a
     * PushCommitProgress() RPC. It doesn't immediately affect the amount of time spent in Push(),
     * but it helps prevent a congested thread pool which could later blocks the Push() call.
     */
    RaftCommitPusher(RaftJournal *journal, RaftPeerSet const *peers,
                     TimeIntervalMillis rpc_timeout_millis);
    ~RaftCommitPusher();

    /**
     * @brief Push Notifies a commit progress that is safe for the peer at the moment, so peers can
     * push commited logs to their state mahcine.
     *
     * @param pusher The caller node's machine address.
     * @param pusher_term The caller node's term when the progress argument was generated.
     * @param progress The replication progress determined at push_term.
     */
    void Push(RaftMachineAddress const &pusher, RaftTerm pusher_term,
              RaftJournalReplicator::Progress const &progress);

  private:
    RaftJournal *journal_;
    RaftPeerSet const *peers_;
    ThreadPool thread_pool_;
    TimeIntervalMillis const rpc_timeout_millis_;
};

} // namespace e8

#endif // RAFT_COMMIT_PUSHER_H
