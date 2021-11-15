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

#ifndef RAFT_FOREGROUND_H
#define RAFT_FOREGROUND_H

#include <mutex>
#include <optional>
#include <utility>

#include "proto_cc/command.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/context.h"

namespace e8 {

/**
 * @brief The RaftForeground class The service using Raft (e.g. a k/v server) wants to start
 * agreement on the next command to be appended to Raft's log. This is the object for that purpose.
 * Do not create more than one foreground on the same context.
 */
class RaftForeground {
  public:
    RaftForeground(RaftContext *context);
    ~RaftForeground();

    /**
     * @brief The BoardcastResult struct Returns by the BoardcastCommand() call.
     */
    struct BoardcastResult {
        explicit BoardcastResult(std::optional<RaftMachineAddress> const &retry_node);
        BoardcastResult(unsigned log_index, RaftTerm term);

        // The current node isn't the leader at the moment. But it knows the leader *might* be the
        // retry_node. The client should try sending the same command to the retry_node at the next
        // attempt.
        std::optional<RaftMachineAddress> retry_node;

        // The index that the command will appear as if it's ever committed. Do not use this value
        // when retry_node isn't std::nullopt.
        RaftLogOffset log_index;

        // The term when the command is appended to the log. Do not use this value when retry_node
        // isn't std::nullopt.
        RaftTerm term;
    };

    /**
     * @brief BoardcastCommand Attempts to boardcast the specified command so it gets replicated
     * among the peers. There is no guarantee that this command will ever be committed to the Raft
     * log, since the current node may not be the leader, or even if it's the leader, it may fail or
     * lose an election.
     *
     * @param command_entry The command to be logged and boardcast.
     * @return See above for the description of the return values.
     */
    BoardcastResult BoardcastCommand(CommandEntry const &command_entry);

    /**
     * @brief TermAndRole Returns the current term and role the node is holding.
     */
    std::pair<RaftTerm, RaftRole> TermAndRole() const;

  private:
    RaftContext *context_;
    std::mutex lock_;
    RandomSource random_source_;
};

} // namespace e8

#endif // RAFT_FOREGROUND_H
