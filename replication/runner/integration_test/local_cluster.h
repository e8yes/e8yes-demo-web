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

#ifndef REPLICATION_LOCAL_CLUSTER_H
#define REPLICATION_LOCAL_CLUSTER_H

#include <grpcpp/grpcpp.h>
#include <memory>
#include <unordered_map>

#include "proto_cc/replication.pb.h"
#include "replication/raft/common_types.h"
#include "replication/runner/client.h"
#include "replication/runner/integration_test/key_value_store.h"
#include "replication/runner/service/replication_service.h"

namespace e8 {

/**
 * @brief The LocalReplicationCluster class Manages a testing cluster of locally running replication
 * nodes.
 */
class LocalReplicationCluster {
  public:
    /**
     * @brief LocalReplicationCluster Starts a cluster of specified number of nodes.
     */
    LocalReplicationCluster(unsigned num_nodes, RaftLogOffset preferred_snapshot_frequency);
    ~LocalReplicationCluster();

    /**
     * @brief ClientConfig Returns a client configuration for talking with the cluster from a
     * ReplicationClient.
     */
    ReplicationClientConfig ClientConfig() const;

  private:
    struct Node {
        ReplicationConfig config;
        std::unique_ptr<ReplicationKvStore> kv_store_;
        std::unique_ptr<ReplicationServiceImpl> service_;
        std::unique_ptr<grpc::Server> server_;
    };

    std::unordered_map<RaftMachineAddress, Node> nodes_;
};

} // namespace e8

#endif // REPLICATION_LOCAL_CLUSTER_H
