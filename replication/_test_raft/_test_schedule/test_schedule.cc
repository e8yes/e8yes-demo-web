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
#include <cmath>
#include <thread>

#include "common/time_util/time_util.h"
#include "common/unit_test_util/unit_test_util.h"
#include "replication/raft/schedule.h"

namespace {

e8::RaftScheduleConfig TestConfig() {
    e8::RaftScheduleConfig config;

    config.heartbeat_millis = e8::TimeIntervalMillis(500);
    config.election_timeout_millis = e8::TimeIntervalMillis(400);
    config.heartbeat_min_timeout_millis = e8::TimeIntervalMillis(800);
    config.heartbeat_max_timeout_millis = e8::TimeIntervalMillis(1000);

    return config;
}

void SendHeartbeatFn(e8::FollowerSchedule *schedule, e8::TimeIntervalMillis after) {
    std::this_thread::sleep_for(std::chrono::milliseconds(after));
    schedule->ConfirmHeartbeat();
}

void WakeUpLeaderFn(e8::LeaderSchedule *schedule, e8::TimeIntervalMillis after) {
    std::this_thread::sleep_for(std::chrono::milliseconds(after));
    schedule->WakeUp();
}

} // namespace

bool FollowerScheduleTest() {
    e8::RaftScheduleConfig config = TestConfig();

    e8::FollowerSchedule schedule(config);
    TEST_CONDITION(schedule.WaitForHeartbeat() == false);

    std::thread heartbeat_thread(SendHeartbeatFn, &schedule, /*after=*/e8::TimeIntervalMillis(500));
    TEST_CONDITION(schedule.WaitForHeartbeat() == true);
    heartbeat_thread.join();

    return true;
}

bool CandidateScheduleTest() {
    e8::RaftScheduleConfig config = TestConfig();

    e8::CandidateSchedule schedule(config);

    e8::TimestampMillis start = e8::CurrentTimestampMillis();
    schedule.WaitForElectionResult();
    e8::TimestampMillis end = e8::CurrentTimestampMillis();

    TEST_CONDITION(std::abs(end - start - config.election_timeout_millis) <
                   e8::TimeIntervalMillis(10));

    return true;
}

bool LeaderScheduleTest() {
    e8::RaftScheduleConfig config = TestConfig();

    e8::LeaderSchedule schedule(config);

    schedule.BeginWork();

    // Do some work.
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    e8::TimestampMillis start = e8::CurrentTimestampMillis();
    schedule.Sleep();
    e8::TimestampMillis end = e8::CurrentTimestampMillis();

    TEST_CONDITION(std::abs(end - start - (config.heartbeat_millis - 200)) <
                   e8::TimeIntervalMillis(10));

    return true;
}

bool LeaderScheduleWakeupTest() {
    e8::RaftScheduleConfig config = TestConfig();

    e8::LeaderSchedule schedule(config);

    std::thread wakeup_thread(WakeUpLeaderFn, &schedule, /*after=*/e8::TimeIntervalMillis(100));

    schedule.BeginWork();
    e8::TimestampMillis start = e8::CurrentTimestampMillis();
    schedule.Sleep();
    e8::TimestampMillis end = e8::CurrentTimestampMillis();

    wakeup_thread.join();

    TEST_CONDITION(std::abs(end - start - 100) < e8::TimeIntervalMillis(10));

    return true;
}

int main() {
    e8::BeginTestSuite("raft_schedule");
    e8::RunTest("FollowerScheduleTest", FollowerScheduleTest);
    e8::RunTest("CandidateScheduleTest", CandidateScheduleTest);
    e8::RunTest("LeaderScheduleTest", LeaderScheduleTest);
    e8::RunTest("LeaderScheduleWakeupTest", LeaderScheduleTest);
    e8::EndTestSuite();
    return 0;
}
