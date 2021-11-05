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
#include <fstream>
#include <google/protobuf/repeated_field.h>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "cluster/task_manager/common_types.h"
#include "cluster/task_manager/startup.h"
#include "proto_cc/task.pb.h"

namespace e8 {

StartupTaskConfigStore::StartupTaskConfigStore(std::string const &persistence_path)
    : persistence_path_(persistence_path), mu_(std::make_unique<std::mutex>()) {
    this->Recover();
}

StartupTaskConfigStore::~StartupTaskConfigStore() {}

bool StartupTaskConfigStore::Add(LaunchConfig const &startup_task_config) {
    std::lock_guard guard(*mu_);

    auto it = std::find_if(data_.startup_task_launch_configs().begin(),
                           data_.startup_task_launch_configs().end(),
                           [&startup_task_config](LaunchConfig const &a) {
                               return a.config_id() == startup_task_config.config_id();
                           });
    if (it != data_.startup_task_launch_configs().end()) {
        return false;
    }

    *data_.mutable_startup_task_launch_configs()->Add() = startup_task_config;
    this->Persist();

    return true;
}

bool StartupTaskConfigStore::Remove(LaunchConfigId const &config_id) {
    std::lock_guard guard(*mu_);

    auto it = std::find_if(
        data_.startup_task_launch_configs().begin(), data_.startup_task_launch_configs().end(),
        [&config_id](LaunchConfig const &a) { return a.config_id() == config_id; });
    if (it == data_.startup_task_launch_configs().end()) {
        return false;
    }

    data_.mutable_startup_task_launch_configs()->erase(it);
    this->Persist();

    return true;
}

google::protobuf::RepeatedPtrField<LaunchConfig> StartupTaskConfigStore::List() const {
    std::lock_guard guard(*mu_);
    return data_.startup_task_launch_configs();
}

void StartupTaskConfigStore::Persist() {
    std::remove(persistence_path_.c_str());

    std::fstream file;
    file.open(persistence_path_.c_str(),
              std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    assert(file.is_open());

    data_.SerializeToOstream(&file);
}

void StartupTaskConfigStore::Recover() {
    std::fstream recovery_file(persistence_path_, std::ios::in | std::ios::binary);
    if (!recovery_file.is_open()) {
        return;
    }

    data_.ParseFromIstream(&recovery_file);
}

} // namespace e8
