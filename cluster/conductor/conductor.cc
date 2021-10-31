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
#include <string>

#include "cluster/conductor/client.h"
#include "cluster/conductor/conductor.h"
#include "proto_cc/cluster_conductor_command.pb.h"
#include "proto_cc/cluster_revision_command.pb.h"
#include "replication/runner/runner.h"

namespace e8 {

ClusterRevisionConductorInterface::ClusterRevisionConductorInterface() {}

ClusterRevisionConductorInterface::~ClusterRevisionConductorInterface() {}

ClusterRevisionConductor::ClusterRevisionConductor(ReplicationInstance *conductor_replicator,
                                                   ClusterConductorClient *conductor_client)
    : conductor_replicator_(conductor_replicator), conductor_client_(conductor_client) {}

ClusterRevisionConductor::~ClusterRevisionConductor() {}

bool ClusterRevisionConductor::ShouldBoardcast() const { return conductor_replicator_->Leader(); }

ClusterRevisionResult ClusterRevisionConductor::RunCommand(ClusterRevisionCommand const &command) {
    ClusterConductorCommand conductor_command;
    *conductor_command.mutable_revision() = command;

    ClusterConductorCommandResult result = conductor_client_->RunCommand(conductor_command);
    assert(result.has_revision_result());
    return result.revision_result();
}

} // namespace e8
