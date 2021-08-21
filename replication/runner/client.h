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

#ifndef REPLICATION_RUNNER_CLIENT_H
#define REPLICATION_RUNNER_CLIENT_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "common/random/random_source.h"
#include "common/time_util/time_util.h"
#include "proto_cc/service_replication.grpc.pb.h"
#include "proto_cc/service_replication.pb.h"
#include "replication/raft/common_types.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {

/**
 * @brief The ReplicationConfig struct Tells the client the peer addresses that will respond to the
 * RunCommand() RPC.
 */
struct ReplicationConfig {
    std::unordered_set<RaftMachineAddress> peers;
};

/**
 * @brief The ReplicationClient class The client to interacts with the replication service. It
 * encapsulates all the error handling of a CP system.
 */
class ReplicationClient {
  public:
    ReplicationClient(ReplicationConfig const &config);
    ~ReplicationClient();

    /**
     * @brief RunCommand Replicates and runs the specified command amount the peers. This function
     * blocks indefinitely until one of the peers returns an error-free response.
     *
     * @return The value returns by the command.
     */
    std::string RunCommand(std::string const &command);

  private:
    std::unordered_map<RaftMachineAddress, std::unique_ptr<ReplicationService::Stub>> peer_stubs_;
    std::unordered_map<RaftMachineAddress, float> peer_weights_;
    RandomSource random_source_;
    RaftMachineAddress possible_leader_;
    UUID4_STATE_T uuid_state_;
};

} // namespace e8

#endif // REPLICATION_RUNNER_CLIENT_H
