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

#include <optional>

#include "common/container/mutable_priority_queue.h"
#include "gomoku/gomoku_agent/search/mct_node.h"
#include "gomoku/gomoku_game/board_state.h"

namespace e8 {

MctNode::MctNode() : game_result(GR_UNDETERMINED), heuristic_policy_weight(-1.0f) {}

MctNode::MctNode(std::optional<GomokuActionId> const arrived_thru_action_id,
                 std::optional<PlayerSide> const action_performed_by, GameResult const game_result,
                 float const heuristic_policy_weight)
    : arrived_thru_action_id(arrived_thru_action_id), action_performed_by(action_performed_by),
      game_result(game_result), heuristic_policy_weight(heuristic_policy_weight) {}

bool MctNode::operator<(MctNode const &rhs) const {
    return upper_confidence_bound < rhs.upper_confidence_bound;
}

} // namespace e8
