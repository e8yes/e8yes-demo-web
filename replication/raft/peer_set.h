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

#ifndef RAFT_PEER_SET_H
#define RAFT_PEER_SET_H

#include <grpcpp/grpcpp.h>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "proto_cc/service_raft.grpc.pb.h"
#include "proto_cc/service_raft.pb.h"
#include "replication/raft/common_types.h"

namespace e8 {

/**
 * @brief The RaftPeerSet class Holds connections to the peer Raft servers.
 */
class RaftPeerSet {
  public:
    /**
     * @brief RaftPeerSet Builds connections from the specified set of machine addresses.
     */
    explicit RaftPeerSet(std::unordered_set<RaftMachineAddress> const &peer_machine_addresses);
    ~RaftPeerSet();

    std::unordered_map<RaftMachineAddress, std::unique_ptr<RaftService::Stub>>::const_iterator
    begin() const;

    std::unordered_map<RaftMachineAddress, std::unique_ptr<RaftService::Stub>>::const_iterator
    end() const;

    /**
     * @brief PeerCount The number of peers.
     */
    unsigned PeerCount() const;

    /**
     * @brief Stub Looks up a peer connection by the specified machine address. The machine address
     * must exists in the peer set. Or else, this function will fail.
     */
    RaftService::Stub *Stub(RaftMachineAddress const &machine_address) const;

  private:
    std::unordered_map<RaftMachineAddress, std::unique_ptr<RaftService::Stub>> peers_;
};

} // namespace e8

#endif // RAFT_PEER_SET_H
