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

#ifndef CLUSTER_TASK_MANAGER_REGISTRY_H
#define CLUSTER_TASK_MANAGER_REGISTRY_H

#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unistd.h>
#include <unordered_map>

#include "cluster/task_manager/common_types.h"
#include "cluster/task_manager/history.h"
#include "cluster/task_manager/startup.h"
#include "proto_cc/task.pb.h"

namespace e8 {

/**
 * @brief The TaskRegistry class Detects and responds to the lifecycle of all running tasks.
 */
class TaskRegistry {
  public:
    /**
     * @brief TaskRegistry This class encapsulates mutations to the history logs and startup list
     * via the provided functions.
     */
    TaskRegistry(TaskHistoryStore *history, StartupTaskConfigStore *startup_configs);
    ~TaskRegistry();

    /**
     * @brief The QueryRunningTaskResult class Represents the result of querying the current
     * collection of running tasks. It blocks all mutations to the registry as long as this class is
     * alive.
     */
    class QueryRunningTaskResult {
      public:
        /**
         * @brief QueryRunningTaskResult Do not construct this class. Acquires it by calling the
         * function RunningTasks().
         */
        QueryRunningTaskResult(std::unordered_map<pid_t, TaskBasicInfo> const &running_tasks,
                               std::mutex *mu);
        ~QueryRunningTaskResult();

        /**
         * @brief List Returns all task information entries keyed by the task's system allocated
         * process ID.
         */
        std::unordered_map<pid_t, TaskBasicInfo> const &List() const;

        /**
         * @brief FindByTaskId Retrieves the information about a particular task entry by task ID if
         * one exists.
         */
        std::optional<std::pair<pid_t, TaskBasicInfo>>
        FindByTaskId(LocalTaskId const &task_id) const;

        /**
         * @brief FindByLaunchConfigId Retrieves the information about a particular task entry by
         * the ID of the configuration which launches it.
         */
        std::optional<std::pair<pid_t, TaskBasicInfo>>
        FindByLaunchConfigId(LaunchConfigId const &config_id) const;

      private:
        std::unordered_map<pid_t, TaskBasicInfo> const &running_tasks_;
        std::mutex *mu_;
    };

    /**
     * @brief AllocateTaskId Allocates an ID for creating a new task.
     */
    LocalTaskId AllocateTaskId();

    /**
     * @brief RegisterNewTask Adds a newly launched task to the registry and so it detects and
     * manages the task's lifecycle.
     *
     * @param task_id ID of the task that was just launched.
     * @param process_id The system allocated process ID of the task.
     * @param launch_config The configuration used to launch the task.
     * @param stdout_file_name File path where the task redirects its standard output.
     * @param stderr_file_name File path where the task redirects its standard error output.
     * @param stdall_file_name File path where the task redirects its standard output and error
     * output combined.
     *
     * @return The basic information that describes the task.
     */
    TaskBasicInfo RegisterNewTask(LocalTaskId const &task_id, pid_t process_id,
                                  LaunchConfig const &launch_config,
                                  std::string const &stdout_file_name,
                                  std::string const &stderr_file_name,
                                  std::string const &stdall_file_name);

    /**
     * @brief RunningTasks Blocks the registry and returns all its recorded the running tasks.
     */
    QueryRunningTaskResult RunningTasks() const;

  private:
    struct TaskRegistryImpl;

    std::unique_ptr<TaskRegistryImpl> pimpl_;
};

} // namespace e8

#endif // CLUSTER_TASK_MANAGER_REGISTRY_H
