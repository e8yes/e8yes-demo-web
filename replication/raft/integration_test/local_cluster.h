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

#include "replication/raft/common_types.h"
#include "replication/raft/journal.h"
#include "replication/raft/raft_instance.h"

namespace e8 {

/**
 * @brief StartLocalRaftCluster Starts a Raft cluster running locally and returns raft instances
 * representing the individual nodes.
 *
 * @param listeners A set of commit listeners that will connect to an individual Raft node. The size
 * of the listener set determines the size of the cluster.
 * @param quorum_size Decides what will be considered a quorum in this cluster.
 * @return Raft instances keyed by their unique machine address.
 */
std::unordered_map<RaftMachineAddress, std::unique_ptr<RaftInstance>>
StartLocalRaftCluster(std::unordered_set<RaftCommitListener *> const &listeners,
                      unsigned quorum_size);

/**
 * @brief ShutdownLocalRaftCluster Shuts down the cluster and cleans up resource.
 */
void DestroyLocalRaftCluster(
    std::unordered_map<RaftMachineAddress, std::unique_ptr<RaftInstance>> *cluster);

} // namespace e8

#endif // RAFT_LOCAL_CLUSTER_H
