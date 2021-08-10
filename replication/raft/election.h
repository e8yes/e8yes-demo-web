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

#ifndef RAFT_ELECTION_H
#define RAFT_ELECTION_H

#include <mutex>
#include <optional>

#include "replication/raft/common_types.h"

namespace e8 {

/**
 * @brief The RaftVotingRecord class A mutually exclusive device to record who the node has
 * most recently voted for. The user does not necessarily have to guard this object.
 */
class RaftVotingRecord {
  public:
    RaftVotingRecord();
    ~RaftVotingRecord();

    void lock();
    void unlock();

    /**
     * @brief VoteFor Records that the node has casted ballot for the candidate.
     */
    void VoteFor(RaftMachineAddress candidate_address);

    /**
     * @brief LastVotedFor Returns who the node has last voted for if there has been any.
     */
    std::optional<RaftMachineAddress> LastVotedFor() const;

  private:
    std::mutex lock_;
    std::optional<RaftMachineAddress> last_voted_for_;
};

} // namespace e8

#endif // RAFT_ELECTION_H
