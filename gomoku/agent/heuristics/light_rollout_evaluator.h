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

#ifndef LIGHT_ROLLOUT_EVALUATOR_H
#define LIGHT_ROLLOUT_EVALUATOR_H

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "common/thread/thread_pool.h"
#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/game/board_state.h"

namespace e8 {
namespace light_rollout_evaluator_internal {

struct BoardContour {
    std::unordered_set<MovePosition> positions;
};

std::shared_ptr<BoardContour> BuildBoardContour(GomokuBoardState const &board);

} // namespace light_rollout_evaluator_internal

/**
 * @brief The GomokuLightRolloutEvaluator class Evaluate the board state by conducting random
 * rollouts.
 */
class GomokuLightRolloutEvaluator : public GomokuEvaluatorInterface {
  public:
    GomokuLightRolloutEvaluator();
    ~GomokuLightRolloutEvaluator() override;

    float EvaluateReward(GomokuBoardState const &state, MctNodeId const state_id) override;

    std::unordered_map<GomokuActionId, float> EvaluatePolicy(GomokuBoardState const &state,
                                                             MctNodeId const state_id) override;

    float ExplorationFactor() const override;

    unsigned NumSimulations() const override;

    void ClearCache() override;

  private:
    struct GomokuLightRolloutEvaluatorInternal;
    std::unique_ptr<GomokuLightRolloutEvaluatorInternal> pimpl_;
};

} // namespace e8

#endif // LIGHT_ROLLOUT_EVALUATOR_H
