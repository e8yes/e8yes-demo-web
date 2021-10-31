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
#include <optional>
#include <thread>

#include "cluster/conductor/background.h"
#include "cluster/conductor/boardcast.h"
#include "cluster/conductor/conductor.h"
#include "cluster/conductor/revision_work_crud.h"
#include "common/random/random_source.h"
#include "common/thread/thread_pool.h"
#include "common/time_util/time_util.h"
#include "proto_cc/cluster_revision.pb.h"
#include "proto_cc/cluster_revision_command.pb.h"

namespace e8 {
namespace {

TimeIntervalMillis const kWipRevisionPollingInterval = 5000;
float const kRevisionBoardcastRate = 0.01f;
unsigned const kMaxRevisionBoardcastRetries = 5;

} // namespace

ClusterConductorBackground::ClusterConductorBackground(
    ClusterRevisionConductorInterface *this_conductor)
    : this_conductor_(this_conductor), random_source_(std::make_unique<RandomSource>()),
      done_(false) {}

ClusterConductorBackground::~ClusterConductorBackground() {}

void ClusterConductorBackground::Run(TaskStorageInterface *) const {
    while (!done_) {
        if (!this_conductor_->ShouldBoardcast()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(kWipRevisionPollingInterval));
            continue;
        }

        //
        std::optional<PollPendingRevisionResult> poll_pending_result =
            PollPendingRevision(this_conductor_, random_source_.get());
        if (poll_pending_result.has_value()) {
            CreateNewRevisionWork(poll_pending_result->resource_service_id, this_conductor_,
                                  &poll_pending_result->pending_revision,
                                  &poll_pending_result->all_revision);
        }

        std::optional<SelectRevisionWorkResult> select_work_result =
            SelectRevisionWork(this_conductor_, random_source_.get());
        if (!select_work_result.has_value()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(kWipRevisionPollingInterval));
            continue;
        }

        // Attempts to boardcast the revision.
        ClusterRevisionWork leftover_work;
        bool complete = BoardcastRevisionWithRetry(
            select_work_result->selected_work, select_work_result->all_revisions,
            kRevisionBoardcastRate, *this_conductor_, kMaxRevisionBoardcastRetries, &leftover_work);
        if (!complete) {
            continue;
        }

        if (leftover_work.targets().empty()) {
            // The revision work's boardcast is error-free, meaning the entire cluster is in-sync.
            // We can safely mark it as finished.
            FinishRevisionWork(select_work_result->resource_service_id,
                               select_work_result->selected_work, this_conductor_);
        } else {
            // The revision isn't completely done. Some machine targets failed to acknowledge the
            // change. We'll put these machine targets back to the work and retry the next time the
            // work is selected.
            UpdateRevisionWork(select_work_result->resource_service_id, leftover_work,
                               this_conductor_);

            // TODO: we should report the unsuccessful machines so the error can be triaged and
            // handled.
            // TODO: Excludes failed machines that were handled from the unsuccessful_machines list.
        }
    }
}

bool ClusterConductorBackground::DropResourceOnCompletion() const { return false; }

void ClusterConductorBackground::Shutdown() {
    done_ = true;
    this_conductor_->Shutdown();
}

} // namespace e8
