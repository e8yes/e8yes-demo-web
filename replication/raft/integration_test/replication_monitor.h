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

#ifndef RAFT_REPLICATION_MONITOR_H
#define RAFT_REPLICATION_MONITOR_H

#include <mutex>
#include <vector>

#include "proto_cc/command.pb.h"
#include "proto_cc/raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/integration_test/local_cluster.h"
#include "replication/raft/journal.h"

namespace e8 {

/**
 * @brief The RaftReplicationMonitor class Monitors command commits. Specifically, it checks if all
 * nodes commit the same entry at the same index and if there are enough nodes to make the
 * commitment.
 */
class RaftReplicationMonitor {
  public:
    /**
     * @brief The Listener class The listener to be used when adding nodes to the cluster.
     */
    class Listener : public RaftCommitListener {
      public:
        Listener(RaftLogOffset preferred_snapshot_frequency);
        ~Listener() override;

        void Apply(CommandEntry const &entry) override;

        RaftLogOffset PreferredSnapshotFrequency() const override;

        void Save() const override;

        void Restore() override;

        void Reset() override;

      public:
        std::mutex mu;
        CommittedCommandEntriesData data;

      private:
        RaftLogOffset const preferred_snapshot_frequency_;
    };

    /**
     * @brief RaftReplicationMonitor The cluster's node must be added with the above Listener for
     * this class to work.
     */
    explicit RaftReplicationMonitor(LocalRaftCluster const *cluster);
    ~RaftReplicationMonitor();

    /**
     * @brief ReplicateCommand Attempts to replicate a command in the cluster and reports if the
     * attempt was successful in 2 maximum election timeout periods.
     *
     * @param from The initial node to try broadcasting the command from. The function may choose a
     * different node if the initial broadcast fails.
     * @param entry The command to be replicated.
     * @param num_agrees The minimum number of nodes require to agree on the command's replication
     * in order for this function to be successful.
     * @return Whether the replication is successful.
     */
    bool ReplicateCommand(RaftMachineAddress const &from, CommandEntry const &entry,
                          unsigned num_agrees);

  private:
    LocalRaftCluster const *cluster_;
};

} // namespace e8

#endif // RAFT_REPLICATION_MONITOR_H
