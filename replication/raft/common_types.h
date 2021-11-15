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

#ifndef RAFT_COMMON_TYPES_H
#define RAFT_COMMON_TYPES_H

#include <string>

namespace e8 {

// Uniquely identifies a raft node. Format: ipv4/ipv6:port.
using RaftMachineAddress = std::string;

/**
 * A valid raft term is a non-negative integer. But it can be used to indicate error by setting it
 * to be negative.
 */
using RaftTerm = int64_t;

// A logical offset for marking locations over the Raft journal.
using RaftLogOffset = uint64_t;

/**
 * @brief The RaftRole enum Defines the role status a node could have at some moment in time.
 */
enum RaftRole {
    RAFT_INVALID_ROLE,
    RAFT_FOLLOWER,
    RAFT_CANDIDATE,
    RAFT_LEADER,
};

inline std::string RoleString(RaftRole role) {
    switch (role) {
    case RAFT_LEADER:
        return "RAFT_LEADER";
    case RAFT_CANDIDATE:
        return "RAFT_CANDIDATE";
    case RAFT_FOLLOWER:
        return "RAFT_FOLLOWER";
    case RAFT_INVALID_ROLE:
    default:
        return "RAFT_INVALID_ROLE";
    }
}

} // namespace e8

#endif // RAFT_COMMON_TYPES_H
