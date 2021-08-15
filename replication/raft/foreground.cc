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

#include "proto_cc/command.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/context.h"
#include "replication/raft/foreground.h"

namespace e8 {

RaftForeground::RaftForeground(RaftContext *context) : context_(context) {}

RaftForeground::~RaftForeground() {}

RaftForeground::BoardcastResult::BoardcastResult(
    std::optional<RaftMachineAddress> const &retry_node)
    : retry_node(retry_node), log_index(0), term(0) {}

RaftForeground::BoardcastResult::BoardcastResult(unsigned log_index, RaftTerm term)
    : retry_node(std::nullopt), log_index(log_index), term(term) {}

RaftForeground::BoardcastResult
RaftForeground::BoardcastCommand(CommandEntry const &command_entry) {
    std::lock_guard<std::mutex> guard(lock_);

    auto [term, role] = context_->role_at_term->TermAndRole();

    if (role != RAFT_LEADER) {
        std::optional<RaftMachineAddress> possible_leader = context_->voting_record->LastVotedFor();

        if (possible_leader->empty()) {
            // Retry the call on this node for the time being since we don't know who the leader is.
            return BoardcastResult(context_->me);
        } else {
            // The node we voted for could possibly be the leader if it wins the election.
            return BoardcastResult(*possible_leader);
        }
    }

    // Adds the log entry to the journal and let the background leader task picks up and replicate
    // among the peers.
    LogEntry log;
    log.set_term(term);
    *log.mutable_entry() = command_entry;

    unsigned log_index = context_->journal->AppendLog(log);
    context_->leader_schedule->WakeUp();

    return BoardcastResult(log_index, term);
}

std::pair<RaftTerm, RaftRole> RaftForeground::TermAndRole() const {
    return context_->role_at_term->TermAndRole();
}

} // namespace e8
