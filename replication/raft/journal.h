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
#include <optional>
#include <string>
#include <utility>

#include "proto_cc/command.pb.h"
#include "proto_cc/raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/persister.h"

namespace e8 {

/**
 * @brief The RaftCommitListener class Used by the state machine to respond to the committed log
 * entries when they are sufficiently replicated. The listener must respond and return quickly. In
 * principle, its processing time must be *on average* an order of magnitude less than the heartbeat
 * interval, though occasional spikes are manageable.
 */
class RaftCommitListener {
  public:
    RaftCommitListener();
    virtual ~RaftCommitListener();

    /**
     * @brief ApplyCommit When a log entry has been committed, this will be called so that the state
     * machine can change its states according to the command carried in the log.
     */
    virtual void Apply(CommandEntry const &entry) = 0;

    /**
     * @brief PreferredSnapshotFrequency Instructs the journal in how often (in terms of "every N
     * fully committed log entries) should it take a snapshot of the commit listener's current
     * state. This value must be greater than 0. Note, the journal doesn't always follow this
     * frequency, but only to take it as the minimum.
     *
     * The default implementation returns a large enough value so that snapshot won't ever occur.
     */
    virtual RaftLogOffset PreferredSnapshotFrequency() const;

    /**
     * @brief Save Discards the previous snapshot file, if there is any, and creates a snapshot of
     * the current states.
     *
     * The default implementation does nothing.
     */
    virtual void Save() const;

    /**
     * @brief Restore Restores the states from a snapshot file if there is one. Otherwise, leaves
     * the states unchanged.
     *
     * The default implementation does nothing.
     */
    virtual void Restore();

    /**
     * @brief Reset When the Raft node got shutdown in a integration testing situation, this
     * function gets called to undo all the applied commands.
     *
     * The default implementation does nothing.
     */
    virtual void Reset();
};

/**
 * @brief The RaftJournal class Handles local logging and commit in a thread-safe and crash-safe
 * manner.
 */
class RaftJournal {
  public:
    /**
     * @brief RaftJournal Tells the object o manage on top of the persister.
     *
     * @param persister The persistent device which holds the log data and manages persistence.
     * @param commit_listener The listener will be notified with committed log entries.
     */
    RaftJournal(RaftPersister *persister, RaftCommitListener *commit_listener);
    ~RaftJournal();

    /**
     * @brief AppendLog Appends a new log to the local log source and persists the updated local
     * logs.
     *
     * @return The index at which the log entry has appended.
     */
    RaftLogOffset AppendLog(LogEntry const &log_entry);

    /**
     * @brief Import Overwrites and appends the local logs with foreign logs starting from the
     * "from" index. Note that the "from" index can set at the end of the local logs but no more
     * further. Otherwise, it's unresolvable, and the function will interrupt the merge and return
     * false. It also checks if the log term preceding the "from" index of the local logs agrees
     * with the foreign. A disagreement means the previous logs are unresolved and will make the
     * call return false immediately. After the merge operation, logs that are different from the
     * foreign source and those come after these entries will be deleted and replaced with the
     * foreign entries. After the local log content is modified, it persists the updated local logs.
     *
     * @param from The starting position of the local log source to overwrite.
     * @param foreign_snapshot_progress The snapshot progress of the foreign log source.
     * @param foreign_log_entries Partial foreign log entries to overwrite from.
     *
     * @return Returns if the merge is successful or not. An unsucessful merge does not change the
     * local journal.
     */
    bool Import(RaftLogOffset from, RaftLogOffset foreign_snapshot_progress,
                google::protobuf::RepeatedPtrField<LogEntry> const &foreign_log_entries,
                RaftTerm preceding_log_term);

    /**
     * @brief Export Copies the specified portion of the local log entries to the output_buffer. It
     * will not touch what the ouput_buffer already has.
     *
     * @param start The starting index of the logs to copy from. It's valid to set the start index
     * to the end of the journal. In that case, nothing will be copied. However, if it's beyond the
     * end of the journal, the function will return false and the output_buffer doesn't get
     * modified. The value of preceding_log_term shouldn't be used if the function doesn't succeed.
     * @param snapshot_progress The journal's current snapshot progress. This is used to determine
     * the logical index of the exported log entries.
     * @param output_buffer The buffer array to which the logs are copied.
     * @param preceding_log_term The log term of the entry just before the start index. This is used
     * by the importing end to test if the portion of logs is resolvable. If the start index is 0,
     * the preceding_log_term will be set to 0.
     *
     * @return Whether the start index is valid.
     */
    bool Export(RaftLogOffset start, RaftLogOffset *snapshot_progress,
                google::protobuf::RepeatedPtrField<LogEntry> *output_buffer,
                RaftTerm *preceding_log_term) const;

    /**
     * @brief EndWithTerm Searches for the greatest boundary B in the interval [0, end] where the
     * log term of the entries in the range [0, B) is less than or equal to the specified term.
     * Consequently, the smallest value of B is zero.
     *
     * @return The greatest boundary.
     */
    RaftLogOffset EndWithTerm(RaftTerm term, RaftLogOffset end) const;

    /**
     * @brief Liveness Returns the liveness states of the current journal. It could be used to
     * compare against other journal to determine how up-to-date the journal is.
     */
    LogSourceLiveness Liveness() const;

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
     * @param full_commit_progress
     */
    void PushCommitProgress(RaftLogOffset safe_commit_progress, RaftLogOffset full_commit_progress);

  private:
    RaftLogOffset ToLogical(RaftLogOffset physical) const;
    RaftLogOffset ToPhysical(RaftLogOffset logical) const;

    RaftPersister *persister_;
    RaftCommitListener *commit_listener_;
    RaftLogOffset commit_progress_;
};

} // namespace e8

#endif // RAFT_JOURNAL_H
