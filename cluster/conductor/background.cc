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

#include <chrono>
#include <memory>
#include <optional>
#include <thread>
#include <vector>

#include "cluster/conductor/background.h"
#include "cluster/conductor/boardcast.h"
#include "cluster/conductor/conductor.h"
#include "cluster/conductor/revision_store.h"
#include "cluster/placement/cluster_map.h"
#include "common/thread/thread_pool.h"
#include "common/time_util/time_util.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/cluster_revision_command.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {
namespace {

TimeIntervalMillis const kWipRevisionPollingInterval = 5000;
float const kRevisionBoardcastRate = 0.01f;
unsigned const kMaxRevisionBoardcastRetries = 5;

} // namespace

ClusterRevisionBackground::ClusterRevisionBackground(
    ClusterRevisionConductorInterface *this_conductor)
    : this_conductor_(this_conductor), done_(false) {}

ClusterRevisionBackground::~ClusterRevisionBackground() {}

void ClusterRevisionBackground::Run(TaskStorageInterface *) const {
    while (!done_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(kWipRevisionPollingInterval));

        if (!this_conductor_->ShouldBoardcast()) {
            continue;
        }

        // Pulls directly from the local store to avoid network & I/O. It's ok to pull an outdated
        // revision because pushing it is harmless. Since we've waited for
        // kWipRevisionPollingInterval after committed an ApplyClusterRevisionCommand, the chance of
        // still pulling an outdated revision locally is low.
        std::optional<ClusterRevisionStore::RevisionSpecs> revision_specs =
            this_conductor_->LocalStore()->WorkInProgress();
        if (!revision_specs.has_value()) {
            continue;
        }

        // Attempts to boardcast the revision.
        std::vector<Machine> unsuccessful_machines;
        bool complete =
            BoardcastRevisionWithRetry(*revision_specs, kRevisionBoardcastRate, *this_conductor_,
                                       kMaxRevisionBoardcastRetries, &unsuccessful_machines);
        if (!complete) {
            continue;
        }

        // TODO: we should report the unsuccessful machines so the error can be triaged and handled.
        // TODO: Excludes failed machines that were handled from the unsuccessful_machines list.
        if (unsuccessful_machines.empty()) {
            // The revision is error-free, meaning the entire cluster is in-sync. We can safely mark
            // it as applied.
            ClusterRevisionCommand apply_revision_command;
            apply_revision_command.set_resource_service_id(revision_specs->resource_service_id);
            *apply_revision_command.mutable_apply_revision()->mutable_revision() =
                revision_specs->WipRevision();
            this_conductor_->RunCommand(apply_revision_command);
        } else {
            // TODO: Makes it possible to let healthy machines receive the latest updates even in
            // face of failures (perhaps apply the current revision as unresolved, and unresolved
            // revision could be retrieved from the WorkInProgress() call).
        }
    }
}

bool ClusterRevisionBackground::DropResourceOnCompletion() const { return false; }

void ClusterRevisionBackground::Shutdown() { done_ = true; }

} // namespace e8
