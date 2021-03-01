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

#ifndef TF_ZERO_PRIOR_EVALUATOR_H
#define TF_ZERO_PRIOR_EVALUATOR_H

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/game/board_state.h"

namespace e8 {

/**
 * @brief The GomokuTfZeroPriorEvaluator class This evaluator loads a tensorflow saved model and
 * runs inference over it to get the reward and policy estimation. The model requires zero
 * prior knowledge but only the current game state.
 */
class GomokuTfZeroPriorEvaluator : public GomokuEvaluatorInterface {
  public:
    /**
     * @brief GomokuTfZeroPriorEvaluator Constructs an evaluator from a tensorflow lite model file.
     */
    GomokuTfZeroPriorEvaluator(std::string const &model_path);
    ~GomokuTfZeroPriorEvaluator() override;

    float EvaluateReward(GomokuBoardState const &state, std::optional<MctNodeId> parent_state_id,
                         MctNodeId state_id) override;

    std::unordered_map<GomokuActionId, float>
    EvaluatePolicy(GomokuBoardState const &state, std::optional<MctNodeId> parent_state_id,
                   MctNodeId state_id) override;

    float ExplorationFactor() const override;

    unsigned NumSimulations() const override;

    void ClearCache() override;

  private:
    struct TfModelBasedEvaluatorInternal;
    std::unique_ptr<TfModelBasedEvaluatorInternal> pimpl_;
};

} // namespace e8

#endif // TF_ZERO_PRIOR_EVALUATOR_H
