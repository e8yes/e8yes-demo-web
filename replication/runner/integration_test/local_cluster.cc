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

#include <algorithm>
#include <cstdio>
#include <google/protobuf/repeated_field.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/time_util/time_util.h"
#include "proto_cc/replication.pb.h"
#include "replication/raft/common_types.h"
#include "replication/runner/client.h"
#include "replication/runner/integration_test/key_value_store.h"
#include "replication/runner/integration_test/local_cluster.h"
#include "replication/runner/service/replication_service.h"

namespace e8 {
namespace {

constexpr char const *kTestHost = "localhost";
unsigned const kRaftTestPortBeginRange = 12345;
unsigned const kReplicationTestPortBeginRange = 13345;
constexpr char const *kTestLogFilePrefix = "local_raft_log";
float const kTestUnavailability = 1.0f;
TimeIntervalMillis kTestFulfillmentTimeout = 50;

RaftMachineAddress NodeAddress(unsigned port) {
    return std::string(kTestHost) + ":" + std::to_string(port);
}

std::string LogPath(RaftMachineAddress const node_address) {
    return std::string("./") + kTestLogFilePrefix + "." + node_address;
}

std::unique_ptr<grpc::Server> StartReplicationServer(RaftMachineAddress const &addr,
                                                     ReplicationServiceImpl *service) {
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    grpc::ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(service);

    return builder.BuildAndStart();
}

} // namespace

LocalReplicationCluster::LocalReplicationCluster(unsigned num_nodes) {
    google::protobuf::RepeatedPtrField<RaftMachineAddress> raft_peers;
    google::protobuf::RepeatedPtrField<RaftMachineAddress> replication_peers;

    for (unsigned i = 0; i < num_nodes; ++i) {
        RaftMachineAddress raft_addr = NodeAddress(kRaftTestPortBeginRange + i);
        RaftMachineAddress replication_addr = NodeAddress(kReplicationTestPortBeginRange + i);

        raft_peers.Add(std::move(raft_addr));
        replication_peers.Add(std::move(replication_addr));
    }

    for (unsigned i = 0; i < num_nodes; ++i) {
        Node node;
        node.kv_store_ = std::make_unique<ReplicationKvStore>();

        node.config.mutable_raft_config()->set_me(raft_peers[i]);
        *node.config.mutable_raft_config()->mutable_peers() = raft_peers;

        std::string log_path = LogPath(raft_peers[i]);
        std::remove(log_path.c_str());
        node.config.mutable_raft_config()->set_log_path(log_path);

        node.config.mutable_raft_config()->set_quorum_size(num_nodes / 2 + 1);
        node.config.mutable_raft_config()->set_unavailability(kTestUnavailability);

        node.config.set_fulfillment_timeout_millis(kTestFulfillmentTimeout);

        node.service_ = std::make_unique<ReplicationServiceImpl>(node.kv_store_.get(), node.config);
        node.server_ = StartReplicationServer(replication_peers[i], node.service_.get());

        nodes_.insert(std::make_pair(replication_peers[i], std::move(node)));
    }
}

LocalReplicationCluster::~LocalReplicationCluster() {
    std::vector<std::string> log_paths;

    for (auto &[addr, node] : nodes_) {
        node.server_->Shutdown();
        node.server_->Wait();

        log_paths.push_back(LogPath(addr));
        log_paths.push_back(LogPath(node.config.raft_config().me()));
    }

    nodes_.clear();

    for (auto const &log_path : log_paths) {
        std::remove(log_path.c_str());
    }
}

ReplicationClientConfig LocalReplicationCluster::ClientConfig() const {
    ReplicationClientConfig config;
    for (auto const &[node_addr, _] : nodes_) {
        config.peers.insert(node_addr);
    }
    return config;
}

} // namespace e8
