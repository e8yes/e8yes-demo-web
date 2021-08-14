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

#ifndef RAFT_SCHEDULE_H
#define RAFT_SCHEDULE_H

#include <memory>
#include <semaphore.h>

#include "common/random/random_source.h"
#include "common/time_util/time_util.h"

namespace e8 {

/**
 * @brief The RaftScheduleConfig struct Defines how fast should Raft peers detect leader
 * unavaliability and complete an election.
 */
struct RaftScheduleConfig {
    // How often should the leader send out a heartbeat?
    TimeIntervalMillis heartbeat_millis;

    // The minimum and maximum number of milliseconds of which heartbeat is absent before
    // determining that the leader is unavailable. heartbeat_max_timeout_millis is exclusive.
    TimeIntervalMillis heartbeat_min_timeout_millis;
    TimeIntervalMillis heartbeat_max_timeout_millis;

    // The number of milliseconds within which an election must finish.
    TimeIntervalMillis election_timeout_millis;
};

/**
 * @brief FastElectionRaftScheduleConfig Creates a schedule pre-set that will do a decent job of
 * bringing up a leader quickly and keeping the role unchanged for a long time.
 *
 * @param scale Any scalar number >= 1. This will scale up the heartbeat interval to reduce
 * resource consumption, but will also lengthen the amount of time to learn the availability of the
 * leader.
 */
RaftScheduleConfig FastElectionRaftScheduleConfig(float scale = 1.0f);

/**
 * @brief The FollowerSchedule class Helps keep a Raft follower in schedule.
 */
class FollowerSchedule {
  public:
    FollowerSchedule(RaftScheduleConfig const &config);
    ~FollowerSchedule();

    /**
     * @brief WaitForHeartbeat Waits for a number milliseconds set between
     * [config.heartbeat_min_timeout_millis, config.heartbeat_max_timeout_millis) and checks if
     * the node receives at least one heartbeat.
     */
    bool WaitForHeartbeat() const;

    /**
     * @brief ConfirmHeartbeat Whenever the node receives a heartbeat, this should be called.
     */
    void ConfirmHeartbeat();

  private:
    unsigned ticks_;
    RaftScheduleConfig config_;
    std::unique_ptr<RandomSource> random_source_;
};

/**
 * @brief The CandidateSchedule class Helps keep a Raft candidate in schedule.
 */
class CandidateSchedule {
  public:
    CandidateSchedule(RaftScheduleConfig const &config);
    ~CandidateSchedule();

    /**
     * @brief WaitForElectionResult Wait for config.election_timeout_millis number of milliseconds.
     * This should be called before checking the election result.
     */
    void WaitForElectionResult();

  private:
    RaftScheduleConfig config_;
};

/**
 * @brief The LeaderSchedule class Helps keep a Raft candidate in schedule.
 */
class LeaderSchedule {
  public:
    LeaderSchedule(RaftScheduleConfig const &config);
    ~LeaderSchedule();

    /**
     * @brief WakeUp Wakes up to perform another log agreement cycle.
     */
    void WakeUp();

    /**
     * @brief BeginWork Notifies the schedule that the caller has started a new leader cycle. The
     * caller should end the leader cycle by calling the Sleep() function. The amount of sleep will
     * be deducted by the amount spent in work, that is, the time spent between BeginWork() and
     * Sleep().
     */
    void BeginWork();

    /**
     * @brief Sleep This should be called in each log agreement cycle to save computation resource
     * when idle.
     */
    void Sleep();

  private:
    RaftScheduleConfig config_;
    sem_t tasks_;
    TimestampMillis begin_work_timestamp_millis_;
};

} // namespace e8

#endif // RAFT_SCHEDULE_H
