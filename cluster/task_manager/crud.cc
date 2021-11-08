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
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <mutex>
#include <optional>
#include <signal.h>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

#include "cluster/task_manager/common_types.h"
#include "cluster/task_manager/context.h"
#include "cluster/task_manager/crud.h"
#include "cluster/task_manager/history.h"
#include "cluster/task_manager/registry.h"
#include "cluster/task_manager/startup.h"
#include "common/time_util/time_util.h"
#include "proto_cc/task.pb.h"

namespace e8 {
namespace {

std::string BinaryPath(LaunchConfig const &launch_config, TaskManagerContext const &context) {
    switch (launch_config.binary_directory()) {
    case LaunchConfig::OS: {
        return launch_config.binary_name();
    }
    case LaunchConfig::TOOLS: {
        return context.tools_binary_path + "/" + launch_config.binary_name();
    }
    case LaunchConfig::BUILD: {
        return context.build_binary_path + "/" + launch_config.binary_name();
    }
    default: {
        assert(false);
    }
    }
}

char **BinaryArgs(LaunchConfig const &launch_config) {
    char **args = new char *[launch_config.args_size() * 2];
    unsigned i_arg = 0;

    for (auto const &[arg_key, arg_value] : launch_config.args()) {
        args[i_arg + 0] = new char[arg_key.size() + 2];
        args[i_arg + 0][0] = '-';
        args[i_arg + 0][1] = '-';
        std::strncpy(args[i_arg] + 2, arg_key.c_str(), arg_key.size());

        args[i_arg + 1] = new char[arg_value.size()];
        std::strncpy(args[i_arg + 1], arg_value.c_str(), arg_value.size());

        i_arg += 2;
    }

    return args;
}

void DeleteBinaryArgs(char **args, LaunchConfig const &launch_config) {
    for (int i = 0; i < launch_config.args_size() * 2; ++i) {
        delete[] args[i];
    }

    delete[] args;
}

void StdOutputRedirectionFiles(std::string const &binary_name, LocalTaskId const &task_id,
                               TaskManagerContext *context, std::string *stdout_file_name,
                               std::string *stderr_file_name, std::string *stdall_file_name) {
    *stdout_file_name = context->task_stdlog_path + "/" + binary_name + "." + task_id + ".stdout";
    *stderr_file_name = context->task_stdlog_path + "/" + binary_name + "." + task_id + ".stderr";
    *stdall_file_name = context->task_stdlog_path + "/" + binary_name + "." + task_id + ".stdall";
}

void HandleChildProcess(std::string const &binary_path, char **args, int error_code_pipe_fd,
                        std::string const &stdout_file_name, std::string const &stderr_file_name,
                        std::string const &stdall_file_name) {
    int stdout_fd = open(stdout_file_name.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0200);
    int stderr_fd = open(stderr_file_name.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0200);
    int stdall_fd = open(stdall_file_name.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0200);

    assert(stdout_fd != -1);
    assert(stderr_fd != -1);
    assert(stdall_fd != -1);

    int stdout_dup = dup(STDOUT_FILENO);
    int stderr_dup = dup(STDERR_FILENO);

    dup2(stdall_fd, stdout_dup);
    dup2(stdall_fd, stderr_dup);
    dup2(stdout_fd, STDOUT_FILENO);
    dup2(stderr_fd, STDERR_FILENO);

    if (-1 == execv(binary_path.c_str(), args)) {
        ssize_t bytes_written = write(error_code_pipe_fd, &errno, sizeof(errno));
        assert(bytes_written == sizeof(errno));
        exit(EXIT_FAILURE);
    } else {
        assert(false);
    }
}

pid_t RunBinary(std::string const &binary_path, char **args, std::string const &stdout_file_name,
                std::string const &stderr_file_name, std::string const &stdall_file_name,
                int *os_error_code) {
    int error_code_pipe_fds[2];
    int pipe_err = pipe2(error_code_pipe_fds, /*__flags=*/O_CLOEXEC);
    if (pipe_err == -1) {
        *os_error_code = errno;
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        *os_error_code = errno;
        return -1;
    }

    if (pid == 0) {
        HandleChildProcess(binary_path, args, error_code_pipe_fds[1], stdout_file_name,
                           stderr_file_name, stdall_file_name);
    }

    // Parent process.
    int exec_error_code;
    if (read(error_code_pipe_fds[0], &exec_error_code, sizeof(exec_error_code)) > 0) {
        *os_error_code = exec_error_code;
        return -1;
    }

    *os_error_code = 0;
    return pid;
}

} // namespace

void LaunchStartupTasks(TaskManagerContext *context) {
    google::protobuf::RepeatedPtrField<LaunchConfig> configs = context->startup_configs->List();

    for (auto const &startup_config : configs) {
        LaunchTask(startup_config, context);
    }
}

LaunchTaskResult LaunchTask(LaunchConfig const &launch_config, TaskManagerContext *context) {
    std::lock_guard guard(context->global_lock);

    LaunchTaskResult result;

    if (context->task_registry->RunningTasks()
            .FindByLaunchConfigId(launch_config.config_id())
            .has_value()) {
        result.error = LaunchTaskError::LTE_ALREADY_RUNNING;
        result.os_return_code = 0;
        return result;
    }

    LocalTaskId task_id = context->task_registry->AllocateTaskId();

    // Gathers information about the task's binary.
    std::string binary_path = BinaryPath(launch_config, *context);
    char **args = BinaryArgs(launch_config);

    std::string stdout_file_name;
    std::string stderr_file_name;
    std::string stdall_file_name;
    StdOutputRedirectionFiles(binary_path, task_id, context, &stdout_file_name, &stderr_file_name,
                              &stdall_file_name);

    // Creates a new process to run the binary.
    int os_error_code;
    pid_t binary_pid = RunBinary(launch_config.binary_name(), args, stdout_file_name,
                                 stderr_file_name, stdall_file_name, &os_error_code);
    DeleteBinaryArgs(args, launch_config);

    if (os_error_code != 0) {
        result.error = LaunchTaskError::LTE_OS_ERROR;
        result.os_return_code = os_error_code;
        return result;
    }

    // Registers the task.
    result.task_info = context->task_registry->RegisterNewTask(
        task_id, binary_pid, launch_config, stdout_file_name, stderr_file_name, stdall_file_name);
    result.error = LaunchTaskError::LTE_NONE;
    result.os_return_code = os_error_code;

    return result;
}

TerminateTaskResult TerminateTask(LocalTaskId const &task_id, TaskManagerContext *context) {
    std::lock_guard guard(context->global_lock);

    TerminateTaskResult result;

    std::optional<std::pair<pid_t, TaskBasicInfo>> search_result =
        context->task_registry->RunningTasks().FindByTaskId(task_id);
    if (!search_result.has_value()) {
        result.error = TerminateTaskError::TTE_NOT_RUNNING;
        result.os_return_code = 0;
        return result;
    }

    auto const &[task_pid, _] = *search_result;

    int kill_result = kill(task_pid, SIGTERM);
    if (kill_result == -1) {
        result.error = TerminateTaskError::TTE_OS_ERROR;
        result.os_return_code = errno;
        return result;
    }

    result.error = TerminateTaskError::TTE_NONE;
    result.os_return_code = 0;
    return result;
}

} // namespace e8
