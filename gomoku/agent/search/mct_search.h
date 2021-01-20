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

#ifndef MCT_SEARCH_H
#define MCT_SEARCH_H

#include <memory>
#include <optional>
#include <unordered_map>

#include "common/container/mutable_priority_queue.h"
#include "common/random/random_source.h"
#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/game/board_state.h"

namespace e8 {

/**
 * @brief The MctSearcher class Holds a Monte Carlo search tree for a sequence of tree search
 * operations.
 */
class MctSearcher {
  public:
    /**
     * @brief MctSearcher Constructs a Monte Carlo Tree searcher with the specified evaluation
     * heuristics. The search strategy uses a PUCT score rather than the convention UCT.
     *
     * @param evaluator Heuristics to help guide the tree search.
     * @param print_stats Whether to print the internal stats after each SearchFrom() call.
     */
    MctSearcher(std::shared_ptr<GomokuEvaluatorInterface> const &evaluator, bool const print_stats);
    MctSearcher(MctSearcher const &) = delete;
    MctSearcher(MctSearcher &&) = delete;
    ~MctSearcher() = default;

    /**
     * @brief Reset Clear the existing search tree if there is one. All the currently holding
     * iterators will be invalidated. Then point the internal tree node iterator to the new root.
     */
    void Reset();

    /**
     * @brief SearchFrom Calculate a stochastically optimal policy for the specified board state
     * with the help of the heuristics. The internal tree node tterator must correspond to the
     * internal board state. Synchronize the internal tree node by calling SelectAction() to
     * transition the tree node state through actions.
     *
     * @param state Board state to search from.
     * @param temperature Controls how "flat" the output policy distribution should be. The
     * resulting policy PMF is normalized as policy_pmf[action_id] =
     * visit_count[action_id]^(1/temperature) / sum(visit_count[action_id]^(1/temperature))
     * @return A stochastic policy.
     */
    std::unordered_map<GomokuActionId, float> SearchFrom(GomokuBoardState state,
                                                         float const temperature);

    /**
     * @brief SelectAction Explicitly transition to a state. If the internal from_state_node has
     * not yet expanded by the MctSearcher's SearchFrom() call, this function will force an
     * expansion with policy evaluation even though the selection has nothing to do with the
     * heuristic policy. The transition will free the memory allocated for the rest of the children.
     *
     * @param from_state_node The parent node to transition from.
     */
    void SelectAction(GomokuBoardState state, GomokuActionId const action_id);

  private:
    MutablePriorityQueue<MctNode> root_;
    std::optional<MutablePriorityQueue<MctNode>::iterator> current_node_it_;
    std::shared_ptr<GomokuEvaluatorInterface> evaluator_;
    bool const print_stats_;
};

/**
 * @brief SelectBestAction Find the action that has the highest mass in the policy distribution.
 */
GomokuActionId BestAction(std::unordered_map<GomokuActionId, float> const &policy);

/**
 * @brief SampleAction Samples an action from the policy PMF.
 */
GomokuActionId SampleAction(std::unordered_map<GomokuActionId, float> const &policy,
                            RandomSource *random_source);

} // namespace e8

#endif // MCT_SEARCH_H
