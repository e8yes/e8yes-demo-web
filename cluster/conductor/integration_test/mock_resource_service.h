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

#ifndef CLUSTER_CONDUCTOR_MOCK_RESOURCE_SERVICE_H
#define CLUSTER_CONDUCTOR_MOCK_RESOURCE_SERVICE_H

#include <grpc/grpc.h>

#include "cluster/placement/cluster_map.h"
#include "common/random/random_source.h"
#include "proto_cc/service_resource_worker.grpc.pb.h"
#include "proto_cc/service_resource_worker.pb.h"

namespace e8 {

/**
 * @brief The MockResourceService class A mock resource worker service that reasonably responds the
 * ApplyClusterMapRevision() RPC as if it's locally managing a set of resources and the cluster map.
 * It also exposes its internal state for testers to validate the behavior of the conductor modules.
 */
class MockResourceService : public ResourceWorkerService::Service {
  public:
    MockResourceService();
    ~MockResourceService() override;

    grpc::Status
    ApplyClusterMapRevision(grpc::ServerContext *context,
                            ApplyClusterMapRevisionRequest const *request,
                            grpc::ServerWriter<ApplyClusterMapRevisionResponse> *writer) override;

    /**
     * @brief GetClusterMap Returns the latest local cluster map.
     */
    ClusterMap const &GetClusterMap() const;

    /**
     * @brief RpcCount Returns the number of times where ApplyClusterMapRevision() gets called.
     */
    unsigned RpcCount() const;

    /**
     * @brief AddFailures Makes the ApplyClusterMapRevision() RPC fail for num_failures times.
     */
    void AddFailures(unsigned num_failures);

  private:
    ClusterMap cluster_map_;
    unsigned rpc_count_;
    unsigned remaining_failure_count_;
    RandomSource random_source_;
};

} // namespace e8

#endif // CLUSTER_CONDUCTOR_MOCK_RESOURCE_SERVICE_H
