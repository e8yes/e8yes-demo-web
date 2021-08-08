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

#ifndef MERGE_LOGS_H
#define MERGE_LOGS_H

#include <vector>

#include "proto_cc/raft.pb.h"

namespace e8 {

/**
 * @brief OverwriteWithForeignLogs Overwrites and appends the local logs with foreign logs starting
 * from the start_index. Note that start_index can set at the end of the local logs but no more
 * further. After the operation, logs that are different from the foreign source and those come
 * after these entries will be deleted and replaced with the foreign entries.
 *
 * @param foreign_log_entries Partial foreign log entries to overwrite from.
 * @param start_index The starting position of the local log source to overwrite.
 * @param commit_index The current commit index of the local log source. It is used to detect
 * inconsistency bugs.
 * @param local_log_entries The local log source to be overwritten.
 */
void OverwriteWithForeignLogs(std::vector<LogEntry> const &foreign_log_entries,
                              unsigned start_index, unsigned commit_index,
                              std::vector<LogEntry> *local_log_entries);

} // namespace e8

#endif // MERGE_LOGS_H
