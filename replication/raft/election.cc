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

#include "replication/raft/common_types.h"
#include "replication/raft/election.h"

namespace e8 {

RaftVotingRecord::RaftVotingRecord() {}

RaftVotingRecord::~RaftVotingRecord() {}

void RaftVotingRecord::lock() { return lock_.lock(); }

void RaftVotingRecord::unlock() { return lock_.unlock(); }

void RaftVotingRecord::VoteFor(RaftMachineAddress candidate_address) {
    last_voted_for_ = candidate_address;
}

std::optional<RaftMachineAddress> RaftVotingRecord::LastVotedFor() const { return last_voted_for_; }

} // namespace e8
