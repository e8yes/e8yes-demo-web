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

#include <cassert>
#include <grpcpp/grpcpp.h>
#include <map>
#include <memory>

#include "distributor/node_state/module/propagator.h"
#include "distributor/node_state/module/sync_node_state.h"
#include "distributor/node_state/service/node_state_service.h"
#include "distributor/store/entity.h"
#include "distributor/store/node_state_store.h"
#include "distributor/store/peer_store.h"
#include "proto_cc/service_node_state.grpc.pb.h"
#include "proto_cc/service_node_state.pb.h"

namespace e8 {

NodeStateServiceImpl::NodeStateServiceImpl(std::string const &db_path)
    : node_states_(std::make_unique<NodeStateStore>(db_path)),
      peers_(std::make_unique<PeerStore>(db_path)),
      propagator_(std::make_unique<GrpcPropagator>()) {}

grpc::Status NodeStateServiceImpl::ReviseNodeState(grpc::ServerContext * /*context*/,
                                                   ReviseNodeStateRequest const *request,
                                                   ReviseNodeStateResponse * /*response*/) {
    bool updatable = false;
    for (auto const &revision : request->revisions()) {
        updatable |= node_states_->UpdateNodeStates(revision);
    }
    if (!updatable) {
        return grpc::Status::OK;
    }

    if (!SyncNodeStates(peers_.get(), node_states_.get(), propagator_.get())) {
        return grpc::Status(grpc::StatusCode::ABORTED, "Node synchronization failed.");
    }

    return grpc::Status::OK;
}

grpc::Status
NodeStateServiceImpl::GetCurrentRevisionEpoch(grpc::ServerContext * /*context*/,
                                              GetCurrentRevisionEpochRequest const * /*request*/,
                                              GetCurrentRevisionEpochResponse *response) {
    response->set_revision_epoch(node_states_->CurrentRevisionEpoch());
    return grpc::Status::OK;
}

grpc::Status NodeStateServiceImpl::AddPeers(grpc::ServerContext * /*context*/,
                                            AddPeersRequest const *request,
                                            AddPeersResponse * /*response*/) {
    for (auto const &node : request->nodes()) {
        peers_->AddPeer(node);
    }

    NodeStateRevision revision;
    // TODO: use paxos update.
    revision.set_revision_epoch(node_states_->CurrentRevisionEpoch() + 1);
    for (auto const &node : request->nodes()) {
        (*revision.mutable_nodes())[node.name()] = node;
        (*revision.mutable_delta_operations())[node.name()] = DOP_ADD;
    }

    ReviseNodeStateRequest revision_request;
    *revision_request.mutable_revisions()->Add() = revision;
    ReviseNodeStateResponse revision_response;
    grpc::Status status = this->ReviseNodeState(nullptr, &revision_request, &revision_response);
    assert(status.ok());

    return grpc::Status::OK;
}

grpc::Status NodeStateServiceImpl::DeletePeers(grpc::ServerContext * /*context*/,
                                               DeletePeersRequest const *request,
                                               DeletePeersResponse * /*response*/) {
    for (auto const &node_name : request->node_names()) {
        peers_->DeletePeer(node_name);
    }

    NodeStateRevision revision;
    // TODO: use paxos update.
    revision.set_revision_epoch(node_states_->CurrentRevisionEpoch() + 1);
    for (auto const &node_name : request->node_names()) {
        (*revision.mutable_delta_operations())[node_name] = DOP_DELETE;
    }

    ReviseNodeStateRequest revision_request;
    *revision_request.mutable_revisions()->Add() = revision;
    ReviseNodeStateResponse revision_response;
    grpc::Status status = this->ReviseNodeState(nullptr, &revision_request, &revision_response);
    assert(status.ok());

    return grpc::Status::OK;
}

} // namespace e8
