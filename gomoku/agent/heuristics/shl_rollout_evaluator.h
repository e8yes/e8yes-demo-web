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

#ifndef SHL_ROLLOUT_EVALUATOR_H
#define SHL_ROLLOUT_EVALUATOR_H

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "common/thread/thread_pool.h"
#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/game/board_state.h"

namespace e8 {

/**
 * @brief The GomokuShlRolloutEvaluator class Evaluates the board state by conducting rollouts on a
 * policy. The policy is constructed simple by taking the top 10 normalized SHL scores during the
 * standard Gomoku game phase.
 */
class GomokuShlRolloutEvaluator : public GomokuEvaluatorInterface {
  public:
    GomokuShlRolloutEvaluator();
    ~GomokuShlRolloutEvaluator();

    float EvaluateReward(GomokuBoardState const &state, MctNodeId const state_id) override;

    std::unordered_map<GomokuActionId, float> EvaluatePolicy(GomokuBoardState const &state,
                                                             MctNodeId const state_id) override;

    float ExplorationFactor() const override;

    unsigned NumSimulations() const override;

    void ClearCache() override;

  private:
    struct GomokuShlRolloutEvaluatorInternal;
    std::unique_ptr<GomokuShlRolloutEvaluatorInternal> pimpl_;
};

} // namespace e8

#endif // SHL_ROLLOUT_EVALUATOR_H
