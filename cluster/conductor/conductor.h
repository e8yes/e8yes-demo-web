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

#ifndef CLUSTER_CONDUCTOR_H
#define CLUSTER_CONDUCTOR_H

#include <optional>

#include "cluster/conductor/client.h"
#include "proto_cc/cluster_revision_command.pb.h"
#include "replication/runner/run_event_id.h"
#include "replication/runner/runner.h"

namespace e8 {

/**
 * @brief The ClusterRevisionConductorInterface class Defines the functions a revision conductor
 * must provide.
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
     * @brief RunCommand Runs the specified command on the local revision conductor instance. This
     * function blocks until the command is committed and gets processed by a majority of conductor
     * instance's revision work pool. Howver, if the local conductor instance isn't an active (loses
     * leader status or gets shut down) boardcastor, it will return an std::nullopt.
     */
    virtual std::optional<ClusterRevisionResult>
    RunCommand(ClusterRevisionCommand const &command) = 0;

    /**
     * @brief Shutdown Unblocks all the currently running command. If the command hasn't returned in
     * time, the RunCommand() function will return an std::nullopt. This function should only be
     * used once.
     */
    virtual void Shutdown() = 0;
};

/**
 * @brief The ClusterRevisionConductor A revision conductor that can potentially run in replicas. It
 * conducts the boardcast of cluster revision messages to the cluster in a fault-tolerant manner.
 */
class ClusterRevisionConductor : public ClusterRevisionConductorInterface {
  public:
    ClusterRevisionConductor(ReplicationInstance *conductor_replicator);
    ~ClusterRevisionConductor() override;

    bool ShouldBoardcast() const override;

    std::optional<ClusterRevisionResult> RunCommand(ClusterRevisionCommand const &command) override;

    void Shutdown() override;

  private:
    ReplicationInstance *replicated_conductor_;
    ReplicationRunEventIdGenerator run_event_id_gen_;
    bool running_;
};

} // namespace e8

#endif // CLUSTER_CONDUCTOR_H
