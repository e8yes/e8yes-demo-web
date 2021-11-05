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

#include <algorithm>
#include <cassert>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "cluster/task_manager/common_types.h"
#include "cluster/task_manager/history.h"
#include "common/time_util/time_util.h"
#include "proto_cc/task.pb.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {

TaskHistoryStore::TaskHistoryStore(std::string const &persistence_path)
    : mu_(std::make_unique<std::mutex>()) {
    uuid4_seed(&uuid_state_);

    this->Recover(persistence_path);

    file_.open(persistence_path.c_str(),
               std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    assert(file_.is_open());
}

TaskHistoryStore::~TaskHistoryStore() {}

TaskBasicInfo TaskHistoryStore::Add(LaunchConfig const &launch_config,
                                    std::string const &stdout_file_name,
                                    std::string const &stderr_file_name,
                                    std::string const &stdall_file_name) {
    std::lock_guard guard(*mu_);

    uuid4_t uuid;
    uuid4_gen(&uuid_state_, &uuid);
    char uuid_string[UUID4_STR_BUFFER_SIZE];
    uuid4_to_s(uuid, uuid_string, sizeof(uuid_string));

    TaskBasicInfo task_info;
    task_info.set_task_id(uuid_string);
    task_info.set_launch_timestamp_millis(CurrentTimestampMillis());
    task_info.set_termination_timestamp_millis(-1);
    task_info.set_stdout_file_name(stdout_file_name);
    task_info.set_stderr_file_name(stderr_file_name);
    task_info.set_stdall_file_name(stdall_file_name);
    *task_info.mutable_launch_config() = launch_config;
    (*data_.mutable_history())[task_info.task_id()] = task_info;

    this->Persist();

    return task_info;
}

bool TaskHistoryStore::MarkTermination(LocalTaskId const &task_id) {
    std::lock_guard guard(*mu_);

    auto task_it = data_.mutable_history()->find(task_id);
    if (task_it == data_.history().end()) {
        return false;
    }

    task_it->second.set_termination_timestamp_millis(CurrentTimestampMillis());
    return true;
}

std::vector<TaskBasicInfo> TaskHistoryStore::List() const {
    std::lock_guard guard(*mu_);

    std::vector<TaskBasicInfo> tasks(data_.history_size());
    unsigned i = 0;
    for (auto const &[_, task] : data_.history()) {
        tasks[i++] = task;
    }

    std::sort(tasks.begin(), tasks.end(), [](TaskBasicInfo const &a, TaskBasicInfo const &b) {
        return a.launch_timestamp_millis() > b.launch_timestamp_millis();
    });

    return tasks;
}

void TaskHistoryStore::Persist() {
    assert(file_.is_open());
    file_.seekp(std::ios_base::beg);
    data_.SerializeToOstream(&file_);
    file_.flush();
}

void TaskHistoryStore::Recover(std::string const &file_path) {
    std::fstream recovery_file(file_path, std::ios::in | std::ios::binary);
    if (!recovery_file.is_open()) {
        return;
    }

    data_.ParseFromIstream(&recovery_file);
}

} // namespace e8
