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
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/time_util/time_util.h"
#include "proto_cc/command.pb.h"
#include "proto_cc/raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/context.h"
#include "replication/raft/integration_test/local_cluster.h"
#include "replication/raft/journal.h"
#include "replication/raft/raft_instance.h"
#include "replication/raft/schedule.h"

namespace e8 {
namespace {

constexpr char const *kTestHost = "localhost";
unsigned const kTestPortBeginRange = 12345;
constexpr char const *kTestLogFilePrefix = "local_raft_log";

class NoOpCommitListener : public RaftCommitListener {
  public:
    NoOpCommitListener() = default;
    ~NoOpCommitListener() override = default;

    void Apply(CommandEntry const & /*entry*/) override {}
};

RaftMachineAddress NodeAddress(unsigned port) {
    return std::string(kTestHost) + ":" + std::to_string(port);
}

std::string LogPath(RaftMachineAddress const node_address) {
    return std::string("./") + kTestLogFilePrefix + "." + node_address;
}

} // namespace

LocalRaftCluster::LocalRaftCluster() : started_(false) {}

LocalRaftCluster::~LocalRaftCluster() {
    std::vector<std::string> log_files;
    for (auto const &[address, _] : nodes_) {
        log_files.push_back(LogPath(address));
    }

    nodes_.clear();

    for (auto const &log_file : log_files) {
        int result = std::remove(log_file.c_str());
        assert(result == 0);
    }
}

LocalRaftCluster &
LocalRaftCluster::AddNode(std::shared_ptr<RaftCommitListener> const &commit_listener) {
    Node initial_node;
    if (commit_listener == nullptr) {
        initial_node.commit_listener = std::make_shared<NoOpCommitListener>();
    } else {
        initial_node.commit_listener = commit_listener;
    }

    RaftMachineAddress node_address = NodeAddress(kTestPortBeginRange + nodes_.size());
    nodes_.insert(std::make_pair(node_address, std::move(initial_node)));

    return *this;
}

LocalRaftCluster::Node const &LocalRaftCluster::Get(RaftMachineAddress const &node_address) const {
    auto it = nodes_.find(node_address);
    assert(it != nodes_.end());
    return it->second;
}

void LocalRaftCluster::Start(unsigned quorum_size, float unavailability) {
    assert(started_ == false);

    google::protobuf::RepeatedPtrField<RaftMachineAddress> peers;
    for (auto const &[addr, _] : nodes_) {
        *peers.Add() = addr;
    }

    for (auto &[addr, node] : nodes_) {
        node.config.set_me(addr);
        *node.config.mutable_peers() = peers;
        node.config.set_log_path(LogPath(addr));
        node.config.set_quorum_size(quorum_size);
        node.config.set_unavailability(unavailability);

        std::remove(node.config.log_path().c_str());

        node.instance = std::make_unique<RaftInstance>(node.commit_listener.get(), node.config);
    }

    started_ = true;
}

void LocalRaftCluster::Recover(RaftMachineAddress const &node_address) {
    auto it = nodes_.find(node_address);
    assert(it != nodes_.end());

    auto &[_, node] = *it;
    assert(node.instance == nullptr);
    node.instance = std::make_unique<RaftInstance>(node.commit_listener.get(), node.config);
}

void LocalRaftCluster::Shutdown(RaftMachineAddress const &node_address) {
    auto it = nodes_.find(node_address);
    assert(it != nodes_.end());

    auto &[_, node] = *it;
    assert(node.instance != nullptr);
    node.instance = nullptr;
    node.commit_listener->Reset();
}

void LocalRaftCluster::SetUnreliableNetwork(bool /*enable*/) {
    // TODO: introduces artificial unreliable communication.
}

TimeIntervalMillis LocalRaftCluster::LocalRaftCluster::ElectionTimeout() const {
    assert(!nodes_.empty());

    auto const &[_, some_node] = *nodes_.begin();

    RaftScheduleConfig config = FastElectionRaftScheduleConfig(some_node.config.unavailability());
    return config.heartbeat_max_timeout_millis + config.election_timeout_millis;
}

std::unordered_map<RaftMachineAddress, LocalRaftCluster::Node>::const_iterator
LocalRaftCluster::begin() const {
    return nodes_.begin();
}

std::unordered_map<RaftMachineAddress, LocalRaftCluster::Node>::const_iterator
LocalRaftCluster::end() const {
    return nodes_.end();
}

} // namespace e8
