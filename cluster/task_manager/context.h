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

#ifndef CLUSTER_TASK_MANAGER_CONTEXT_H
#define CLUSTER_TASK_MANAGER_CONTEXT_H

#include <memory>
#include <string>

#include "cluster/task_manager/history.h"
#include "cluster/task_manager/pid_slots.h"
#include "cluster/task_manager/startup.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {

/**
 * @brief The TaskManagerContext struct Global context which the task manager will run on.
 */
struct TaskManagerContext {
    std::unique_ptr<TaskHistoryStore> history;
    std::unique_ptr<ProcessIdSlots> pid_slots;
    std::unique_ptr<StartupTaskConfigStore> startup_configs;

    std::string tools_binary_path;
    std::string build_binary_path;
    std::string task_stdlog_path;

    uuid4_state_t uuid_state;
};

/**
 * @brief CreateTaskManagerContext Constructs the context which the task manager will run on.
 *
 * @param tools_binary_path File path that the task manager searches for tools binary.
 * @param build_binary_path File path that the task manager searches for built binary.
 * @param task_stdlog_path File path that a launched task stores its standard outputs.
 * @param task_history_log_path File path for storing the task history.
 * @param startup_config_path File path for storing startup configurations.
 *
 * @return The context.
 */
std::unique_ptr<TaskManagerContext>
CreateTaskManagerContext(std::string const &tools_binary_path, std::string const &build_binary_path,
                         std::string const &task_stdlog_path,
                         std::string const &task_history_log_path,
                         std::string const &startup_config_path);

} // namespace e8

#endif // CLUSTER_TASK_MANAGER_CONTEXT_H
