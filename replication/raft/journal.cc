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
#include <fstream>
#include <google/protobuf/repeated_field.h>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

#include "proto_cc/command.pb.h"
#include "proto_cc/raft.pb.h"
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

RaftJournal::RaftJournal(std::shared_ptr<RaftPersister> const &persister,
                         std::shared_ptr<RaftCommitListener> const &commit_listener)
    : persister_(persister), commit_listener_(commit_listener), commit_progress_(0),
      listener_progress_(0) {}

RaftJournal::~RaftJournal() {}

void RaftJournal::AppendLog(LogEntry const &log_entry) {
    std::lock_guard<RaftPersister> const guard(*persister_);

    *persister_->LogEntriesRef()->Add() = log_entry;

    persister_->Persist();
}

void RaftJournal::MergeForeignLogs(
    unsigned from, google::protobuf::RepeatedField<LogEntry> const &foreign_log_entries) {
    std::lock_guard<RaftPersister> const guard(*persister_);

    // start_index shouldn't be after the end of the local logs.
    assert(from <= static_cast<unsigned>(foreign_log_entries.size()));

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
}

} // namespace e8
