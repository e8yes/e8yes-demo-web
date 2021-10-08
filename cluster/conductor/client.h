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

#ifndef CLUSTER_CONDUCTOR_CLIENT_H
#define CLUSTER_CONDUCTOR_CLIENT_H

#include "proto_cc/cluster_conductor_command.pb.h"
#include "replication/runner/client.h"

namespace e8 {

/**
 * @brief The ClusterConductorClient class The client to interact with the replicated
 * ClusterConductorInstance.
 */
class ClusterConductorClient {
  public:
    ClusterConductorClient(ReplicationClientConfig const &config);

    /**
     * @brief RunCommand Runs the specified command on the replicated ClusterConductorInstance. It
     * will block until the specified conductor command is committed.
     */
    ClusterConductorCommandResult RunCommand(ClusterConductorCommand const &command);

  private:
    ReplicationClient replication_client_;
};

} // namespace e8

#endif // CLUSTER_CONDUCTOR_CLIENT_H
