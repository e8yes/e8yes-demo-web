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

#ifndef REPLICATION_RUNNER_H
#define REPLICATION_RUNNER_H

#include <memory>
#include <optional>
#include <string>

#include "common/time_util/time_util.h"
#include "proto_cc/replication.pb.h"
#include "proto_cc/service_replication.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/raft_instance.h"
#include "replication/runner/command_queue.h"
#include "replication/runner/common_types.h"
#include "replication/runner/fulfillment_pool.h"

namespace e8 {

/**
 * @brief The ReplicationInstance class Connects low-level objects that implements the command
 * replication and running. It also manages the life cycle of those objects and background threads.
 * It exposes an easy-to-use function to run a command consistently among the peers.
 */
class ReplicationInstance {
  public:
    /**
     * @brief ReplicationInstance Specifies the custom runner used to run the committed commands as
     * well as the configuration.
     */
    ReplicationInstance(CommandRunnerInterface *runner, ReplicationConfig const &config);
    ~ReplicationInstance();

    /**
     * @brief The RunCommandResult struct Returns by the RunCommand() call.
     */
    struct RunCommandResult {
        RunCommandResult(RunCommandError error, std::optional<RaftMachineAddress> retry_node);
        RunCommandResult(RunCommandError error, std::string return_value);

        // Error status. Retry the command on the same node when this field has value RCE_TIMED_OUT.
        RunCommandError error;

        // Use the address to retry the command when the error field has the value RCE_WRONG_NODE.
        std::optional<RaftMachineAddress> retry_node;

        // Only read this value when the error field has the value RCE_WRONG_NODE.
        std::string return_value;
    };

    /**
     * @brief RunCommand Runs a command across the peers. The command should be uniquely identified
     * by its run_event_id, so it will run at exactly once if the call succeed and none the
     * otherwise. This function will only succeed when the underlying Raft instance currently has
     * the leader status.
     */
    RunCommandResult RunCommand(std::string const &command,
                                ReplicationRunEventId const &run_event_id);

  private:
    std::unique_ptr<FulfillmentPool> fulfillments_;
    std::unique_ptr<CommandQueueProcessor> processor_;
    std::unique_ptr<RaftInstance> raft_;
    TimeIntervalMillis const fulfillment_timeout_;
};

} // namespace e8

#endif // REPLICATION_RUNNER_H
