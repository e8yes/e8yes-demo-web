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

#ifndef SHL_MODEL_ENSEMBLE_EVALUATOR_H
#define SHL_MODEL_ENSEMBLE_EVALUATOR_H

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
 * @brief The GomokuShlModelEnsembleEvaluator class Evaluates the board state by an ensemble of
 * methods. In particular, the value of the state is evaluated by:
 *  1. Conducting rollouts on the SHL policy.
 *  2. A model based denoiser module adds inductive bias to the rollout samples.
 * The heuristic policy is constructed by:
 *  A neural network based model using the board state and the SHL map.
 */
class GomokuShlModelEvaluator : public GomokuEvaluatorInterface {
  public:
    GomokuShlModelEvaluator(std::string const &model_path);
    ~GomokuShlModelEvaluator();

    float EvaluateReward(GomokuBoardState const &state, std::optional<MctNodeId> parent_state_id,
                         MctNodeId state_id) override;

    std::unordered_map<GomokuActionId, float>
    EvaluatePolicy(GomokuBoardState const &state, std::optional<MctNodeId> parent_state_id,
                   MctNodeId state_id) override;

    float ExplorationFactor() const override;

    unsigned NumSimulations() const override;

    void ClearCache() override;

  private:
    struct GomokuShlModelEvaluatorInternal;
    std::unique_ptr<GomokuShlModelEvaluatorInternal> pimpl_;
};

} // namespace e8

#endif // SHL_MODEL_ENSEMBLE_EVALUATOR_H
