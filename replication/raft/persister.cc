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
#include <string>

#include "proto_cc/raft.pb.h"
#include "replication/raft/common_types.h"

#include "replication/raft/persister.h"

namespace e8 {

RaftPersister::RaftPersister(std::string const &file_path) {
    this->Recover(file_path);

    file_.open(file_path, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    assert(file_.is_open());

    this->Persist();
}

RaftPersister::~RaftPersister() {}

void RaftPersister::lock() { lock_.lock(); }

void RaftPersister::unlock() { lock_.unlock(); }

std::pair<RaftTerm, RaftRole> RaftPersister::TermAndRole() {
    return std::make_pair(RaftTerm(states_.term()), RaftRole(states_.role()));
}

void RaftPersister::SetTermAndRole(std::pair<RaftTerm, RaftRole> const &term_and_role) {
    auto [term, role] = term_and_role;
    states_.set_term(term);
    states_.set_role(role);
}

google::protobuf::RepeatedPtrField<LogEntry> *RaftPersister::LogEntriesRef() {
    return states_.mutable_log_entries();
}

void RaftPersister::Persist() {
    assert(file_.is_open());
    states_.SerializeToOstream(&file_);
    file_.flush();
}

void RaftPersister::Recover(std::string const &file_path) {
    std::fstream recovery_file(file_path, std::ios::in | std::ios::binary);
    if (!recovery_file.is_open()) {
        states_.set_term(0);
        states_.set_role(RAFT_FOLLOWER);
        return;
    }

    states_.ParseFromIstream(&recovery_file);
}

} // namespace e8
