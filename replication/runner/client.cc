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

#include <chrono>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "common/random/random_source.h"
#include "common/random/sample.h"
#include "common/time_util/time_util.h"
#include "proto_cc/service_replication.grpc.pb.h"
#include "proto_cc/service_replication.pb.h"
#include "replication/raft/common_types.h"
#include "replication/runner/client.h"
#include "replication/runner/common_types.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {

ReplicationClient::ReplicationClient(ReplicationConfig const &config) {
    for (auto const &peer : config.peers) {
        auto peer_channel = grpc::CreateChannel(peer, grpc::InsecureChannelCredentials());
        auto stub = ReplicationService::NewStub(peer_channel);
        peer_stubs_.insert(std::make_pair(peer, std::move(stub)));

        peer_weights_.insert(std::make_pair(peer, 1.0f / config.peers.size()));
    }

    // The RunCommand() call will correct the leader address later on. We simply use the first node
    // in the peer set as a placeholder.
    possible_leader_ = *config.peers.begin();

    uuid4_seed(&uuid_state_);
}

ReplicationClient::~ReplicationClient() {}

std::string ReplicationClient::RunCommand(std::string const &command) {
    UUID4_T uuid;
    uuid4_gen(&uuid_state_, &uuid);

    char uuid_string[UUID4_STR_BUFFER_SIZE];
    uuid4_to_s(uuid, uuid_string, sizeof(uuid_string));

    RunEventId run_event_id(uuid_string);

    while (true) {
        RunCommandRequest request;
        request.set_run_event_id(run_event_id);
        request.set_command(command);

        RunCommandResponse response;

        grpc::ClientContext context;
        grpc::Status status =
            peer_stubs_[possible_leader_]->RunCommand(&context, request, &response);

        if (!status.ok() || response.error() == RunCommandError::RCE_TIMED_OUT) {
            // Unreachable/stuck in a wrong network partition. Picks a random node and retries.
            possible_leader_ = SampleFrom(peer_weights_, &random_source_);
            continue;
        }

        if (response.error() == RunCommandError::RCE_WRONG_NODE) {
            possible_leader_ = response.retry_address().value();
            continue;
        }

        if (response.error() == RunCommandError::RCE_NONE) {
            return response.return_value();
        }
    }
}

} // namespace e8
