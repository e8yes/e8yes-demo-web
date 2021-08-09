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

#ifndef RAFT_ROLE_AT_TERM_H
#define RAFT_ROLE_AT_TERM_H

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "replication/raft/common_types.h"
#include "replication/raft/persister.h"

namespace e8 {

/**
 * @brief The RoleAtTerm class It keeps track of what role the node should play at the latest term
 * and persists any changes to the role or term. Raft role is coupled with the term number. It
 * allows atomic access to the term-role pair and facilitates monotonic term increments and proper
 * role transition based on term upgrade events. This object is thread safe.
 */
class RoleAtTerm {
  public:
    enum TermUpgradeReason {
        LACK_OF_HEARTBEAT,
        SPLIT_VOTES,
        WON_AN_ELECTION,
        ENCOUNTERED_HIGHER_TERM_MESSAGE,
    };

    /**
     * @brief RoleAtTerm The role and term is recovered from the persister. Any future changes to
     * either term or role will be immediately saved by this persister.
     */
    RoleAtTerm(std::shared_ptr<RaftPersister> const &persister);
    ~RoleAtTerm();

    /**
     * @brief TermAndRole Atomically reads the current term-role pair.
     */
    std::pair<RaftTerm, RaftRole> TermAndRole() const;

    /**
     * @brief UpgradeTerm It helps put the holder into the correct raft term-role pair by detecting
     * the transition condition. Or more precisely, it implements the state machine:
     *
     *                                             == Re-elction ==
     *                                            ||              ||
     *                   === times out ====> {CANDIDATE} <=========
     *                  ||                     ||    ||
     * Starts up => {FOLLOWER} <= higher term ==      == majority vote => {LEADER}
     *                  |^|                                                  ||
     *                   ==================== higher term ====================
     *
     * @param node_id The raft node that is triggering the term upgrade (for logging purposes).
     * @param new_term The term to ugrade to.
     * @param reason The event that triggers the term upgrade.
     *
     * @return Returns true when the new term has overwritten the existing value and the role has
     * properly transitioned accordingly. This happens when the new_term is at least as high as the
     * current term and must be higher when the triggering event is ENCOUNTERED_HIGHER_TERM_MESSAGE.
     */
    bool UpgradeTerm(RaftMachineAddress node, RaftTerm new_term, TermUpgradeReason reason);

  private:
    std::shared_ptr<RaftPersister> persister_;
};

} // namespace e8

#endif // RAFT_ROLE_AT_TERM_H
