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
#include <optional>
#include <string>

#include "cluster/conductor/client.h"
#include "cluster/conductor/conductor.h"
#include "proto_cc/cluster_conductor_command.pb.h"
#include "proto_cc/cluster_revision_command.pb.h"
#include "proto_cc/service_replication.pb.h"
#include "replication/runner/run_event_id.h"
#include "replication/runner/runner.h"

namespace e8 {

ClusterRevisionConductorInterface::ClusterRevisionConductorInterface() {}

ClusterRevisionConductorInterface::~ClusterRevisionConductorInterface() {}

ClusterRevisionConductor::ClusterRevisionConductor(ReplicationInstance *conductor_replicator)
    : replicated_conductor_(conductor_replicator), running_(true) {}

ClusterRevisionConductor::~ClusterRevisionConductor() {}

bool ClusterRevisionConductor::ShouldBoardcast() const { return replicated_conductor_->Leader(); }

std::optional<ClusterRevisionResult>
ClusterRevisionConductor::RunCommand(ClusterRevisionCommand const &command) {
    ClusterConductorCommand conductor_command;
    *conductor_command.mutable_revision() = command;

    ReplicationRunEventId run_event_id = run_event_id_gen_.Generate();
    ReplicationInstance::RunCommandResult run_command_result;

    do {
        run_command_result =
            replicated_conductor_->RunCommand(conductor_command.SerializeAsString(), run_event_id);
    } while (running_ && run_command_result.error == RunCommandError::RCE_TIMED_OUT);

    if (run_command_result.error != RunCommandError::RCE_NONE) {
        return std::nullopt;
    }

    ClusterRevisionResult result;
    result.ParseFromString(run_command_result.return_value);

    return result;
}

void ClusterRevisionConductor::Shutdown() { running_ = false; }

} // namespace e8
