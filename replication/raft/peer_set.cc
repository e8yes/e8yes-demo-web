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
#include <grpcpp/grpcpp.h>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "proto_cc/service_raft.grpc.pb.h"
#include "proto_cc/service_raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/peer_set.h"

namespace e8 {

RaftPeerSet::RaftPeerSet(std::unordered_set<RaftMachineAddress> const &peer_machine_addresses) {
    for (auto const &peer_address : peer_machine_addresses) {
        auto peer_channel = grpc::CreateChannel(peer_address, grpc::InsecureChannelCredentials());
        auto stub = RaftService::NewStub(peer_channel);
        peers_.insert(std::make_pair(peer_address, std::move(stub)));
    }
}

RaftPeerSet::~RaftPeerSet() {}

unsigned RaftPeerSet::PeerCount() const { return peers_.size(); }

RaftService::Stub *RaftPeerSet::Stub(RaftMachineAddress const &machine_address) const {
    auto it = peers_.find(machine_address);
    assert(it != peers_.end());
    return it->second.get();
}

std::unordered_map<RaftMachineAddress, std::unique_ptr<RaftService::Stub>>::const_iterator
RaftPeerSet::begin() const {
    return peers_.begin();
}

std::unordered_map<RaftMachineAddress, std::unique_ptr<RaftService::Stub>>::const_iterator
RaftPeerSet::end() const {
    return peers_.end();
}

} // namespace e8
