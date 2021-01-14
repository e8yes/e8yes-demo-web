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

#ifndef MCT_NODE_H
#define MCT_NODE_H

#include <optional>

#include "common/container/mutable_priority_queue.h"
#include "gomoku/game/board_state.h"

namespace e8 {

/**
 * @brief The MctNode struct An abstract game state which stores statistics and action information
 * during monte carlo tree search.
 */
struct MctNode {
    std::optional<GomokuActionId> const arrived_thru_action_id;
    std::optional<PlayerSide> const action_performed_by;

    // Game result at the current game state.
    GameResult const game_result;

    // Policy weight given by the heuristics used to rank the action leading to the current
    // state.
    float const heuristic_policy_weight;

    // The below values can be used to calculate the Q value.
    float summed_reward = 0.0f;
    unsigned num_bandit_pulls = 0;

    // Upper confidence bound on the Q value.
    float upper_confidence_bound = 0.0f;

    // Child states obtained by applying exactly one action from the current state.
    MutablePriorityQueue<MctNode> children;

    /**
     * @brief MctNode Used by the container to fill up space. Otherwise, the node will be at an
     * invalid state.
     */
    MctNode();

    /**
     * @brief MctNode The only valid constructor.
     *
     * @param arrived_thru_action_id How did the current state arrive from the parent state. Set
     * this to nullopt if it's a the root state.
     * @param action_performed_by Which player performed the action. Set this ot nullopt if it's the
     * root state.
     * @param game_result Game state at the current state.
     * @param heuristic_policy_weight Policy weight given by the heuristics used to rank the action
     * leading to the current state.
     */
    MctNode(std::optional<GomokuActionId> const arrived_thru_action_id,
            std::optional<PlayerSide> const action_performed_by, GameResult const game_result,
            float const heuristic_policy_weight);

    MctNode(MctNode const &) = default;
    MctNode(MctNode &&) = default;
    ~MctNode() = default;

    /**
     * @brief operator < Orders the node by the upper confidence bound.
     */
    bool operator<(MctNode const &rhs) const;
};

} // namespace e8

#endif // MCT_NODE_H
