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

#include "replication/raft/common_types.h"
#include "replication/raft/persister.h"
#include "replication/raft/role_at_term.h"

namespace e8 {
namespace {

std::string TermUpgradeReasonString(RoleAtTerm::TermUpgradeReason reason) {
    switch (reason) {
    case RoleAtTerm::LACK_OF_HEARTBEAT:
        return "LACK_OF_HEARTBEAT";
    case RoleAtTerm::LOST_AN_ELECTION:
        return "LOST_AN_ELECTION";
    case RoleAtTerm::WON_AN_ELECTION:
        return "WON_AN_ELECTION";
    case RoleAtTerm::ENCOUNTERED_HIGHER_TERM_MESSAGE:
        return "ENCOUNTERED_HIGHER_TERM_MESSAGE";
    default:
        return "INVALID_REASON";
    }
}

} // namespace

RoleAtTerm::RoleAtTerm(RaftPersister *persister) : persister_(persister) {}

RoleAtTerm::~RoleAtTerm() {}

std::pair<RaftTerm, RaftRole> RoleAtTerm::TermAndRole() const {
    std::lock_guard<RaftPersister> const lock(*persister_);

    return persister_->TermAndRole();
}

bool RoleAtTerm::UpgradeTerm(RaftMachineAddress node, RaftTerm new_term, TermUpgradeReason reason) {
    std::lock_guard<RaftPersister> const lock(*persister_);

    auto [current_term, current_role] = persister_->TermAndRole();

    if (new_term < current_term) {
        return false;
    }

    if (new_term == current_term && reason == ENCOUNTERED_HIGHER_TERM_MESSAGE) {
        // new_term must be higher than the existing value.
        return false;
    }

    RaftRole new_role;

    switch (current_role) {
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
            std::cerr << "At node=" << node
                      << ": illegal term upgrade reason=" << TermUpgradeReasonString(reason)
                      << std::endl;
            return false;
        }
        }

        break;
    }
    case RAFT_CANDIDATE: {
        switch (reason) {
        case LOST_AN_ELECTION: {
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
            std::cerr << "At node=" << node
                      << ": illegal term upgrade reason=" << TermUpgradeReasonString(reason)
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
            std::cerr << "At node=" << node
                      << ": illegal term upgrade reason=" << TermUpgradeReasonString(reason)
                      << std::endl;
            return false;
        }
        }

        break;
    }
    }

    std::cout << "At node=" << node << ": term=" << current_term << "->" << new_term
              << " role=" << RoleString(current_role) << "->" << RoleString(new_role)
              << " reason=" << TermUpgradeReasonString(reason) << std::endl;

    persister_->SetTermAndRole(std::make_pair(new_term, new_role));
    persister_->Persist();

    return true;
}

void RoleAtTerm::SetTermAndRole(RaftTerm term, RaftRole role) {
    std::lock_guard<RaftPersister> const lock(*persister_);

    persister_->SetTermAndRole(std::make_pair(term, role));
    persister_->Persist();
}

} // namespace e8
