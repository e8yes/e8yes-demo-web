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
#include <cmath>
#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "common/random/random_distribution.h"
#include "common/random/random_source.h"
#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/heuristics/shl_feature.h"
#include "gomoku/agent/heuristics/shl_rollout_evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/agent/search/mct_search.h"
#include "gomoku/game/board_state.h"

namespace e8 {
namespace {

unsigned kNumRollouts = 10;

} // namespace

struct GomokuShlRolloutEvaluator::GomokuShlRolloutEvaluatorInternal {
    GomokuShlRolloutEvaluatorInternal();

    RandomSource random_source;
};

GomokuShlRolloutEvaluator::GomokuShlRolloutEvaluatorInternal::GomokuShlRolloutEvaluatorInternal()
    : random_source(/*seed=*/317) {}

GomokuShlRolloutEvaluator::GomokuShlRolloutEvaluator()
    : pimpl_(std::make_unique<GomokuShlRolloutEvaluatorInternal>()) {}

GomokuShlRolloutEvaluator::~GomokuShlRolloutEvaluator() {}

float GomokuShlRolloutEvaluator::EvaluateReward(GomokuBoardState const &state,
                                                MctNodeId const /*state_id*/) {
    PlayerSide evaluate_for_player = state.CurrentPlayerSide();
    float acc_reward = 0.0f;

    for (unsigned i = 0; i < kNumRollouts; ++i) {
        GomokuBoardState rollout_state = state;

        GameResult game_result = rollout_state.CurrentGameResult();
        do {
            std::unordered_map<GomokuActionId, float> policy =
                this->EvaluatePolicy(rollout_state, /*state_id=*/-1);
            GomokuActionId action_id = SampleAction(policy, &pimpl_->random_source);
            game_result = rollout_state.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
        } while (game_result == GameResult::GR_UNDETERMINED);

        switch (game_result) {
        case GR_TIE: {
            acc_reward += 0.0f;
            break;
        }
        case GR_PLAYER_A_WIN: {
            if (evaluate_for_player == PlayerSide::PS_PLAYER_A) {
                acc_reward += 1.0f;
            } else {
                acc_reward -= 1.0f;
            }
            break;
        }
        case GR_PLAYER_B_WIN: {
            if (evaluate_for_player == PlayerSide::PS_PLAYER_B) {
                acc_reward += 1.0f;
            } else {
                acc_reward -= 1.0f;
            }
            break;
        }
        default: {
            assert(false);
        }
        }
    }

    return acc_reward / kNumRollouts;
}

std::unordered_map<GomokuActionId, float>
GomokuShlRolloutEvaluator::EvaluatePolicy(GomokuBoardState const &state,
                                          MctNodeId const /*state_id*/) {
    std::unordered_map<GomokuActionId, float> policy;

    switch (state.CurrentGamePhase()) {
    case GP_PLACE_3_STONES:
    case GP_SWAP2_DECISION:
    case GP_SWAP2_PLACE_2_STONES:
    case GP_STONE_TYPE_DECISION: {
        std::vector<float> random_pmf =
            RandomPmf(state.LegalActions().size(), &pimpl_->random_source);
        unsigned selector = 0;
        for (auto const &[action_id, _] : state.LegalActions()) {
            policy[action_id] = random_pmf[selector++];
        }
        break;
    }
    case GP_STANDARD_GOMOKU: {
        StoneType next_move_stone_type = state.PlayerStoneType(state.CurrentPlayerSide());
        ShlFeatures shl_map = ComputeShlFeatures(state, next_move_stone_type, /*top_k=*/10);

        float cdf = 0.0f;
        for (auto const &[pos, shl_components] : shl_map.normalized_top_k_map) {
            GomokuActionId action_id = state.MovePositionToActionId(pos);
            float shl_score = ToShlScore(shl_components);
            cdf += shl_score;
            policy[action_id] = shl_score;
        }

        assert(std::abs(cdf - 1.0f) < 1e-1);
        break;
    }
    }

    return policy;
}

float GomokuShlRolloutEvaluator::ExplorationFactor() const { return 3.0f; }

unsigned GomokuShlRolloutEvaluator::NumSimulations() const { return 1000; }

void GomokuShlRolloutEvaluator::ClearCache() {}

} // namespace e8
