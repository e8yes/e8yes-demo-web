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

#ifndef ROLLOUT_DENOISER_FEATURE_LOG_STORE_H
#define ROLLOUT_DENOISER_FEATURE_LOG_STORE_H

#include <vector>

#include "gomoku/logging/common_types.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

/**
 * @brief The RolloutDenoiserFeatureLogStore class
 */
class RolloutDenoiserFeatureLogStore {
  public:
    /**
     * @brief RolloutDenoiserFeatureLogStore All the logs will be written to the specified
     * connection reservoir target.
     */
    explicit RolloutDenoiserFeatureLogStore(ConnectionReservoirInterface *conns);

    /**
     * @brief The RolloutSequenceStats struct Aggregate statistics extracted from a rollout
     * sequence.
     */
    struct RolloutSequenceStats {
        std::vector<float> value_distribution;
        std::vector<unsigned> suboptimality_histogram;
        unsigned num_moves;
        unsigned num_flips;
        float outcome;
    };

    /**
     * @brief LogFeatures Stores the rollout denoiser features for a specified action step in a
     * game. The feature then is defined for the game state at that action step.
     *
     * @param game_id ID of the game that this feature is for.
     * @param step_number The action step that this feature is for.
     * @param trial_number This specifies for which trial is the rollout samples in the feature.
     * @param num_rollouts The number of rollouts was conducted to construct the features.
     * @param per_seq_stats Per rollout sequence statistics. See above.
     * @param value_outcome_mean Mean of the outcomes produced by the rollout samples.
     * @param value_outcome_var Variance of the outcomes produced by the rollout samples.
     * @param ground_truth_value Target ground truth value that needs to be learned.
     */
    void LogFeatures(GameId game_id, GameStepNumber step_number,
                     RolloutDenoiserTrialNumber trial_number, unsigned num_rollouts,
                     std::vector<RolloutSequenceStats> const &per_seq_stats,
                     float value_outcome_mean, float value_outcome_var, float ground_truth_value);

  private:
    ConnectionReservoirInterface *const conns_;
};

} // namespace e8

#endif // ROLLOUT_DENOISER_FEATURE_LOG_STORE_H
