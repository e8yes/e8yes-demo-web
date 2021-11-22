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
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>

#include "cluster/task_manager/common_types.h"
#include "cluster/task_manager/history.h"
#include "cluster/task_manager/registry.h"
#include "cluster/task_manager/startup.h"
#include "common/thread/thread_pool.h"
#include "common/time_util/time_util.h"
#include "proto_cc/task.pb.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {
namespace {

TimeIntervalMillis kScanningInterval = 500;

class RegistrationHandler : public TaskInterface {
  public:
    RegistrationHandler(std::unordered_map<pid_t, TaskBasicInfo> *running_tasks,
                        std::mutex *running_tasks_mu);
    ~RegistrationHandler() override;

    void Add(pid_t process_id, TaskBasicInfo const &task);

    void Run(TaskStorageInterface *) const override;
    bool DropResourceOnCompletion() const override;
    void Shutdown();

  private:
    std::optional<std::pair<pid_t, TaskBasicInfo>> NextRegistration();
    bool TryRegister(std::pair<pid_t, TaskBasicInfo> const &registration);
    void PutbackRegistration(std::pair<pid_t, TaskBasicInfo> const &registration);

    bool running_;
    std::mutex mu_;
    std::queue<std::pair<pid_t, TaskBasicInfo>> todos_;

    std::unordered_map<pid_t, TaskBasicInfo> *running_tasks_;
    std::mutex *running_tasks_mu_;
};

RegistrationHandler::RegistrationHandler(std::unordered_map<pid_t, TaskBasicInfo> *running_tasks,
                                         std::mutex *running_tasks_mu)
    : running_(true), running_tasks_(running_tasks), running_tasks_mu_(running_tasks_mu) {}

RegistrationHandler::~RegistrationHandler() {}

std::optional<std::pair<pid_t, TaskBasicInfo>> RegistrationHandler::NextRegistration() {
    std::lock_guard guard(mu_);
    if (todos_.empty()) {
        return std::nullopt;
    }

    std::pair<pid_t, TaskBasicInfo> registration = todos_.front();
    todos_.pop();

    return registration;
}

bool RegistrationHandler::TryRegister(std::pair<pid_t, TaskBasicInfo> const &registration) {
    std::lock_guard guard(*running_tasks_mu_);
    return running_tasks_->insert(registration).second;
}

void RegistrationHandler::PutbackRegistration(std::pair<pid_t, TaskBasicInfo> const &registration) {
    std::lock_guard guard(mu_);
    todos_.push(registration);
}

void RegistrationHandler::Add(pid_t process_id, TaskBasicInfo const &task) {
    std::lock_guard guard(mu_);
    todos_.push(std::make_pair(process_id, task));
}

void RegistrationHandler::Run(TaskStorageInterface *) const {
    RegistrationHandler *this_ = const_cast<RegistrationHandler *>(this);

    while (running_) {
        std::optional<std::pair<pid_t, TaskBasicInfo>> registration = this_->NextRegistration();
        if (!registration.has_value()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(kScanningInterval));
            continue;
        }

        if (!this_->TryRegister(*registration)) {
            this_->PutbackRegistration(*registration);
        }
    }
}

bool RegistrationHandler::DropResourceOnCompletion() const { return false; }

void RegistrationHandler::Shutdown() { running_ = false; }

class TerminationHandler : public TaskInterface {
  public:
    TerminationHandler(std::unordered_map<pid_t, TaskBasicInfo> *running_tasks,
                       std::mutex *running_tasks_mu, TaskHistoryStore *history);
    ~TerminationHandler() override;

    void Run(TaskStorageInterface *) const override;
    bool DropResourceOnCompletion() const override;
    void Shutdown();
    std::optional<pid_t> NextTermination();
    bool TryTermination(pid_t termination);
    void PutbackTermination(pid_t termination);
    void AddTermination(pid_t termination);

  private:
    bool running_;

    std::queue<pid_t> todos_;
    std::mutex mu_;

    std::unordered_map<pid_t, TaskBasicInfo> *running_tasks_;
    std::mutex *running_tasks_mu_;

    TaskHistoryStore *history_;
};

TerminationHandler::TerminationHandler(std::unordered_map<pid_t, TaskBasicInfo> *running_tasks,
                                       std::mutex *running_tasks_mu, TaskHistoryStore *history)
    : running_(true), running_tasks_(running_tasks), running_tasks_mu_(running_tasks_mu),
      history_(history) {}

TerminationHandler::~TerminationHandler() {}

std::optional<pid_t> TerminationHandler::NextTermination() {
    pid_t termination = wait(nullptr);
    if (termination != -1) {
        return termination;
    }

    std::lock_guard guard(mu_);

    if (!todos_.empty()) {
        termination = todos_.front();
        todos_.pop();
        return termination;
    }

    return std::nullopt;
}

bool TerminationHandler::TryTermination(pid_t termination) {
    std::lock_guard guard(*running_tasks_mu_);

    auto task_it = running_tasks_->find(termination);
    if (task_it == running_tasks_->end()) {
        return false;
    }

    history_->MarkTermination(task_it->second.task_id());
    running_tasks_->erase(task_it);

    return true;
}

void TerminationHandler::PutbackTermination(pid_t termination) {
    std::lock_guard guard(mu_);
    todos_.push(termination);
}

void TerminationHandler::AddTermination(pid_t termination) {
    this->PutbackTermination(termination);
}

void TerminationHandler::Run(TaskStorageInterface *) const {
    TerminationHandler *this_ = const_cast<TerminationHandler *>(this);

    while (running_) {
        std::optional<pid_t> termination = this_->NextTermination();
        if (!termination.has_value()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(kScanningInterval));
            continue;
        }

        if (!this_->TryTermination(*termination)) {
            this_->PutbackTermination(*termination);
        }
    }
}

bool TerminationHandler::DropResourceOnCompletion() const { return false; }

void TerminationHandler::Shutdown() { running_ = false; }

} // namespace

struct TaskRegistry::TaskRegistryImpl {
    TaskRegistryImpl(TaskHistoryStore *history, StartupTaskConfigStore *startup_configs);
    ~TaskRegistryImpl();

    std::unordered_map<pid_t, TaskBasicInfo> running_tasks;
    std::mutex mu;

    TaskHistoryStore *history;
    StartupTaskConfigStore *startup_configs;

    uuid4_state_t uuid_state;

    std::shared_ptr<RegistrationHandler> registration_handler;
    std::shared_ptr<TerminationHandler> termination_handler;
    ThreadPool background_threads;
};

TaskRegistry::TaskRegistryImpl::TaskRegistryImpl(TaskHistoryStore *history,
                                                 StartupTaskConfigStore *startup_configs)
    : history(history), startup_configs(startup_configs),
      registration_handler(std::make_shared<RegistrationHandler>(&running_tasks, &mu)),
      termination_handler(std::make_shared<TerminationHandler>(&running_tasks, &mu, history)),
      background_threads(/*num_workers=*/2) {
    uuid4_seed(&uuid_state);

    background_threads.Schedule(registration_handler);
    background_threads.Schedule(termination_handler);
}

TaskRegistry::TaskRegistryImpl::~TaskRegistryImpl() {
    registration_handler->Shutdown();
    termination_handler->Shutdown();

    background_threads.WaitForNextCompleted();
    background_threads.WaitForNextCompleted();
}

TaskRegistry::TaskRegistry(TaskHistoryStore *history, StartupTaskConfigStore *startup_configs)
    : pimpl_(std::make_unique<TaskRegistryImpl>(history, startup_configs)) {}

TaskRegistry::~TaskRegistry() {}

TaskRegistry::QueryRunningTaskResult::QueryRunningTaskResult(
    std::unordered_map<pid_t, TaskBasicInfo> const &running_tasks, std::mutex *mu)
    : running_tasks_(running_tasks), mu_(mu) {
    mu_->lock();
}

TaskRegistry::QueryRunningTaskResult::~QueryRunningTaskResult() { mu_->unlock(); }

std::unordered_map<pid_t, TaskBasicInfo> const &TaskRegistry::QueryRunningTaskResult::List() const {
    return running_tasks_;
}

std::optional<std::pair<pid_t, TaskBasicInfo>>
TaskRegistry::QueryRunningTaskResult::FindByTaskId(LocalTaskId const &task_id) const {
    for (auto const &[pid, task] : running_tasks_) {
        if (task.task_id() == task_id) {
            return std::make_pair(pid, task);
        }
    }
    return std::nullopt;
}

std::optional<std::pair<pid_t, TaskBasicInfo>>
TaskRegistry::QueryRunningTaskResult::FindByLaunchConfigId(LaunchConfigId const &config_id) const {
    for (auto const &[pid, task] : running_tasks_) {
        if (task.launch_config().config_id() == config_id) {
            return std::make_pair(pid, task);
        }
    }
    return std::nullopt;
}

LocalTaskId TaskRegistry::AllocateTaskId() {
    uuid4_t uuid;
    uuid4_gen(&pimpl_->uuid_state, &uuid);
    char uuid_string[UUID4_STR_BUFFER_SIZE];
    uuid4_to_s(uuid, uuid_string, sizeof(uuid_string));

    return LocalTaskId(uuid_string);
}

TaskBasicInfo TaskRegistry::RegisterNewTask(LocalTaskId const &task_id, pid_t process_id,
                                            LaunchConfig const &launch_config,
                                            std::string const &stdout_file_name,
                                            std::string const &stderr_file_name,
                                            std::string const &stdall_file_name) {
    std::lock_guard guard(pimpl_->mu);

    if (launch_config.startup()) {
        pimpl_->startup_configs->Add(launch_config);
    }

    TaskBasicInfo task_info;
    task_info.set_task_id(task_id);
    task_info.set_launch_timestamp_millis(CurrentTimestampMillis());
    task_info.set_termination_timestamp_millis(-1);
    task_info.set_stdout_file_name(stdout_file_name);
    task_info.set_stderr_file_name(stderr_file_name);
    task_info.set_stdall_file_name(stdall_file_name);
    *task_info.mutable_launch_config() = launch_config;

    pimpl_->registration_handler->Add(process_id, task_info);
    pimpl_->history->Add(task_info);

    return task_info;
}

TaskRegistry::QueryRunningTaskResult TaskRegistry::RunningTasks() const {
    return QueryRunningTaskResult(pimpl_->running_tasks, &pimpl_->mu);
}

void TaskRegistry::AddTermination(pid_t process_id) {
    pimpl_->termination_handler->AddTermination(process_id);
}

} // namespace e8
