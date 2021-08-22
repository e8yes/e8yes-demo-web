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

#ifndef REPLICATION_FULFILLMENT_POOL_H
#define REPLICATION_FULFILLMENT_POOL_H

#include <memory>
#include <mutex>
#include <unordered_map>

#include "proto_cc/service_replication.pb.h"
#include "replication/runner/common_types.h"
#include "replication/runner/future.h"

namespace e8 {

/**
 * @brief The FulfillmentPool class Keeps a collection of fulfillment tasks keyed by the
 * corresponding ID of the run event which started the task. This ensures that a fulfillment applies
 * to a specific run event exactly once. This data structure is thread safe.
 */
class FulfillmentPool {
  public:
    FulfillmentPool();
    ~FulfillmentPool();

    /**
     * @brief NewRunEvent Attempts to start a new run event keyed by the specified unique
     * run_event_id. If a run event has been started before, it returns the old run event object
     * instead. A run event object is a fulfilment task. This class holds the fulfillment tasks
     * regardless of their fulfillment status unless they are cleared out by explicitly calling the
     * Clear() function.
     */
    ReplicationFuture *NewRunEvent(RunEventId const &run_event_id);

    /**
     * @brief Fulfill Makes the task associated with the run event fulfill. Once a call event is
     * fulfilled, it can't be undone ever since. In addition, every run event can only fulfill at
     * most once, or this function will fail.
     *
     * @param run_event_id ID of the run event to be fulfilled.
     * @param return_value The result of the computation task.
     * @param err Error arisen from the computation task.
     */
    void Fulfill(RunEventId const &run_event_id, std::string const &return_value,
                 RunCommandError err);

    /**
     * @brief Fulfilled Checks if the fulfillment task associated with the run event had fulfilled.
     */
    bool Fulfilled(RunEventId const &run_event_id) const;

    /**
     * @brief Clear Clears all the fulfillment tasks. The Clear() call should not create any race
     * conditions with other functions in order for the class to function correctly, though failing
     * to do so will not make any class member fails.
     */
    void Clear();

  private:
    std::unordered_map<RunEventId, std::unique_ptr<ReplicationFuture>> futures_;
    std::unique_ptr<std::mutex> mu_;
};

} // namespace e8

#endif // REPLICATION_FULFILLMENT_POOL_H
