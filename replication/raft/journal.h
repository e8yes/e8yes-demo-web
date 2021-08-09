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

#include <google/protobuf/repeated_field.h>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

#include "proto_cc/command.pb.h"
#include "proto_cc/raft.pb.h"
#include "replication/raft/role_at_term.h"

namespace e8 {

/**
 * @brief The RaftCommitListener class Used by the state machine to respond to the committed log
 * entries when they are sufficiently replicated.
 */
class RaftCommitListener {
  public:
    /**
     * @brief AfterCommit When a log entry has been committed, this will be called so that the state
     * machine can change its state according to the command carried in the log.
     */
    virtual void AfterCommit(CommandEntry const &entry) = 0;
};

/**
 * @brief The RaftJournal class Handles logging and commit in a thread-safe and crash-safe manner.
 */
class RaftJournal {
  public:
    /**
     * @brief RaftJournal On constuction of this object, it will attempt to recover the persistent
     * states from the specified log file, if one exists.
     *
     * @param commit_listener The listener will be notified with committed log entries.
     * @param log_path File path to the local file system to persist log entries for crash recovery
     * purposes.
     */
    RaftJournal(std::shared_ptr<RaftCommitListener> const &commit_listener,
                std::string const &log_path);
    ~RaftJournal();

    /**
     * @brief MergeForeignLogs Overwrites and appends the local logs with foreign logs starting from
     * the "from" index. Note that the "from" index can set at the end of the local logs but no more
     * further. After the operation, logs that are different from the foreign source and those come
     * after these entries will be deleted and replaced with the foreign entries. After the local
     * log content is modified, it persists the updated local logs.
     *
     * @param from The starting position of the local log source to overwrite.
     * @param foreign_log_entries Partial foreign log entries to overwrite from.
     * @param term_and_role The current term and role of the caller that starts this call.
     */
    void MergeForeignLogs(unsigned from,
                          google::protobuf::RepeatedField<LogEntry> const &foreign_log_entries,
                          std::pair<RaftTerm, RaftRole> const &term_and_role);

  private:
    void Persist(std::pair<RaftTerm, RaftRole> const &term_and_role);
    void Recover();

    std::mutex lock_;

    RaftPersistentStates data_;

    std::shared_ptr<RaftCommitListener> commit_listener_;
    unsigned commit_progress_;
    unsigned listener_progress_;

    std::string const log_path_;
};

} // namespace e8

#endif // MERGE_LOGS_H