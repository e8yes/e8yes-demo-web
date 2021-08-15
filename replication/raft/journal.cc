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
#include <mutex>
#include <string>
#include <utility>

#include "proto_cc/command.pb.h"
#include "proto_cc/raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/journal.h"
#include "replication/raft/persister.h"

namespace e8 {
namespace {

void TruncateLogsTo(unsigned new_size, google::protobuf::RepeatedPtrField<LogEntry> *logs) {
    while (static_cast<unsigned>(logs->size()) > new_size) {
        logs->RemoveLast();
    }
}

} // namespace

RaftCommitListener::RaftCommitListener() {}

RaftCommitListener::~RaftCommitListener() {}

RaftJournal::RaftJournal(RaftPersister *persister, RaftCommitListener *commit_listener)
    : persister_(persister), commit_listener_(commit_listener), commit_progress_(0) {
    assert(persister_ != nullptr);
    assert(commit_listener_ != nullptr);
}

RaftJournal::~RaftJournal() {}

unsigned RaftJournal::AppendLog(LogEntry const &log_entry) {
    std::lock_guard<RaftPersister> const guard(*persister_);

    *persister_->LogEntriesRef()->Add() = log_entry;

    persister_->Persist();

    return persister_->LogEntriesRef()->size() - 1;
}

LogSourceLiveness RaftJournal::Liveness() const {
    std::lock_guard<RaftPersister> const guard(*persister_);

    LogSourceLiveness liveness;
    liveness.set_log_progress(persister_->LogEntriesRef()->size());

    if (persister_->LogEntriesRef()->empty()) {
        liveness.set_highest_log_term(0);
    } else {
        liveness.set_highest_log_term(persister_->LogEntriesRef()->rbegin()->term());
    }

    return liveness;
}

bool RaftJournal::Import(unsigned from,
                         google::protobuf::RepeatedPtrField<LogEntry> const &foreign_log_entries,
                         RaftTerm preceding_log_term) {
    std::lock_guard<RaftPersister> const guard(*persister_);

    if (from > static_cast<unsigned>(foreign_log_entries.size())) {
        // Replicating too far in the future, there are missing entries.
        return false;
    }

    if (from != 0 && preceding_log_term != (*persister_->LogEntriesRef())[from - 1].term()) {
        // There are some previous log entries which are unresolved.
        return false;
    }

    for (unsigned i = 0; i < static_cast<unsigned>(foreign_log_entries.size()); i++) {
        unsigned target_index = from + i;

        if (target_index < commit_progress_) {
            // It's disastrous to override committed logs with different entries.
            assert((*persister_->LogEntriesRef())[target_index].term() ==
                   foreign_log_entries[i].term());
        }

        if (target_index >= static_cast<unsigned>(persister_->LogEntriesRef()->size())) {
            *persister_->LogEntriesRef()->Add() = foreign_log_entries[i];
            continue;
        }

        if ((*persister_->LogEntriesRef())[target_index].term() != foreign_log_entries[i].term()) {
            // Trim everything beyond target_index because the foreign log contains different
            // entries starting at this point.
            (*persister_->LogEntriesRef())[target_index] = foreign_log_entries[i];
            TruncateLogsTo(target_index + 1, persister_->LogEntriesRef());
        } else {
            // Doesn't have to do anything if the two entries are the same.
        }
    }

    persister_->Persist();

    return true;
}

bool RaftJournal::Export(unsigned start,
                         google::protobuf::RepeatedPtrField<LogEntry> *output_buffer,
                         RaftTerm *preceding_log_term) const {
    std::lock_guard<RaftPersister> const guard(*persister_);

    if (start > static_cast<unsigned>(persister_->LogEntriesRef()->size())) {
        *preceding_log_term = RaftTerm();
        return false;
    }

    for (unsigned i = start; i < static_cast<unsigned>(persister_->LogEntriesRef()->size()); ++i) {
        *output_buffer->Add() = (*persister_->LogEntriesRef())[i];
    }

    if (start == 0) {
        *preceding_log_term = RaftTerm();
    } else {
        *preceding_log_term = (*persister_->LogEntriesRef())[start - 1].term();
    }

    return true;
}

unsigned RaftJournal::EndWithTerm(RaftTerm term, unsigned end) const {
    std::lock_guard<RaftPersister> const guard(*persister_);

    if (end > static_cast<unsigned>(persister_->LogEntriesRef()->size())) {
        end = persister_->LogEntriesRef()->size();
    }

    for (unsigned i = end; i > 0; --i) {
        if ((*persister_->LogEntriesRef())[i - 1].term() == term) {
            return i;
        }
    }

    return 0;
}

bool RaftJournal::Stale(LogSourceLiveness const &foreign_liveness) {
    std::lock_guard<RaftPersister> const guard(*persister_);

    if (persister_->LogEntriesRef()->empty()) {
        // The foreign journal can not be stale compared to an empty journal.
        return false;
    }

    RaftTerm highest_log_term = persister_->LogEntriesRef()->rbegin()->term();
    if (foreign_liveness.highest_log_term() > highest_log_term) {
        return false;
    }

    if (foreign_liveness.highest_log_term() == highest_log_term) {
        return foreign_liveness.log_progress() > persister_->LogEntriesRef()->size();
    }

    return true;
}

void RaftJournal::PushCommitProgress(unsigned safe_commit_progress) {
    std::lock_guard<RaftPersister> const guard(*persister_);

    if (safe_commit_progress <= commit_progress_) {
        // Nothing to commit.
        return;
    }

    // Can't commit beyond what is replicated to the local journal.
    assert(safe_commit_progress <= static_cast<unsigned>(persister_->LogEntriesRef()->size()));

    // Pushes logs to the state machine.
    while (commit_progress_ < safe_commit_progress) {
        commit_listener_->Apply((*persister_->LogEntriesRef())[commit_progress_].entry());
        ++commit_progress_;
    }
}

} // namespace e8
