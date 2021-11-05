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

#ifndef CLUSTER_TASK_MANAGER_PID_SLOTS_H
#define CLUSTER_TASK_MANAGER_PID_SLOTS_H

#include <mutex>
#include <optional>
#include <unistd.h>
#include <unordered_map>

#include "cluster/task_manager/common_types.h"

namespace e8 {

/**
 * @brief The ProcessIdSlots class Keeps track of what task is believed to be currently occupying
 * the system's process ID. This class is a lockable device.
 */
class ProcessIdSlots {
  public:
    ProcessIdSlots();
    ~ProcessIdSlots();

    // For guarding this class. Guards this class before using the other functions.
    void lock();
    void unlock();

    /**
     * @brief UseSlot Swaps what was in the specified process ID slot with the new task ID and
     * returns the old ID.
     *
     * @param pid The system process ID slot where the new task occupies.
     * @param new_task_id ID of the new task that is occupying the pid.
     * @return ID of the old task (must have terminated) that was using the pid if there is any.
     */
    std::optional<LocalTaskId> UseSlot(pid_t pid, LocalTaskId const &new_task_id);

    /**
     * @brief ClearSlot Removes and returns what is occupying the specified process ID.
     *
     * @param pid The system process ID slot to be cleared.
     * @return ID of the old task (must have terminated) that was using the pid if there is any.
     */
    std::optional<LocalTaskId> ClearSlot(pid_t pid);

    /**
     * @brief All Returns all the process IDs that are believed to be in-use and the ID of the
     * corresponding tasks which occupy those process IDs.
     */
    std::unordered_map<pid_t, LocalTaskId> All() const;

  private:
    std::unordered_map<pid_t, LocalTaskId> slots_;
    std::mutex mu_;
};

} // namespace e8

#endif // CLUSTER_TASK_MANAGER_PID_SLOTS_H
