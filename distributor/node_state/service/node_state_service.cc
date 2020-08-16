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

#include <grpcpp/grpcpp.h>

#include "distributor/node_state/service/node_state_service.h"
#include "distributor/store/constants.h"
#include "distributor/store/node_state_store.h"
#include "distributor/store/peer_store.h"
#include "proto_cc/service_node_state.grpc.pb.h"
#include "proto_cc/service_node_state.pb.h"

namespace e8 {

NodeStateServiceImpl::NodeStateServiceImpl()
    : node_states_(kDefaultNodeStatesDatabasePath), peers_(kDefaultNodeStatesDatabasePath) {}

grpc::Status NodeStateServiceImpl::ReviseNodeState(grpc::ServerContext * /*context*/,
                                                   ReviseNodeStateRequest const *request,
                                                   ReviseNodeStateResponse * /*response*/) {
    bool updatable = false;
    for (auto const &revision : request->revisions()) {
        updatable |= node_states_.UpdateNodeStates(revision);
    }
    if (!updatable) {
        return grpc::Status::OK;
    }

    // TODO: Propogate updates to the peers.
    return grpc::Status::OK;
}

} // namespace e8
