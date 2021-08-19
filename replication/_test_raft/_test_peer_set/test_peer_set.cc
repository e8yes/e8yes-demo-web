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

#include <unordered_set>
#include <utility>

#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/service_raft.grpc.pb.h"
#include "proto_cc/service_raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/peer_set.h"

bool PeerSetTest() {
    e8::RaftPeerSet peers(std::unordered_set<e8::RaftMachineAddress>(
                              {"localhost:12345", "localhost:12346", "localhost:12347",
                               "localhost:12348", "localhost:12349"}),
                          /*quorum_size=*/3);

    TEST_CONDITION(peers.PeerCount() == 5);
    TEST_CONDITION(peers.QuorumSize() == 3);
    TEST_CONDITION(peers.Stub("localhost:12345") != nullptr);
    TEST_CONDITION(peers.Stub("localhost:12346") != nullptr);
    TEST_CONDITION(peers.Stub("localhost:12347") != nullptr);
    TEST_CONDITION(peers.Stub("localhost:12348") != nullptr);
    TEST_CONDITION(peers.Stub("localhost:12349") != nullptr);

    std::unordered_set<e8::RaftMachineAddress> iterated_peer_addrs;
    std::unordered_set<e8::RaftService::Stub *> iterated_peer_stubs;
    for (auto const &[peer_addr, peer_stub] : peers) {
        TEST_CONDITION(peer_stub != nullptr);
        iterated_peer_addrs.insert(peer_addr);
        iterated_peer_stubs.insert(peer_stub.get());
    }

    TEST_CONDITION(iterated_peer_stubs.size() == 5);
    TEST_CONDITION(iterated_peer_addrs.size() == 5);
    TEST_CONDITION(iterated_peer_addrs.find("localhost:12345") != iterated_peer_addrs.end());
    TEST_CONDITION(iterated_peer_addrs.find("localhost:12346") != iterated_peer_addrs.end());
    TEST_CONDITION(iterated_peer_addrs.find("localhost:12347") != iterated_peer_addrs.end());
    TEST_CONDITION(iterated_peer_addrs.find("localhost:12348") != iterated_peer_addrs.end());
    TEST_CONDITION(iterated_peer_addrs.find("localhost:12349") != iterated_peer_addrs.end());

    return true;
}

int main() {
    e8::BeginTestSuite("raft_peer_set");
    e8::RunTest("PeerSetTest", PeerSetTest);
    e8::EndTestSuite();
    return 0;
}
