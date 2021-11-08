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

#ifndef CLUSTER_TASK_MANAGER_HISTORY_H
#define CLUSTER_TASK_MANAGER_HISTORY_H

#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "cluster/task_manager/common_types.h"
#include "proto_cc/task.pb.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {

/**
 * @brief The TaskHistoryStore class For keeping a history of all the launched tasks. All the
 * modification to this task is persisted to a local file. This class is thread safe.
 */
class TaskHistoryStore {
  public:
    /**
     * @brief LocalTaskStore On construction, it attempts to recover the history from disk. New
     * history is also stored in the file specified by the persistence_path.
     */
    TaskHistoryStore(std::string const &persistence_path);
    ~TaskHistoryStore();

    /**
     * @brief Add Appends a new task entry to the history log. If there is an existing entry with
     * the same task ID, this function will fail.
     */
    void Add(TaskBasicInfo const &task);

    /**
     * @brief MarkTermination Marks the termination of the specified task. It updates the
     * termination timestamp of the task's log entry.
     *
     * @param task_id ID of the launched task to be mark terminated.
     * @return True if the task exists.
     */
    bool MarkTermination(LocalTaskId const &task_id);

    /**
     * @brief List Returns a list of all the tasks that are in the store sorted by the launch
     * timestamp in descending order.
     */
    std::vector<TaskBasicInfo> List() const;

  private:
    void Persist();
    void Recover(std::string const &file_path);

    TaskHistoryData data_;
    uuid4_state_t uuid_state_;
    std::fstream file_;
    std::unique_ptr<std::mutex> mu_;
};

} // namespace e8

#endif // CLUSTER_TASK_MANAGER_HISTORY_H
