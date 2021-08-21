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
#include <chrono>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

#include "common/time_util/time_util.h"
#include "proto_cc/command.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/integration_test/local_cluster.h"
#include "replication/raft/integration_test/replication_monitor.h"
#include "replication/raft/journal.h"

namespace e8 {
namespace {

TimeIntervalMillis kInspectionInterval = 10;

std::unordered_map<RaftMachineAddress, RaftReplicationMonitor::Listener *>
ClusterListeners(LocalRaftCluster const *cluster) {
    std::unordered_map<RaftMachineAddress, RaftReplicationMonitor::Listener *> listeners;

    for (auto const &[node_addr, node] : *cluster) {
        listeners[node_addr] =
            dynamic_cast<RaftReplicationMonitor::Listener *>(node.commit_listener.get());
    }

    return listeners;
}

} // namespace

RaftReplicationMonitor::Listener::Listener() {}

RaftReplicationMonitor::Listener::~Listener() {}

void RaftReplicationMonitor::Listener::Apply(CommandEntry const &entry) {
    std::lock_guard<std::mutex> guard(mu);
    commited_entries.push_back(entry);
}

RaftReplicationMonitor::RaftReplicationMonitor(LocalRaftCluster const *cluster)
    : cluster_(cluster) {}

RaftReplicationMonitor::~RaftReplicationMonitor() {}

bool RaftReplicationMonitor::ReplicateCommand(RaftMachineAddress const &from,
                                              CommandEntry const &entry, unsigned num_agrees) {
    // Keeps broadcasting the command until the call is succcessful, though it says nothing about
    // whether the command will ever be committed.
    RaftForeground::BoardcastResult result(/*retry_node=*/from);

    while (result.retry_node.has_value()) {
        LocalRaftCluster::Node const &node = cluster_->Get(*result.retry_node);
        assert(node.instance != nullptr);

        result = node.instance->Foreground()->BoardcastCommand(entry);

        std::this_thread::sleep_for(std::chrono::milliseconds(kInspectionInterval));
    }

    // Keep polling the listeners to see if the command is committed in any node until it reaches
    // the specified number of agreed nodes.
    std::unordered_map<RaftMachineAddress, RaftReplicationMonitor::Listener *> commit_listeners =
        ClusterListeners(cluster_);

    for (TimeIntervalMillis time_spent = 0; time_spent < 2 * cluster_->ElectionTimeout();
         time_spent += kInspectionInterval) {

        unsigned num_nodes_committed = 0;

        for (auto const &[node, listener] : commit_listeners) {
            std::lock_guard<std::mutex> guard(listener->mu);
            if (listener->commited_entries.size() > result.log_index) {
                assert(listener->commited_entries[result.log_index].SerializeAsString() ==
                       entry.SerializeAsString());
                ++num_nodes_committed;
            }
        }

        if (num_nodes_committed >= num_agrees) {
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(kInspectionInterval));
    }

    return false;
}

} // namespace e8
