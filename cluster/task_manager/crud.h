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

#ifndef CLUSTER_TASK_MANAGER_CRUD_H
#define CLUSTER_TASK_MANAGER_CRUD_H

#include <string>

#include "cluster/task_manager/context.h"
#include "proto_cc/task.pb.h"

namespace e8 {

/**
 * @brief LaunchStartupTasks Launches all the startup tasks, if there is any, listed from the start
 * up configuration store.
 */
void LaunchStartupTasks(TaskManagerContext *context);

/**
 * @brief The LaunchTaskResult struct The result of the function LaunchTask().
 */
struct LaunchTaskResult {
    // Indicates the type of error occurred while launching the new task.
    LaunchTaskError error;

    // If the error is an OS_ERROR, this gives the error code returned by the operating system.
    int os_return_code;

    // If there hasn't been error, this is the information about the new task that was launched.
    TaskBasicInfo task_info;
};

/**
 * @brief LaunchTask Launches a new task. It creates a child process to house the new task, then
 * runs the binary specified by the launch configuration in it. It redirects all the standard
 * outputs of the new task to local files for keeping a record of the run. Then it registers the
 * task to the task registry asynchronously. The binary is guaranteed to be running when this
 * function ends, but may appear to be running, according to the registry, briefly after.
 *
 * @param launch_config Defines what and how a task should be launched.
 * @param context Global context of the task manager module.
 * @return See the above LaunchTaskResult structure.
 */
LaunchTaskResult LaunchTask(LaunchConfig const &launch_config, TaskManagerContext *context);

/**
 * @brief The TerminateTaskResult struct The result of the function TerminateTask().
 */
struct TerminateTaskResult {
    // Indicates the type of error occurred while terminating the task.
    TerminateTaskError error;

    // If the error is an OS_ERROR, this gives the error code returned by the operating system.
    int os_return_code;
};

/**
 * @brief TerminateTask Terminates a running task. This function does not block to wait for the
 * task's termination.
 *
 * @param task_id ID of the task to be terminated.
 * @param context Global context of the task manager module.
 */
TerminateTaskResult TerminateTask(LocalTaskId const &task_id, TaskManagerContext *context);

} // namespace e8

#endif // CLUSTER_TASK_MANAGER_CRUD_H
