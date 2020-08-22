/**
 * e8yes demo web server.
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

#ifndef NODESTATESERVICEIMPL_H
#define NODESTATESERVICEIMPL_H

#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>

#include "distributor/mutation_propagator/module/propagator.h"
#include "distributor/store/node_state_store.h"
#include "distributor/store/peer_store.h"
#include "proto_cc/service_node_state.grpc.pb.h"
#include "proto_cc/service_node_state.pb.h"

namespace e8 {

/**
 * @brief The NodeStateServiceImpl class
 */
class NodeStateServiceImpl : public NodeStateService::Service {
  public:
    NodeStateServiceImpl(std::string const &db_path);
    ~NodeStateServiceImpl() override = default;

    grpc::Status ReviseNodeState(grpc::ServerContext *context,
                                 ReviseNodeStateRequest const *request,
                                 ReviseNodeStateResponse *response) override;

    grpc::Status GetCurrentRevisionEpoch(grpc::ServerContext *context,
                                         GetCurrentRevisionEpochRequest const *request,
                                         GetCurrentRevisionEpochResponse *response) override;

    grpc::Status AddPeers(grpc::ServerContext *context, AddPeersRequest const *request,
                          AddPeersResponse *response) override;

    grpc::Status DeletePeers(grpc::ServerContext *context, DeletePeersRequest const *request,
                             DeletePeersResponse *response) override;

  private:
    std::unique_ptr<NodeStateStoreInterface> node_states_;
    std::unique_ptr<PeerStoreInterface> peers_;
    std::unique_ptr<PropagatorInterface> propagator_;
};

} // namespace e8

#endif // NODESTATESERVICEIMPL_H
