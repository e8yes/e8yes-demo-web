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
#include <mutex>
#include <semaphore.h>
#include <string>
#include <utility>

#include "common/time_util/time_util.h"
#include "proto_cc/service_replication.pb.h"
#include "replication/runner/future.h"

namespace e8 {

ReplicationFuture::ReplicationFuture() : err_(RunCommandError::RCE_TIMED_OUT) {
    sem_init(&signal_, /*__pshared=*/0, /*__value=*/0);
}

ReplicationFuture::~ReplicationFuture() {}

void ReplicationFuture::FulFill(std::string const &return_value, RunCommandError err) {
    {
        std::lock_guard guard(mu_);

        assert(!this->Fulfilled());

        return_value_ = return_value;
        err_ = err;
    }

    sem_post(&signal_);
}

bool ReplicationFuture::Fulfilled() const { return err_ != RunCommandError::RCE_TIMED_OUT; }

std::pair<std::string, RunCommandError> ReplicationFuture::Evaluate(TimeIntervalMillis timeout) {
    timespec ts = FutureTimeSpec(timeout);
    sem_timedwait(&signal_, &ts);

    std::lock_guard guard(mu_);
    return std::make_pair(return_value_, err_);
}

} // namespace e8
