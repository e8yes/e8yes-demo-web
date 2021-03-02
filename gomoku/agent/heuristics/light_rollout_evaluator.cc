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

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "common/random/random_distribution.h"
#include "common/random/random_source.h"
#include "common/random/uniform_distribution.h"
#include "common/thread/thread_pool.h"
#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/heuristics/light_rollout_evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/game/board_state.h"

namespace e8 {
namespace {

unsigned const kNumValueSamples = 512;
unsigned const kMaxSimulationSteps = 11;
float const kRewardDiscount = 0.8f;

class RolloutData : public TaskStorageInterface {
  public:
    RolloutData(GomokuBoardState const &board, ContourBuilder const &contour,
                unsigned const num_samples, unsigned const job_idx);

    bool SampleNext();
    float AccumulatedRewardFor(PlayerSide const stone_type) const;
    unsigned NumSamples() const;
    void Reset();

  private:
    GomokuBoardState board_;
    ContourBuilder original_copy_;
    RandomSource random_source_;

    ContourBuilder contour_builder_;
    std::array<float, 2> reward_;
    unsigned num_steps_ = 0;
    unsigned const num_samples_;
    unsigned max_sim_steps_;
};

RolloutData::RolloutData(GomokuBoardState const &board, ContourBuilder const &contour_builder,
                         unsigned const num_samples, unsigned const job_idx)
    : board_(board), original_copy_(contour_builder), random_source_(31 * (job_idx + 47128)),
      contour_builder_(contour_builder), reward_({0, 0}), num_samples_(num_samples) {
    max_sim_steps_ = kMaxSimulationSteps;
}

void RolloutData::Reset() {
    contour_builder_ = original_copy_;

    while (num_steps_ > 0) {
        board_.RetractAction();
        --num_steps_;
    }
}

MovePosition SelectRandomMoveFromContour(std::unordered_set<MovePosition> const &contour,
                                         RandomSource *random_source) {
    assert(!contour.empty());

    std::vector<int> selector = DrawUniformInts(/*lower_bound=*/0, /*upper_bound=*/contour.size(),
                                                /*num_instances=*/1, random_source);
    assert(selector.size() == 1);

    int i = 0;
    MovePosition selected_move(/*x=*/-1, /*y=*/-1);
    for (auto const &pos : contour) {
        if (i == selector[0]) {
            selected_move = pos;
            break;
        }
        ++i;
    }

    assert(selected_move.x != -1 && selected_move.y != -1);

    return selected_move;
}

bool RolloutData::SampleNext() {
    ++num_steps_;

    if (contour_builder_.Contour().empty()) {
        // Can't make any more moves.
        return false;
    }

    MovePosition selected_move =
        SelectRandomMoveFromContour(contour_builder_.Contour(), &random_source_);
    assert(selected_move.x >= 0 && selected_move.x < board_.Width());
    assert(selected_move.y >= 0 && selected_move.y < board_.Height());
    contour_builder_.AddStone(selected_move);

    GomokuActionId action_id = board_.MovePositionToActionId(selected_move);
    GameResult game_result = board_.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    switch (game_result) {
    case GR_TIE: {
        float discounted_reward = std::pow(kRewardDiscount, num_steps_ - 1);
        reward_[PlayerSide::PS_PLAYER_A] = discounted_reward;
        reward_[PlayerSide::PS_PLAYER_B] = discounted_reward;
        return false;
    }
    case GR_PLAYER_A_WIN:
    case GR_PLAYER_B_WIN: {
        float discounted_reward = std::pow(kRewardDiscount, num_steps_ - 1);
        reward_[(board_.CurrentPlayerSide() + 1) & 1] = discounted_reward;
        return false;
    }
    case GR_UNDETERMINED: {
        return num_steps_ < max_sim_steps_;
    }
    default: {
        assert(false);
        return false;
    }
    }
}

float RolloutData::AccumulatedRewardFor(PlayerSide const player_side) const {
    return reward_[player_side];
}

unsigned RolloutData::NumSamples() const { return num_samples_; }

class RolloutTask : public TaskInterface {
  public:
    void Run(TaskStorageInterface *storage) const;
    bool DropResourceOnCompletion() const;
};

void RolloutTask::Run(TaskStorageInterface *storage) const {
    RolloutData *data = static_cast<RolloutData *>(storage);

    for (unsigned i = 0; i < data->NumSamples(); ++i) {
        while (data->SampleNext()) {
            // Do nothing.
        }
        data->Reset();
    }
}

bool RolloutTask::DropResourceOnCompletion() const { return false; }

} // namespace

struct GomokuLightRolloutEvaluator::GomokuLightRolloutEvaluatorInternal {
    ThreadPool thread_pool;
    RandomSource random_source;
    std::unordered_map<MctNodeId, std::shared_ptr<ContourBuilder>> contour_cache;

    GomokuLightRolloutEvaluatorInternal();

    std::unordered_map<MctNodeId, std::shared_ptr<ContourBuilder>>::const_iterator
    FetchContour(MctNodeId const state_id, GomokuBoardState const &state);
};

GomokuLightRolloutEvaluator::GomokuLightRolloutEvaluatorInternal::
    GomokuLightRolloutEvaluatorInternal()
    : random_source(13) {}

std::unordered_map<MctNodeId, std::shared_ptr<ContourBuilder>>::const_iterator
GomokuLightRolloutEvaluator::GomokuLightRolloutEvaluatorInternal::FetchContour(
    MctNodeId const state_id, GomokuBoardState const &state) {
    auto contour_cache_it = contour_cache.find(state_id);
    if (contour_cache_it == contour_cache.end()) {
        contour_cache_it = contour_cache
                               .insert(std::make_pair(
                                   state_id, std::make_shared<ContourBuilder>(state, /*order=*/1)))
                               .first;
    }

    return contour_cache_it;
}

GomokuLightRolloutEvaluator::GomokuLightRolloutEvaluator()
    : pimpl_(std::make_unique<GomokuLightRolloutEvaluatorInternal>()) {}

GomokuLightRolloutEvaluator::~GomokuLightRolloutEvaluator() {}

float GomokuLightRolloutEvaluator::EvaluateReward(GomokuBoardState const &state,
                                                  std::optional<MctNodeId> /*parent_state_id*/,
                                                  MctNodeId state_id) {
    float value;

    switch (state.CurrentGamePhase()) {
    case GP_PLACE_3_STONES:
    case GP_SWAP2_DECISION:
    case GP_SWAP2_PLACE_2_STONES:
    case GP_STONE_TYPE_DECISION: {
        value = 0.0f;
        break;
    }
    case GP_STANDARD_GOMOKU: {
        auto task = std::make_shared<RolloutTask>();

        auto contour_cache_it = pimpl_->FetchContour(state_id, state);

        unsigned const num_parallelism = pimpl_->thread_pool.NumWorkers();
        // unsigned const num_parallelism = 1;
        for (unsigned job_idx = 0; job_idx < num_parallelism; ++job_idx) {
            auto rollout_data = std::make_unique<RolloutData>(
                state, *contour_cache_it->second, kNumValueSamples / num_parallelism, job_idx);
            pimpl_->thread_pool.Schedule(task, std::move(rollout_data));
        }

        float reward_diff = 0.0f;
        float total_reward = 0.0f;
        for (unsigned i = 0; i < num_parallelism; ++i) {
            std::unique_ptr<TaskStorageInterface> rollout_data =
                pimpl_->thread_pool.WaitForNextCompleted();
            float wins = static_cast<RolloutData *>(rollout_data.get())
                             ->AccumulatedRewardFor(state.CurrentPlayerSide());
            float losses = static_cast<RolloutData *>(rollout_data.get())
                               ->AccumulatedRewardFor(static_cast<PlayerSide>(
                                   (static_cast<int>(state.CurrentPlayerSide()) + 1) & 1));
            reward_diff += wins - losses;
            total_reward += wins + losses;
        }

        if (total_reward > 0) {
            value = reward_diff / total_reward;

            assert(value < 1.05f && value > -1.05f);
            assert(!std::isinf(value));
            assert(!std::isnan(value));
        } else {
            value = 0;
        }

        break;
    }
    }

    return value;
}

std::unordered_map<GomokuActionId, float>
GomokuLightRolloutEvaluator::EvaluatePolicy(GomokuBoardState const &state,
                                            std::optional<MctNodeId> /*parent_state_id*/,
                                            MctNodeId state_id) {
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
        auto contour_cache_it = pimpl_->FetchContour(state_id, state);

        float uniform = 1.0f / contour_cache_it->second->Contour().size();
        for (auto const &[action_id, action] : state.LegalActions()) {
            if (contour_cache_it->second->Contour().find(*action.stone_pos) !=
                contour_cache_it->second->Contour().end()) {
                policy[action_id] = uniform;
            } else {
                policy[action_id] = 0.0f;
            }
        }
        break;
    }
    }

    return policy;
}

float GomokuLightRolloutEvaluator::ExplorationFactor() const { return 2; }

unsigned GomokuLightRolloutEvaluator::NumSimulations() const { return 6000; }

void GomokuLightRolloutEvaluator::ClearCache() { pimpl_->contour_cache.clear(); }

} // namespace e8
