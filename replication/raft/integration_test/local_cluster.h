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

#ifndef RAFT_LOCAL_CLUSTER_H
#define RAFT_LOCAL_CLUSTER_H

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "common/time_util/time_util.h"
#include "proto_cc/raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/context.h"
#include "replication/raft/journal.h"
#include "replication/raft/raft_instance.h"

namespace e8 {

/**
 * @brief The LocalRaftCluster class Manages a testing cluster of locally running Raft nodes.
 */
class LocalRaftCluster {
  public:
    /**
     * @brief The Node struct Consists of
     * 1. The configuration to start the Raft instance.
     * 2. Owns the commit listener object that listens to the instance.
     * 3. The Raft instance. It could be a nullptr if it's stopped.
     */
    struct Node {
        RaftConfig config;
        std::shared_ptr<RaftCommitListener> commit_listener;
        std::unique_ptr<RaftInstance> instance;
    };

    /**
     * @brief LocalRaftCluster The constructor doesn't start a local cluster. See the Start().
     */
    LocalRaftCluster();
    ~LocalRaftCluster();

    /**
     * @brief AddNode Adds a node to the cluster with the specified commit listner. If the commit
     * listener is empty, it will fallback to a default no-op listner. The client can't add node
     * once the cluster has started.
     */
    LocalRaftCluster &AddNode(std::shared_ptr<RaftCommitListener> const &commit_listener);

    /**
     * @brief Get Retrives a node by its address. It returns a const reference to the node stored
     * internally.
     */
    Node const &Get(RaftMachineAddress const &node_address) const;

    /**
     * @brief Start Starts the cluster will the specified quorum size and unavailability preference.
     */
    void Start(unsigned quorum_size, float unavailability = 1.0f);

    /**
     * @brief Recover Restarts a node that is currently down. If the node is still running, this
     * function will fail.
     */
    void Recover(RaftMachineAddress const &node_address);

    /**
     * @brief Shutdown Stops a currently running node. If the node is shut, this function will fail.
     */
    void Shutdown(RaftMachineAddress const &node_address);

    /**
     * @brief SetUnreliableNetwork Makes the Raft communication unreliable or brings it back to the
     * normal state.
     */
    void SetUnreliableNetwork(bool enable);

    /**
     * @brief ElectionTimeout The maximum election timeout the nodes are using.
     */
    TimeIntervalMillis ElectionTimeout() const;

    // Iterates through the nodes in the cluster.
    std::unordered_map<RaftMachineAddress, Node>::const_iterator begin() const;
    std::unordered_map<RaftMachineAddress, Node>::const_iterator end() const;

  private:
    bool started_;
    std::unordered_map<RaftMachineAddress, Node> nodes_;
};

} // namespace e8

#endif // RAFT_LOCAL_CLUSTER_H
