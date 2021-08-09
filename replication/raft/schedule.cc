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
#include <chrono>
#include <semaphore.h>
#include <thread>
#include <time.h>
#include <vector>

#include "common/random/random_source.h"
#include "common/random/uniform_distribution.h"
#include "common/time_util/time_util.h"
#include "replication/raft/schedule.h"

namespace e8 {

RaftScheduleConfig FastElectionRaftScheduleConfig(float scale) {
    RaftScheduleConfig config;
    config.heartbeat_millis = 100 * scale;
    config.heartbeat_min_timeout_millis = 500 * scale;
    config.heartbeat_max_timeout_millis = 1200 * scale;
    config.election_timeout_millis = 400;
    return config;
}

FollowerSchedule::FollowerSchedule(RaftScheduleConfig const &config)
    : ticks_(0), config_(config), random_source_(std::make_unique<RandomSource>(/*seed=*/137)) {}

FollowerSchedule::~FollowerSchedule() {}

bool FollowerSchedule::WaitForHeartbeat() const {
    unsigned old_ticks = ticks_;

    std::vector<int> timeout_millis =
        DrawUniformInts(config_.heartbeat_min_timeout_millis, config_.heartbeat_max_timeout_millis,
                        /*num_instances=*/1, random_source_.get());
    assert(timeout_millis.size() == 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout_millis[0]));

    return ticks_ > old_ticks;
}

void FollowerSchedule::ConfirmHeartbeat() { ++ticks_; }

CandidateSchedule::CandidateSchedule(RaftScheduleConfig const &config) : config_(config) {}

CandidateSchedule::~CandidateSchedule() {}

void CandidateSchedule::WaitForElectionResult() {
    std::this_thread::sleep_for(std::chrono::milliseconds(config_.election_timeout_millis));
}

LeaderSchedule::LeaderSchedule(RaftScheduleConfig const &config) : config_(config) {}

LeaderSchedule::~LeaderSchedule() {}

void LeaderSchedule::WakeUp() { sem_post(&tasks_); }

void LeaderSchedule::Sleep() {
    timespec ts = FutureTimeSpec(config_.heartbeat_millis);
    sem_timedwait(&tasks_, &ts);

    // All previously added tasks will be collectively processed. It's ok to empty the entire task
    // queue.
    sem_init(&tasks_, /*__pshared=*/0, /*__value=*/0);
}

} // namespace e8
