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

#include <cassert>
#include <cstdio>
#include <google/protobuf/repeated_field.h>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "cluster/conductor/instance.h"
#include "cluster/conductor/integration_test/local_cluster.h"
#include "cluster/conductor/integration_test/mock_resource_service.h"
#include "cluster/placement/common_types.h"
#include "replication/raft/common_types.h"
#include "replication/runner/client.h"
#include "replication/runner/runner.h"

namespace e8 {
namespace {

constexpr char const *kTestHost = "localhost";
unsigned const kTestCondutorPortBeginRange = 12345;
unsigned const kTestWorkerPortBeginRange = 22345;
constexpr char const *kTestLogFilePrefix = "local_raft_log";
constexpr char const *kTestRevisionSnapshotFilePrefix = "conductor_revision_snapshot";

RaftMachineAddress GenerateConductorAddress(unsigned i) {
    return std::string(kTestHost) + ":" + std::to_string(kTestCondutorPortBeginRange + i);
}

ResourceWorkerAddress GenerateResourceWorkerAddress(unsigned i) {
    return std::string(kTestHost) + ":" + std::to_string(kTestWorkerPortBeginRange + i);
}

std::string LogFilePath(RaftMachineAddress const node_address) {
    return std::string("./") + kTestLogFilePrefix + "." + node_address;
}

std::string RevisionSnapshotFilePath(RaftMachineAddress const node_address) {
    return std::string("./") + kTestRevisionSnapshotFilePrefix + "." + node_address;
}

void StartConductors(unsigned num_conductors,
                     std::vector<LocalCluster::ConductorNode> *conductors) {
    google::protobuf::RepeatedPtrField<RaftMachineAddress> conductor_addresses;
    for (unsigned i = 0; i < num_conductors; ++i) {
        conductor_addresses.Add(GenerateConductorAddress(i));
    }

    for (unsigned i = 0; i < num_conductors; ++i) {
        // Removes leftover files from the previous run.
        remove(RevisionSnapshotFilePath(conductor_addresses[i]).c_str());
        remove(LogFilePath(conductor_addresses[i]).c_str());

        // Creates the conductor while recording the configuration it uses.
        LocalCluster::ConductorNode node;
        node.address = conductor_addresses[i];

        node.config.set_revision_work_pool_snapshot_file(
            RevisionSnapshotFilePath(conductor_addresses[i]));
        node.config.mutable_replication_config()->set_fulfillment_timeout_millis(100);
        node.config.mutable_replication_config()->mutable_raft_config()->set_me(
            conductor_addresses[i]);
        node.config.mutable_replication_config()->mutable_raft_config()->set_log_path(
            LogFilePath(conductor_addresses[i]));
        node.config.mutable_replication_config()->mutable_raft_config()->set_quorum_size(
            num_conductors / 2 + 1);
        node.config.mutable_replication_config()->mutable_raft_config()->set_unavailability(1.0f);
        *node.config.mutable_replication_config()->mutable_raft_config()->mutable_peers() =
            conductor_addresses;

        node.conductor = std::make_unique<ConductorInstance>(node.config);

        conductors->emplace_back(std::move(node));
    }
}

void StartResourceWorkers(unsigned num_resource_workers,
                          std::vector<LocalCluster::WorkerNode> *resource_workers) {
    for (unsigned i = 0; i < num_resource_workers; ++i) {
        LocalCluster::WorkerNode node;
        node.address = GenerateResourceWorkerAddress(i);
        node.service = std::make_unique<MockResourceService>();

        grpc::ServerBuilder builder;
        builder.AddListeningPort(node.address, grpc::InsecureServerCredentials());
        builder.RegisterService(node.service.get());
        node.server = builder.BuildAndStart();

        resource_workers->emplace_back(std::move(node));
    }
}

} // namespace

LocalCluster::LocalCluster(unsigned num_conductors, unsigned num_resource_workers) {
    assert(num_conductors > 0);

    StartConductors(num_conductors, &conductors_);
    StartResourceWorkers(num_resource_workers, &resource_workers_);
}

LocalCluster::~LocalCluster() {
    for (auto const &conductor : conductors_) {
        remove(RevisionSnapshotFilePath(conductor.address).c_str());
        remove(LogFilePath(conductor.address).c_str());
    }
}

ReplicationClientConfig LocalCluster::GetConductorClientConfig() {
    ReplicationClientConfig config;
    for (auto const &condutor : conductors_) {
        config.peers.insert(condutor.address);
    }
    return config;
}

LocalCluster::WorkerNode const &LocalCluster::GetWorkerNode(unsigned i) const {
    assert(i < resource_workers_.size());
    return resource_workers_[i];
}

} // namespace e8
