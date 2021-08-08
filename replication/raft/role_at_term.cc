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
#include <cstdint>
#include <iostream>
#include <mutex>
#include <string>
#include <utility>

#include "replication/raft/role_at_term.h"

namespace e8 {
namespace {

std::string RoleString(RaftRole role) {
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

} // namespace

RoleAtTerm::RoleAtTerm() : lock_(std::make_unique<std::mutex>()), term_(0), role_(RAFT_FOLLOWER) {}

RoleAtTerm::~RoleAtTerm() {}

std::pair<RaftTerm, RaftRole> RoleAtTerm::TermAndRole() const {
    std::lock_guard<std::mutex> const lock(*lock_);

    return std::make_pair(term_, role_);
}

bool RoleAtTerm::UpgradeTerm(RaftNodeId node_id, RaftTerm new_term, TermUpgradeReason reason) {
    std::lock_guard<std::mutex> const lock(*lock_);

    if (new_term < term_) {
        return false;
    }

    if (new_term == term_ && reason == ENCOUNTERED_HIGHER_TERM_MESSAGE) {
        // new_term must be higher than the existing value.
        return false;
    }

    RaftRole new_role;

    switch (role_) {
    case RAFT_INVALID_ROLE: {
        assert(false);
        break;
    }
    case RAFT_FOLLOWER: {
        switch (reason) {
        case LACK_OF_HEARTBEAT: {
            new_role = RAFT_CANDIDATE;
            break;
        }
        case ENCOUNTERED_HIGHER_TERM_MESSAGE: {
            new_role = RAFT_FOLLOWER;
            break;
        }
        default: {
            std::cerr << "At node=" << node_id << ": illegal term upgrade reason=" << reason
                      << std::endl;
            return false;
        }
        }

        break;
    }
    case RAFT_CANDIDATE: {
        switch (reason) {
        case SPLIT_VOTES:
        case LACK_OF_HEARTBEAT: {
            new_role = RAFT_CANDIDATE;
            break;
        }
        case WON_AN_ELECTION: {
            new_role = RAFT_LEADER;
            break;
        }
        case ENCOUNTERED_HIGHER_TERM_MESSAGE: {
            new_role = RAFT_FOLLOWER;
            break;
        }
        default: {
            std::cerr << "At node=" << node_id << ": illegal term upgrade reason=" << reason
                      << std::endl;
            return false;
        }
        }

        break;
    }
    case RAFT_LEADER: {
        switch (reason) {
        case LACK_OF_HEARTBEAT: {
            new_role = RAFT_LEADER;
            break;
        }
        case ENCOUNTERED_HIGHER_TERM_MESSAGE: {
            new_role = RAFT_FOLLOWER;
            break;
        }
        default: {
            std::cerr << "At node=" << node_id << ": illegal term upgrade reason=" << reason
                      << std::endl;
            return false;
        }
        }

        break;
    }
    }

    std::cout << "At node=" << node_id << ": term=" << term_ << "->" << new_term
              << " role=" << RoleString(role_) << "->" << RoleString(new_role)
              << " reason=" << reason << std::endl;

    term_ = new_term;
    role_ = new_role;

    return true;
}

} // namespace e8
