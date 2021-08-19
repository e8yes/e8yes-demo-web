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
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "replication/raft/common_types.h"
#include "replication/raft/integration_test/local_cluster.h"
#include "replication/raft/journal.h"
#include "replication/raft/raft_instance.h"

namespace e8 {
namespace {

unsigned const kTestPortBeginRange = 12345;
constexpr char const *kTestLogFilePrefix = "local_raft_log";
float const kTestAvailability = 1.0f;

} // namespace

std::unordered_map<RaftMachineAddress, std::unique_ptr<RaftInstance>>
StartLocalRaftCluster(std::unordered_set<RaftCommitListener *> const &listeners,
                      unsigned quorum_size) {
    std::unordered_set<RaftMachineAddress> peer_addresses;
    for (unsigned port = kTestPortBeginRange; port < kTestPortBeginRange + listeners.size();
         ++port) {
        peer_addresses.insert("localhost:" + std::to_string(port));
    }

    std::unordered_map<RaftMachineAddress, std::unique_ptr<RaftInstance>> cluster;

    auto peer_it = peer_addresses.begin();
    for (auto commit_listener : listeners) {
        RaftConfig config;
        config.me = *peer_it;
        config.peers = peer_addresses;
        config.log_path = std::string("./") + kTestLogFilePrefix + "." + config.me;
        config.quorum_size = quorum_size;
        config.unavailability = kTestAvailability;

        ++peer_it;

        auto raft = std::make_unique<RaftInstance>(commit_listener, config);

        cluster.insert(std::make_pair(config.me, std::move(raft)));
    }

    return cluster;
}

void DestroyLocalRaftCluster(
    std::unordered_map<RaftMachineAddress, std::unique_ptr<RaftInstance>> *cluster) {
    std::vector<std::string> log_files;
    for (auto const &[address, _] : *cluster) {
        log_files.push_back(std::string("./") + kTestLogFilePrefix + "." + address);
    }

    cluster->clear();

    for (auto const &log_file : log_files) {
        int result = std::remove(log_file.c_str());
        assert(result == 0);
    }
}

} // namespace e8
