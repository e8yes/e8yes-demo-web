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

#include <limits>
#include <string>

#include "proto_cc/command.pb.h"
#include "proto_cc/service_replication.pb.h"
#include "replication/raft/journal.h"
#include "replication/runner/command_queue.h"
#include "replication/runner/fulfillment_pool.h"

namespace e8 {

CommandRunnerInterface::CommandRunnerInterface() {}

CommandRunnerInterface::~CommandRunnerInterface() {}

RaftLogOffset CommandRunnerInterface::PreferredSnapshotFrequency() const {
    return std::numeric_limits<RaftLogOffset>::max();
}

void CommandRunnerInterface::Save() const {}

void CommandRunnerInterface::Restore() {}

void CommandRunnerInterface::Reset() {}

CommandQueueProcessor::CommandQueueProcessor(FulfillmentPool *fulfillments,
                                             CommandRunnerInterface *runner)
    : fulfillments_(fulfillments), runner_(runner) {}

CommandQueueProcessor::~CommandQueueProcessor() {}

void CommandQueueProcessor::Apply(CommandEntry const &entry) {
    if (fulfillments_->Fulfilled(entry.run_event_id())) {
        // Skip a duplicate command.
        return;
    }

    std::string return_value = runner_->Run(entry.command());

    fulfillments_->Fulfill(entry.run_event_id(), return_value, RunCommandError::RCE_NONE);
}

RaftLogOffset CommandQueueProcessor::PreferredSnapshotFrequency() const {
    return runner_->PreferredSnapshotFrequency();
}

void CommandQueueProcessor::Save() const { runner_->Save(); }

void CommandQueueProcessor::Restore() { runner_->Restore(); }

} // namespace e8
