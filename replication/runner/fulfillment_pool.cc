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
#include <memory>
#include <mutex>
#include <unordered_map>

#include "proto_cc/service_replication.pb.h"
#include "replication/runner/common_types.h"
#include "replication/runner/fulfillment_pool.h"
#include "replication/runner/future.h"

namespace e8 {

FulfillmentPool::FulfillmentPool() : mu_(std::make_unique<std::mutex>()) {}

FulfillmentPool::~FulfillmentPool() {}

ReplicationFuture *FulfillmentPool::NewRunEvent(ReplicationRunEventId const &run_event_id) {
    std::lock_guard guard(*mu_);

    auto it = futures_.find(run_event_id);
    if (it == futures_.end()) {
        auto future = std::make_unique<ReplicationFuture>();
        it = futures_.insert(std::make_pair(run_event_id, std::move(future))).first;
    }

    return it->second.get();
}

void FulfillmentPool::Fulfill(ReplicationRunEventId const &run_event_id,
                              std::string const &return_value, RunCommandError err) {
    ReplicationFuture *future = this->NewRunEvent(run_event_id);
    future->FulFill(return_value, err);
}

bool FulfillmentPool::Fulfilled(ReplicationRunEventId const &run_event_id) const {
    std::lock_guard guard(*mu_);

    auto it = futures_.find(run_event_id);
    if (it == futures_.end()) {
        return false;
    }

    auto const &[_, future] = *it;
    return future->Fulfilled();
}

void FulfillmentPool::Clear() {
    std::lock_guard guard(*mu_);
    futures_.clear();
}

} // namespace e8
