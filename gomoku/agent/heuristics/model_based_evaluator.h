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

#ifndef MODELBASEDEVALUATOR_H
#define MODELBASEDEVALUATOR_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/game/board_state.h"

namespace e8 {

/**
 * @brief The GomokuModelBasedEvaluator class This evaluator loads a tensorflow lite model and runs
 * inference over it to get the reward and policy estimation.
 */
class GomokuModelBasedEvaluator : public GomokuEvaluatorInterface {
  public:
    /**
     * @brief GomokuModelBasedEvaluator Constructs an evaluator from a tensorflow lite model file.
     */
    GomokuModelBasedEvaluator(std::string const &model_path);
    ~GomokuModelBasedEvaluator() override;

    float EvaluateReward(GomokuBoardState const &state, MctNodeId const state_id) override;

    std::unordered_map<GomokuActionId, float> EvaluatePolicy(GomokuBoardState const &state,
                                                             MctNodeId const state_id) override;

    float ExplorationFactor() const override;

    unsigned NumSimulations() const override;

    void ClearCache() override;

  private:
    struct ModelBasedEvaluatorInternal;
    std::unique_ptr<ModelBasedEvaluatorInternal> pimpl_;
};

} // namespace e8

#endif // MODELBASEDEVALUATOR_H
