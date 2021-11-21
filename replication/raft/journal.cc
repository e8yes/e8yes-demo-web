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
#include <google/protobuf/repeated_field.h>
#include <limits>
#include <mutex>
#include <optional>
#include <string>
#include <utility>

#include "proto_cc/command.pb.h"
#include "proto_cc/raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/journal.h"
#include "replication/raft/persister.h"

namespace e8 {
namespace {

void TruncateLogsTo(RaftLogOffset new_size, google::protobuf::RepeatedPtrField<LogEntry> *logs) {
    while (static_cast<RaftLogOffset>(logs->size()) > new_size) {
        logs->RemoveLast();
    }
}

void TruncateLogsBefore(RaftLogOffset new_front,
                        google::protobuf::RepeatedPtrField<LogEntry> *logs) {
    assert(new_front < static_cast<RaftLogOffset>(logs->size()));
    for (unsigned i = new_front; i < static_cast<RaftLogOffset>(logs->size()); ++i) {
        (*logs)[i - new_front] = (*logs)[i];
    }

    TruncateLogsTo(logs->size() - new_front, logs);
}

RaftLogOffset ToLogical(RaftLogOffset physical, RaftLogOffset snapshot_progress) {
    return snapshot_progress + physical;
}

RaftLogOffset ToPhysical(RaftLogOffset logical, RaftLogOffset snapshot_progress) {
    assert(logical >= snapshot_progress);
    return logical - snapshot_progress;
}

} // namespace

RaftCommitListener::RaftCommitListener() {}

RaftCommitListener::~RaftCommitListener() {}

RaftLogOffset RaftCommitListener::PreferredSnapshotFrequency() const {
    return std::numeric_limits<RaftLogOffset>::max();
}

void RaftCommitListener::Save() const {}

void RaftCommitListener::Restore() {}

void RaftCommitListener::Reset() {}

RaftJournal::RaftJournal(RaftPersister *persister, RaftCommitListener *commit_listener)
    : persister_(persister), commit_listener_(commit_listener), commit_progress_(0) {
    assert(persister_ != nullptr);
    assert(commit_listener_ != nullptr);

    // Restores states from the snapshot on startup.
    commit_listener_->Restore();
}

RaftJournal::~RaftJournal() {}

RaftLogOffset RaftJournal::ToLogical(RaftLogOffset physical) const {
    return e8::ToLogical(physical, persister_->SnapshotProgress());
}

RaftLogOffset RaftJournal::ToPhysical(RaftLogOffset logical) const {
    return e8::ToPhysical(logical, persister_->SnapshotProgress());
}

RaftLogOffset RaftJournal::AppendLog(LogEntry const &log_entry) {
    std::lock_guard<RaftPersister> const guard(*persister_);

    *persister_->LogEntriesRef()->Add() = log_entry;
    persister_->Persist();
    return this->ToLogical(persister_->LogEntriesRef()->size() - 1);
}

LogSourceLiveness RaftJournal::Liveness() const {
    std::lock_guard<RaftPersister> const guard(*persister_);

    LogSourceLiveness liveness;
    liveness.set_log_progress(this->ToLogical(persister_->LogEntriesRef()->size()));

    if (persister_->LogEntriesRef()->empty()) {
        liveness.set_highest_log_term(persister_->SnapshotHighestLogTerm());
    } else {
        liveness.set_highest_log_term(persister_->LogEntriesRef()->rbegin()->term());
    }

    return liveness;
}

bool RaftJournal::Import(RaftLogOffset from, RaftLogOffset foreign_snapshot_progress,
                         google::protobuf::RepeatedPtrField<LogEntry> const &foreign_log_entries,
                         RaftTerm preceding_log_term) {
    std::lock_guard<RaftPersister> const guard(*persister_);

    if (from > this->ToLogical(static_cast<RaftLogOffset>(persister_->LogEntriesRef()->size()))) {
        // Importing from too far in the future. There are missing entries before the first foreign
        // log. The caller needs to supply earlier logs to help this local journal catch up.
        return false;
    }

    if (from > this->ToLogical(0) &&
        preceding_log_term != (*persister_->LogEntriesRef())[from - 1].term()) {
        // There are some older log entries being inconsistent with the foreign journal. Refuses to
        // import.
        return false;
    }

    for (RaftLogOffset i = 0; i < static_cast<RaftLogOffset>(foreign_log_entries.size()); i++) {
        RaftLogOffset logical_target_index = e8::ToLogical(from + i, foreign_snapshot_progress);

        if (logical_target_index < this->ToLogical(0)) {
            continue;
        }

        if (logical_target_index < commit_progress_) {
            // It's disastrous to override committed logs with different entries.
            assert((*persister_->LogEntriesRef())[this->ToPhysical(logical_target_index)].term() ==
                   foreign_log_entries[i].term());
            continue;
        }

        if (logical_target_index >=
            this->ToLogical(static_cast<RaftLogOffset>(persister_->LogEntriesRef()->size()))) {
            *persister_->LogEntriesRef()->Add() = foreign_log_entries[i];
            continue;
        }

        if ((*persister_->LogEntriesRef())[this->ToPhysical(logical_target_index)].term() !=
            foreign_log_entries[i].term()) {
            // Trim everything beyond target_index because the foreign logs contain different
            // entries starting at this point.
            (*persister_->LogEntriesRef())[this->ToPhysical(logical_target_index)] =
                foreign_log_entries[i];
            TruncateLogsTo(this->ToPhysical(logical_target_index) + 1, persister_->LogEntriesRef());
        } else {
            // Doesn't have to do anything if the two entries are the same.
        }
    }

    persister_->Persist();

    return true;
}

bool RaftJournal::Export(RaftLogOffset start, RaftLogOffset *snapshot_progress,
                         google::protobuf::RepeatedPtrField<LogEntry> *output_buffer,
                         RaftTerm *preceding_log_term) const {
    std::lock_guard<RaftPersister> const guard(*persister_);

    *snapshot_progress = persister_->SnapshotProgress();

    if (start > this->ToLogical(static_cast<RaftLogOffset>(persister_->LogEntriesRef()->size()))) {
        // Exporting too far in the future.
        *preceding_log_term = RaftTerm();
        return false;
    }

    assert(start >=
           this->ToLogical(0)); // Can't ever export anything that has gone to the snapshot.

    for (RaftLogOffset i = this->ToPhysical(start);
         i < static_cast<RaftLogOffset>(persister_->LogEntriesRef()->size()); ++i) {
        *output_buffer->Add() = (*persister_->LogEntriesRef())[i];
    }

    if (start == this->ToLogical(0)) {
        *preceding_log_term = persister_->SnapshotHighestLogTerm();
    } else {
        *preceding_log_term = (*persister_->LogEntriesRef())[start - 1].term();
    }

    return true;
}

RaftLogOffset RaftJournal::EndWithTerm(RaftTerm term, RaftLogOffset end) const {
    std::lock_guard<RaftPersister> const guard(*persister_);

    RaftLogOffset journal_logical_end =
        this->ToLogical(static_cast<RaftLogOffset>(persister_->LogEntriesRef()->size()));
    if (end > journal_logical_end) {
        end = journal_logical_end;
    }

    for (RaftLogOffset i = this->ToPhysical(end); i > 0; --i) {
        if ((*persister_->LogEntriesRef())[i - 1].term() == term) {
            return this->ToLogical(i);
        }
    }

    return this->ToLogical(0);
}

bool RaftJournal::Stale(LogSourceLiveness const &foreign_liveness) {
    std::lock_guard<RaftPersister> const guard(*persister_);

    RaftTerm highest_log_term;
    if (persister_->LogEntriesRef()->empty()) {
        highest_log_term = persister_->SnapshotHighestLogTerm();
    } else {
        highest_log_term = persister_->LogEntriesRef()->rbegin()->term();
    }

    if (foreign_liveness.highest_log_term() > highest_log_term) {
        return false;
    }

    if (foreign_liveness.highest_log_term() == highest_log_term) {
        return this->ToLogical(static_cast<RaftLogOffset>(persister_->LogEntriesRef()->size())) >
               static_cast<RaftLogOffset>(foreign_liveness.log_progress());
    }

    return true;
}

void RaftJournal::PushCommitProgress(RaftLogOffset safe_commit_progress,
                                     RaftLogOffset full_commit_progress) {
    std::lock_guard<RaftPersister> const guard(*persister_);

    if (safe_commit_progress <= commit_progress_) {
        // Nothing to commit.
        return;
    }

    // Can't commit beyond what is replicated to the local journal.
    assert(safe_commit_progress <= static_cast<RaftLogOffset>(persister_->LogEntriesRef()->size()));

    // Pushes logs to the state machine.
    while (commit_progress_ < safe_commit_progress) {
        commit_listener_->Apply((*persister_->LogEntriesRef())[commit_progress_].entry());
        ++commit_progress_;
    }

    assert(full_commit_progress >= this->ToLogical(0));
    assert(full_commit_progress <= static_cast<RaftLogOffset>(persister_->LogEntriesRef()->size()));
    assert(commit_listener_->PreferredSnapshotFrequency() > 0);
    if (full_commit_progress - this->ToLogical(0) >=
        commit_listener_->PreferredSnapshotFrequency()) {
        // Takes a new snapshot.
        commit_listener_->Save();

        persister_->SetSnapshotProgress(full_commit_progress);
        persister_->SetSnapshotHighestLogTerm(
            (*persister_->LogEntriesRef())[full_commit_progress - 1].term());

        TruncateLogsBefore(full_commit_progress, persister_->LogEntriesRef());

        persister_->Persist();
    }
}

} // namespace e8
