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

#include "distributor/node_state/service/node_state_service.h"
#include "distributor/store/constants.h"
#include "distributor/store/entity.h"
#include "distributor/store/node_state_store.h"
#include "distributor/store/peer_store.h"
#include "proto_cc/service_node_state.grpc.pb.h"
#include "proto_cc/service_node_state.pb.h"

namespace e8 {
namespace {

std::string IpStr(std::string const &ip_bytes) {
    if (ip_bytes.size() == 4) {
        return std::to_string(ip_bytes[0]) + "." + std::to_string(ip_bytes[1]) + "." +
               std::to_string(ip_bytes[2]) + "." + std::to_string(ip_bytes[3]);
    } else {
        assert(false);
    }
}

std::string DistributorPort(NodeState const &node) {
    for (int i = 0; i < node.functions_size(); i++) {
        if (node.functions(i) == NDF_DISTRIBUTOR) {
            return std::to_string(node.function_ports(i));
        }
    }
    assert(false);
}

std::string NodeToTargetStr(NodeState const &node) {
    return IpStr(node.ip_address()) + ":" + DistributorPort(node);
}

} // namespace

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

    RevisionEpoch current_epoch = node_states_.CurrentRevisionEpoch();

    // Propogate updates to the peers.
    std::map<NodeName, NodeState> peers = peers_.Peers();
    for (auto const &[_, node] : peers) {
        std::string target_string = NodeToTargetStr(node);
        std::unique_ptr<NodeStateService::Stub> stub = NodeStateService::NewStub(
            grpc::CreateChannel(target_string, grpc::InsecureChannelCredentials()));

        GetCurrentRevisionEpochResponse peer_epoch;
        grpc::Status status =
            stub->GetCurrentRevisionEpoch(nullptr, GetCurrentRevisionEpochRequest(), &peer_epoch);
        assert(status.ok());

        std::vector<NodeStateRevision> delta =
            node_states_.Revisions(peer_epoch.revision_epoch(), current_epoch);
        if (delta.empty()) {
            continue;
        }

        ReviseNodeStateRequest update_delta_request;
        *update_delta_request.mutable_revisions() = {delta.begin(), delta.end()};
        ReviseNodeStateResponse update_delta_response;
        status = stub->ReviseNodeState(nullptr, update_delta_request, &update_delta_response);
        assert(status.ok());
    }

    return grpc::Status::OK;
}

grpc::Status
NodeStateServiceImpl::GetCurrentRevisionEpoch(grpc::ServerContext * /*context*/,
                                              GetCurrentRevisionEpochRequest const * /*request*/,
                                              GetCurrentRevisionEpochResponse *response) {
    response->set_revision_epoch(node_states_.CurrentRevisionEpoch());
    return grpc::Status::OK;
}

grpc::Status NodeStateServiceImpl::AddPeers(grpc::ServerContext * /*context*/,
                                            AddPeersRequest const *request,
                                            AddPeersResponse * /*response*/) {
    for (auto const &node : request->nodes()) {
        peers_.AddPeer(node);
    }

    NodeStateRevision revision;
    // TODO: use paxos update.
    revision.set_revision_epoch(node_states_.CurrentRevisionEpoch() + 1);
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
        peers_.DeletePeer(node_name);
    }

    NodeStateRevision revision;
    // TODO: use paxos update.
    revision.set_revision_epoch(node_states_.CurrentRevisionEpoch() + 1);
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
