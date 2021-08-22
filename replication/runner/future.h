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

#ifndef REPLICATION_FUTURE_H
#define REPLICATION_FUTURE_H

#include <mutex>
#include <semaphore.h>
#include <string>
#include <utility>

#include "common/time_util/time_util.h"
#include "proto_cc/service_replication.pb.h"

namespace e8 {

/**
 * @brief The ReplicationFuture class It acts initially as a placeholder response object of a
 * request to some computation. Then it may fulfilled subsequently, that is, the requested
 * computation finishes and the result gets put into this object.
 */
class ReplicationFuture {
  public:
    ReplicationFuture();
    ~ReplicationFuture();

    /**
     * @brief FulFill This should only be called at most once, or it will fail. It fulfills the
     * future by assigning the actual return value and error of a computation, allowing the
     * Evaluate() call to finish without timing out.
     *
     * @param return_value The value to be returned to the Evaluate() call.
     * @param err Can't be RCE_TIMED_OUT.
     */
    void FulFill(std::string const &return_value, RunCommandError err);

    /**
     * @brief Fulfilled Checks if the future has been fulfilled.
     */
    bool Fulfilled() const;

    /**
     * @brief Evaluate Waits until timeout or the future is fulfilled and returns the computation
     * result, aka the return value-error pair. If the call was last unblocked by a timeout, the
     * error would be set to RCE_TIMED_OUT and the return value is unusable.
     */
    std::pair<std::string, RunCommandError> Evaluate(TimeIntervalMillis timeout);

  private:
    RunCommandError err_;
    std::string return_value_;
    sem_t signal_;
    std::mutex mu_;
};

} // namespace e8

#endif // REPLICATION_FUTURE_H
