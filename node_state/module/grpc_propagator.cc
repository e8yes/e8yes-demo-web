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

#include <grpcpp/grpcpp.h>
#include <memory>
#include <optional>

#include "distributor/mutation_propagator/propagator.h"
#include "node_state/module/grpc_propagator.h"
#include "proto_cc/node.pb.h"
#include "proto_cc/service_node_state.grpc.pb.h"
#include "proto_cc/service_node_state.pb.h"

namespace e8 {
namespace {
std::string IpStr(std::string const &ip_bytes) {
    if (ip_bytes.size() == 4) {
        return std::to_string(static_cast<unsigned int>(static_cast<unsigned char>(ip_bytes[0]))) +
               "." +
               std::to_string(static_cast<unsigned int>(static_cast<unsigned char>(ip_bytes[1]))) +
               "." +
               std::to_string(static_cast<unsigned int>(static_cast<unsigned char>(ip_bytes[2]))) +
               "." +
               std::to_string(static_cast<unsigned int>(static_cast<unsigned char>(ip_bytes[3])));
    } else {
        // TODO: Add IPv6 support.
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

std::unique_ptr<NodeStateService::Stub> CreateStub(NodeState const &target) {
    std::string target_string = NodeToTargetStr(target);
    return NodeStateService::NewStub(
        grpc::CreateChannel(target_string, grpc::InsecureChannelCredentials()));
}

} // namespace

std::optional<RevisionEpoch> GrpcPropagator::GetRevisionEpoch(NodeState const &target) {
    std::unique_ptr<NodeStateService::Stub> stub = CreateStub(target);

    grpc::ClientContext context;

    GetCurrentRevisionEpochResponse peer_epoch;
    grpc::Status status =
        stub->GetCurrentRevisionEpoch(&context, GetCurrentRevisionEpochRequest(), &peer_epoch);
    if (!status.ok()) {
        return std::nullopt;
    }

    return peer_epoch.revision_epoch();
}

bool GrpcPropagator::PropagateDelta(NodeState const &target,
                                    std::vector<NodeStateRevision> const &delta) {
    std::unique_ptr<NodeStateService::Stub> stub = CreateStub(target);

    grpc::ClientContext context;

    ReviseNodeStateRequest update_delta_request;
    *update_delta_request.mutable_revisions() = {delta.begin(), delta.end()};

    ReviseNodeStateResponse update_delta_response;
    grpc::Status status =
        stub->ReviseNodeState(&context, update_delta_request, &update_delta_response);

    return status.ok();
}

} // namespace e8
