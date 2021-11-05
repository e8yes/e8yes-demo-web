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

#include <mutex>
#include <optional>
#include <unistd.h>
#include <unordered_map>

#include "cluster/task_manager/common_types.h"
#include "cluster/task_manager/pid_slots.h"

namespace e8 {

ProcessIdSlots::ProcessIdSlots() {}

ProcessIdSlots::~ProcessIdSlots() {}

void ProcessIdSlots::lock() { mu_.lock(); }

void ProcessIdSlots::unlock() { mu_.unlock(); }

std::optional<LocalTaskId> ProcessIdSlots::UseSlot(pid_t pid, LocalTaskId const &new_task_id) {
    auto slot_it = slots_.find(pid);

    if (slot_it != slots_.end()) {
        LocalTaskId old_task_id = slot_it->second;
        slot_it->second = new_task_id;
        return old_task_id;
    } else {
        slots_.insert(std::make_pair(pid, new_task_id));
        return std::nullopt;
    }
}

std::optional<LocalTaskId> ProcessIdSlots::ClearSlot(pid_t pid) {
    auto slot_it = slots_.find(pid);

    if (slot_it != slots_.end()) {
        LocalTaskId old_task_id = slot_it->second;
        slots_.erase(slot_it);
        return old_task_id;
    } else {
        return std::nullopt;
    }
}

std::unordered_map<pid_t, LocalTaskId> ProcessIdSlots::All() const { return slots_; }

} // namespace e8
