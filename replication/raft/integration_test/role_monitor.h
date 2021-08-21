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

#ifndef RAFT_ROLE_MONITOR_H
#define RAFT_ROLE_MONITOR_H

#include <optional>

#include "common/thread/thread_pool.h"
#include "replication/raft/common_types.h"
#include "replication/raft/integration_test/local_cluster.h"

namespace e8 {

namespace role_monitor_internal {

class RaftRoleMonitorImpl;

} // namespace role_monitor_internal

/**
 * @brief The RaftRoleMonitor class Constantly monitoring the nodes' role at each term in the
 * background. It will panic when it sees if there has been ever multiple leaders in any term. It
 * also provides functions to retrieve the latest leader node if there is any.
 */
class RaftRoleMonitor {
  public:
    /**
     * @brief RaftRoleMonitor Specifies the cluster to monitor for, then starts up the background
     * monitoring task.
     */
    explicit RaftRoleMonitor(LocalRaftCluster const *cluster);
    ~RaftRoleMonitor();

    /**
     * @brief FindOneRaftLeaeder Returns the leader node's address at the latest term, if there is
     * any. If the monitor can't spot a leader in 2 maximum election timeout periods, it returns a
     * std::nullopt.
     */
    std::optional<RaftMachineAddress> FindOneRaftLeaeder();

  private:
    std::shared_ptr<role_monitor_internal::RaftRoleMonitorImpl> pimpl_;

    ThreadPool monitor_thread_;
};

} // namespace e8

#endif // RAFT_ROLE_MONITOR_H
