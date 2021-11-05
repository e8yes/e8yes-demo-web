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

#include <memory>
#include <string>

#include "cluster/task_manager/context.h"
#include "cluster/task_manager/history.h"
#include "cluster/task_manager/pid_slots.h"
#include "cluster/task_manager/startup.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {

std::unique_ptr<TaskManagerContext>
CreateTaskManagerContext(std::string const &tools_binary_path, std::string const &build_binary_path,
                         std::string const &task_stdlog_path,
                         std::string const &task_history_log_path,
                         std::string const &startup_config_path) {
    auto context = std::make_unique<TaskManagerContext>();
    context->history = std::make_unique<TaskHistoryStore>(task_history_log_path);
    context->pid_slots = std::make_unique<ProcessIdSlots>();
    context->startup_configs = std::make_unique<StartupTaskConfigStore>(startup_config_path);
    context->tools_binary_path = tools_binary_path;
    context->build_binary_path = build_binary_path;
    context->task_stdlog_path = task_stdlog_path;
    uuid4_seed(&context->uuid_state);
    return context;
}

} // namespace e8
