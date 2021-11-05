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

#ifndef CLUSTER_TASK_MANAGER_STARTUP_H
#define CLUSTER_TASK_MANAGER_STARTUP_H

#include <fstream>
#include <google/protobuf/repeated_field.h>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "cluster/task_manager/common_types.h"
#include "proto_cc/task.pb.h"

namespace e8 {

/**
 * @brief The StartupTaskConfigStore class Keeps a list of all startup tasks. All the modification
 * to this task is persisted to a local file. This class is thread safe.
 */
class StartupTaskConfigStore {
  public:
    /**
     * @brief StartupTaskConfigStore On construction, it attempts to recover the startup list from
     * disk. New startup list  is also stored in the file specified by the persistence_path.
     */
    StartupTaskConfigStore(std::string const &persistence_path);
    ~StartupTaskConfigStore();

    /**
     * @brief Add Adds a new startup task to the list. The startup tasks are in
     * first-come-first-serve order. The startup configuration can only be added once to the list.
     *
     * @param startup_task_config The launch configuration that is used to launch the startup task.
     * @return True if the configuration isn't in the list and is added.
     */
    bool Add(LaunchConfig const &startup_task_config);

    /**
     * @brief Remove Removes a startup task from the list.
     *
     * @param config_id ID of the configuration to be remove from the list.
     * @return True if the startup task is in the list.
     */
    bool Remove(LaunchConfigId const &config_id);

    /**
     * @brief List Returns the startup task list, in the order the startup task was added.
     */
    google::protobuf::RepeatedPtrField<LaunchConfig> List() const;

  private:
    void Persist();
    void Recover();

    StartupTaskConfigData data_;
    std::string const persistence_path_;
    std::unique_ptr<std::mutex> mu_;
};

} // namespace e8

#endif // CLUSTER_TASK_MANAGER_STARTUP_H
