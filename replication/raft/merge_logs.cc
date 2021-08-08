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
#include <vector>

#include "proto_cc/raft.pb.h"
#include "replication/raft/merge_logs.h"

namespace e8 {

void OverwriteWithForeignLogs(std::vector<LogEntry> const &foreign_log_entries,
                              unsigned start_index, unsigned commit_index,
                              std::vector<LogEntry> *local_log_entries) {
    // start_index shouldn't be after the end of the local logs.
    assert(start_index <= foreign_log_entries.size());

    for (unsigned i = 0; i < foreign_log_entries.size(); i++) {
        unsigned target_index = start_index + i;

        if (target_index < commit_index) {
            // It's disastrous to override committed logs with different entries.
            assert((*local_log_entries)[target_index].term() == foreign_log_entries[i].term());
        }

        if (target_index >= local_log_entries->size()) {
            local_log_entries->push_back(foreign_log_entries[i]);
            continue;
        }

        if ((*local_log_entries)[target_index].term() != foreign_log_entries[i].term()) {
            // Trim everything beyond target_index because the foreign log contains different
            // entries starting at this point.
            (*local_log_entries)[target_index] = foreign_log_entries[i];
            local_log_entries->resize(target_index + 1);
        } else {
            // Doesn't have to do anything if the two entries are the same.
        }
    }
}

} // namespace e8
