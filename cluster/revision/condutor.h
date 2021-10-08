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

#ifndef CLUSTER_REVISION_CONDUTOR_H
#define CLUSTER_REVISION_CONDUTOR_H

#include "cluster/revision/store.h"
#include "proto_cc/cluster_revision_command.pb.h"

namespace e8 {

/**
 * @brief The ClusterRevisionConductorInterface class Defines the functions a revision conductor
 * must provide. A revision conductor can potentially run in replicas. It conducts the boardcast of
 * cluster revision messages to the cluster in a fault-tolerant manner.
 */
class ClusterRevisionConductorInterface {
  public:
    ClusterRevisionConductorInterface();
    virtual ~ClusterRevisionConductorInterface();

    /**
     * @brief ShouldBoardcast If the revision conductor runs in replicas, only one instance should
     * be allow to boardcast the revision during most of the period to save communication bandwidth,
     * though it's ok to briefly have multiple boardcaster.
     */
    virtual bool ShouldBoardcast() const = 0;

    /**
     * @brief RunCommand Runs the specified command over all the revision conductors, if there are
     * multiple. This function blocks until the command is committed and gets processed by a
     * majority of command runners.
     */
    virtual ClusterRevisionResult RunCommand(ClusterRevisionCommand const &command) = 0;

    /**
     * @brief LocalStore Returns a constant pointer to the local revision store for querying
     * purposes. This allows the client of the conductor to bypass Raft's logging mechanism to read
     * the store's states when it doesn't care if a state gets updated by previous write operations.
     */
    virtual ClusterRevisionStore const *LocalStore();
};

} // namespace e8

#endif // CLUSTER_REVISION_CONDUTOR_H
