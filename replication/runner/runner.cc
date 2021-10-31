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

#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "common/time_util/time_util.h"
#include "proto_cc/command.pb.h"
#include "proto_cc/replication.pb.h"
#include "proto_cc/service_replication.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/raft_instance.h"
#include "replication/runner/command_queue.h"
#include "replication/runner/fulfillment_pool.h"
#include "replication/runner/future.h"
#include "replication/runner/run_event_id.h"
#include "replication/runner/runner.h"

namespace e8 {

ReplicationInstance::ReplicationInstance(CommandRunnerInterface *runner,
                                         ReplicationConfig const &config)
    : fulfillments_(std::make_unique<FulfillmentPool>()),
      processor_(std::make_unique<CommandQueueProcessor>(fulfillments_.get(), runner)),
      raft_(std::make_unique<RaftInstance>(processor_.get(), config.raft_config())),
      fulfillment_timeout_(config.fulfillment_timeout_millis()) {}

ReplicationInstance::~ReplicationInstance() {}

ReplicationInstance::RunCommandResult::RunCommandResult() {}

ReplicationInstance::RunCommandResult::RunCommandResult(
    RunCommandError error, std::optional<RaftMachineAddress> retry_node)
    : error(error), retry_node(retry_node) {}

ReplicationInstance::RunCommandResult::RunCommandResult(std::string return_value,
                                                        RunCommandError error)
    : error(error), return_value(return_value) {}

ReplicationInstance::RunCommandResult
ReplicationInstance::RunCommand(std::string const &command,
                                ReplicationRunEventId const &run_event_id) {
    ReplicationFuture *future = fulfillments_->NewRunEvent(run_event_id);
    if (future->Fulfilled()) {
        auto [return_value, error] = future->Evaluate(fulfillment_timeout_);
        return RunCommandResult(return_value, error);
    }

    CommandEntry command_entry;
    command_entry.set_run_event_id(run_event_id);
    command_entry.set_command(command);

    RaftForeground::BoardcastResult boardcast_result =
        raft_->Foreground()->BoardcastCommand(command_entry);
    if (boardcast_result.retry_node.has_value()) {
        return RunCommandResult(RunCommandError::RCE_WRONG_NODE, boardcast_result.retry_node);
    }

    auto [return_value, error] = future->Evaluate(fulfillment_timeout_);
    return RunCommandResult(return_value, error);
}

bool ReplicationInstance::Leader() const {
    auto [_, role] = raft_->Foreground()->TermAndRole();
    return role == RAFT_LEADER;
}

} // namespace e8
