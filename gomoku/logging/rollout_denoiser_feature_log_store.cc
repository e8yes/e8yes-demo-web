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

#include <cassert>
#include <cstdlib>
#include <vector>

#include "gomoku/logging/common_types.h"
#include "gomoku/logging/rollout_denoiser_feature_log_store.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {
namespace {

char const *kGomokuRolloutDenoiserFeaturesTableName = "gomoku_rollout_denoiser_features";

struct GomokuRolloutDenoiserFeaturesEntity : public SqlEntityInterface {
    GomokuRolloutDenoiserFeaturesEntity()
        : SqlEntityInterface({&game_id, &step_number, &rollouts_number, &num_rollouts,
                              &value_seq_distributions, &suboptimality_histogram,
                              &value_seq_num_moves, &value_seq_num_flips, &value_outcomes,
                              &value_outcome_mean, &value_outcome_var, &ground_truth_value,
                              &created_at}) {}

    GomokuRolloutDenoiserFeaturesEntity(GomokuRolloutDenoiserFeaturesEntity const &other)
        : GomokuRolloutDenoiserFeaturesEntity() {
        game_id = other.game_id;
        step_number = other.step_number;
        rollouts_number = other.rollouts_number;
        num_rollouts = other.num_rollouts;
        value_seq_distributions = other.value_seq_distributions;
        suboptimality_histogram = other.suboptimality_histogram;
        value_seq_num_moves = other.value_seq_num_moves;
        value_seq_num_flips = other.value_seq_num_flips;
        value_outcomes = other.value_outcomes;
        value_outcome_mean = other.value_outcome_mean;
        value_outcome_var = other.value_outcome_var;
        ground_truth_value = other.ground_truth_value;
        created_at = other.created_at;
    }

    GomokuRolloutDenoiserFeaturesEntity &
    operator=(GomokuRolloutDenoiserFeaturesEntity const &other) {
        game_id = other.game_id;
        step_number = other.step_number;
        rollouts_number = other.rollouts_number;
        num_rollouts = other.num_rollouts;
        value_seq_distributions = other.value_seq_distributions;
        suboptimality_histogram = other.suboptimality_histogram;
        value_seq_num_moves = other.value_seq_num_moves;
        value_seq_num_flips = other.value_seq_num_flips;
        value_outcomes = other.value_outcomes;
        value_outcome_mean = other.value_outcome_mean;
        value_outcome_var = other.value_outcome_var;
        ground_truth_value = other.ground_truth_value;
        created_at = other.created_at;

        return *this;
    }

    SqlLong game_id = SqlLong("game_id");
    SqlInt step_number = SqlInt("step_number");
    SqlInt rollouts_number = SqlInt("rollouts_number");
    SqlInt num_rollouts = SqlInt("num_rollouts");
    SqlFloatArr value_seq_distributions = SqlFloatArr("value_seq_distributions");
    SqlIntArr suboptimality_histogram = SqlIntArr("suboptimality_histogram");
    SqlIntArr value_seq_num_moves = SqlIntArr("value_seq_num_moves");
    SqlIntArr value_seq_num_flips = SqlIntArr("value_seq_num_flips");
    SqlFloatArr value_outcomes = SqlFloatArr("value_outcomes");
    SqlFloat value_outcome_mean = SqlFloat("value_outcome_mean");
    SqlFloat value_outcome_var = SqlFloat("value_outcome_var");
    SqlFloat ground_truth_value = SqlFloat("ground_truth_value");
    SqlTimestamp created_at = SqlTimestamp("created_at");
};

} // namespace

RolloutDenoiserFeatureLogStore::RolloutDenoiserFeatureLogStore(ConnectionReservoirInterface *conns)
    : conns_(conns) {}

void RolloutDenoiserFeatureLogStore::LogFeatures(
    GameId game_id, GameStepNumber step_number, RolloutDenoiserTrialNumber trial_number,
    unsigned num_rollouts, std::vector<RolloutSequenceStats> const &per_seq_stats,
    float value_outcome_mean, float value_outcome_var, float ground_truth_value) {
    GomokuRolloutDenoiserFeaturesEntity entity;
    *entity.game_id.ValuePtr() = game_id;
    *entity.step_number.ValuePtr() = step_number;
    *entity.rollouts_number.ValuePtr() = trial_number;
    *entity.num_rollouts.ValuePtr() = num_rollouts;

    for (auto const &stats : per_seq_stats) {
        for (auto bin : stats.value_distribution) {
            entity.value_seq_distributions.ValuePtr()->push_back(bin);
        }
        for (auto bin : stats.suboptimality_histogram) {
            entity.suboptimality_histogram.ValuePtr()->push_back(bin);
        }
        entity.value_seq_num_moves.ValuePtr()->push_back(stats.num_moves);
        entity.value_seq_num_flips.ValuePtr()->push_back(stats.num_flips);
        entity.value_outcomes.ValuePtr()->push_back(stats.outcome);
    }

    *entity.value_outcome_mean.ValuePtr() = value_outcome_mean;
    *entity.value_outcome_var.ValuePtr() = value_outcome_var;
    *entity.ground_truth_value.ValuePtr() = ground_truth_value;

    int64_t num_rows =
        Update(entity, kGomokuRolloutDenoiserFeaturesTableName, /*replace=*/true, conns_);
    assert(num_rows == 1);
}

} // namespace e8
