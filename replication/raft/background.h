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

#ifndef RAFT_BACKGROUND_H
#define RAFT_BACKGROUND_H

#include "common/thread/thread_pool.h"
#include "replication/raft/context.h"

namespace e8 {

/**
 * @brief The RaftBackground class A bockground running task to maintain the node's current raft
 * role and to start up a Raft server to serve RPCs.
 *
 * The following describes what this background task would do when the node has transitioned into
 * each role, respectively:
 *
 * Follower:
 *  Detects the absence of heartbeat and transition into a candidate.
 *
 * Candidate:
 * 	Conducts election until it finds other leaders or win an election, then it sets itself to a
 * follower or leader respectively.
 *
 * Leader:
 *  Constantly publishing and committing logs as well as sending heartbeats to the followers.\
 *
 * Please look at the service_raft.proto for information about what RPCs the Raft werver provides.
 *
 * Note, never run more than one background task on the same context.
 */
class RaftBackground : public TaskInterface {
  public:
    RaftBackground(RaftContext *context);
    virtual ~RaftBackground() override;

    /**
     * @brief Shutdown Signals to the Raft role maintenance cycle as well as the Raft server.
     */
    void Shutdown();

    /**
     * @brief Run This function blocks until Shutdown() is called. No task data is required.
     */
    void Run(TaskStorageInterface *) const override;

    bool DropResourceOnCompletion() const override;

  private:
    RaftContext *context_;
    bool done_;
};

} // namespace e8

#endif // RAFT_BACKGROUND_H
