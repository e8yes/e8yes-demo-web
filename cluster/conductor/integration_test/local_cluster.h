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

#ifndef CLUSTER_CONDUCTOR_LOCAL_CLUSTER_H
#define CLUSTER_CONDUCTOR_LOCAL_CLUSTER_H

#include <grpcpp/grpcpp.h>
#include <memory>
#include <vector>

#include "cluster/conductor/instance.h"
#include "cluster/conductor/integration_test/mock_resource_service.h"
#include "cluster/placement/common_types.h"
#include "proto_cc/cluster_conductor.pb.h"
#include "replication/raft/common_types.h"
#include "replication/runner/client.h"
#include "replication/runner/runner.h"

namespace e8 {

/**
 * @brief The LocalCluster class Manages a testing cluster consisting of locally running conductor
 * instances and resource service workers.
 */
class LocalCluster {
  public:
    /**
     * @brief The ConductorNode struct The client of this structure should only modify the conductor
     * instance. Other fields are readonly.
     */
    struct ConductorNode {
        // The IP address of the conductor node which houses the conductor instance.
        RaftMachineAddress address;

        // The configuration used to start the conductor instance.
        ClusterConductorConfig config;

        // The running conductor instance if not null.
        std::unique_ptr<ConductorInstance> conductor;
    };

    /**
     * @brief The WorkerNode struct The client of this structure should only interact upon the
     * service. Other fields are readonly.
     */
    struct WorkerNode {
        // The IP address of the resource worker service.
        ResourceWorkerAddress address;

        // The mock resource worker service for testing, if not null.
        std::unique_ptr<MockResourceService> service;

        // The running server providing the resource worker service.
        std::unique_ptr<grpc::Server> server;
    };

    /**
     * @brief LocalCluster Constructs a cluster running locally for testing purposes.
     *
     * @param num_conductors Specifies the number of conductor nodes in the cluster. Must be greater
     * than 0.
     * @param num_resource_workers Specifies the number of resource workers in the cluster. Could be
     * zero.
     */
    LocalCluster(unsigned num_conductors, unsigned num_resource_workers);
    ~LocalCluster();

    /**
     * @brief GetConductorClientConfig Returns the config that can be used to build a conductor
     * client. The built client will interact with all the conductor nodes in the cluster, whether
     * they are alive or not.
     */
    ReplicationClientConfig GetConductorClientConfig();

    /**
     * @brief GetWorkerNode Gets a worker node by a zero-offset index.
     *
     * @param i Must be in the interval [0, num_resource_workers).
     * @param A const reference to the worker node with index i.
     */
    WorkerNode const &GetWorkerNode(unsigned i) const;

  private:
    std::vector<ConductorNode> conductors_;
    std::vector<WorkerNode> resource_workers_;
};

} // namespace e8

#endif // CLUSTER_CONDUCTOR_LOCAL_CLUSTER_H
