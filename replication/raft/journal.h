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

#ifndef RAFT_JOURNAL_H
#define RAFT_JOURNAL_H

#include <google/protobuf/repeated_field.h>
#include <memory>
#include <string>
#include <utility>

#include "proto_cc/command.pb.h"
#include "proto_cc/raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/persister.h"

namespace e8 {

/**
 * @brief The RaftCommitListener class Used by the state machine to respond to the committed log
 * entries when they are sufficiently replicated.
 */
class RaftCommitListener {
  public:
    /**
     * @brief ApplyCommit When a log entry has been committed, this will be called so that the state
     * machine can change its states according to the command carried in the log.
     */
    virtual void Apply(CommandEntry const &entry) = 0;
};

/**
 * @brief The RaftJournal class Handles logging and commit in a thread-safe and crash-safe manner.
 */
class RaftJournal {
  public:
    /**
     * @brief RaftJournal Tells the object o manage on top of the persister.
     *
     * @param commit_listener The listener will be notified with committed log entries.
     * @param persister The persistent device which holds the log data and manages persistence.
     */
    RaftJournal(std::shared_ptr<RaftPersister> const &persister,
                std::shared_ptr<RaftCommitListener> const &commit_listener);
    ~RaftJournal();

    /**
     * @brief AppendLog Appends a new log to the local log source and persists the updated local
     * logs.
     *
     * @return The index at which the log entry has appended.
     */
    unsigned AppendLog(LogEntry const &log_entry);

    /**
     * @brief MergeForeignLogs Overwrites and appends the local logs with foreign logs starting from
     * the "from" index. Note that the "from" index can set at the end of the local logs but no more
     * further, otherwise, it's unresolvable and will interrupt the merge and return false. It also
     * checks if the log term preceding the "from" index of the local logs agrees with the foreign.
     * A disagreement means the previous logs are unresolved and will also make the call return
     * false immediately. After the merge operation, logs that are different from the foreign source
     * and those come after these entries will be deleted and replaced with the foreign entries.
     * After the local log content is modified, it persists the updated local logs.
     *
     * @param from The starting position of the local log source to overwrite.
     * @param foreign_log_entries Partial foreign log entries to overwrite from.
     *
     * @return Returns if the merge is successful or not. An unsucessful merge does not change the
     * local journal.
     */
    bool MergeForeignLogs(unsigned from,
                          google::protobuf::RepeatedPtrField<LogEntry> const &foreign_log_entries,
                          RaftTerm preceding_log_term);

    /**
     * @brief Stale Given the lastest log progress and highest log term the foreign journal have,
     * deduce if it misses any of the local log entries that have committed or are about to commit.
     * Note that, it's not the same as checking if the foreign journal misses any of the local log
     * entries. For log entries that were added in previous terms, the foreign journal could not
     * have contained them but still consider fresh. That is because those log entries are subject
     * to potential overwrite and are not, for the moment, sending to the state machine.
     */
    bool Stale(LogSourceLiveness const &foreign_liveness);

    /**
     * @brief PushCommitProgress Synchronizes local commit progress with the commit progress
     * requested by the leader and pushes all the committed log commands to the state machine
     * connected to by the commit_listener (passed in during construction).
     *
     * @param safe_commit_progress The highest commit progress where all logs within the range
     * [0, safe_commit_progress) has all been replicated by a quorum as well as here locally in the
     * journal.
     */
    void PushCommitProgress(unsigned safe_commit_progress);

  private:
    std::shared_ptr<RaftPersister> persister_;

    std::shared_ptr<RaftCommitListener> commit_listener_;
    unsigned commit_progress_;
};

} // namespace e8

#endif // RAFT_JOURNAL_H
