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
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "common/random/random_distribution.h"
#include "common/random/random_source.h"
#include "common/random/uniform_distribution.h"
#include "common/thread/thread_pool.h"
#include "gomoku/gomoku_agent/heuristics/evaluator.h"
#include "gomoku/gomoku_agent/heuristics/light_rollout_evaluator.h"
#include "gomoku/gomoku_game/board_state.h"

namespace e8 {
namespace {

unsigned const kNumQValueSamples = 200;
unsigned const kMaxSimulationSteps = 7;

struct BoardContour {
    std::unordered_set<MovePosition> positions;
};

void AddStone(GomokuBoardState const &board, MovePosition const &stone_pos, BoardContour *contour) {
    contour->positions.erase(stone_pos);
    if (stone_pos.x - 1 >= 0) {
        contour->positions.insert(MovePosition(stone_pos.x - 1, stone_pos.y));
    }
    if (stone_pos.x + 1 < board.Width()) {
        contour->positions.insert(MovePosition(stone_pos.x + 1, stone_pos.y));
    }

    if (stone_pos.y - 1 >= 0) {
        contour->positions.insert(MovePosition(stone_pos.x, stone_pos.y - 1));
        if (stone_pos.x - 1 >= 0) {
            contour->positions.insert(MovePosition(stone_pos.x - 1, stone_pos.y - 1));
        }
        if (stone_pos.x + 1 < board.Width()) {
            contour->positions.insert(MovePosition(stone_pos.x + 1, stone_pos.y - 1));
        }
    }

    if (stone_pos.y + 1 < board.Height()) {
        contour->positions.insert(MovePosition(stone_pos.x, stone_pos.y + 1));
        if (stone_pos.x - 1 >= 0) {
            contour->positions.insert(MovePosition(stone_pos.x - 1, stone_pos.y + 1));
        }
        if (stone_pos.x + 1 < board.Width()) {
            contour->positions.insert(MovePosition(stone_pos.x + 1, stone_pos.y + 1));
        }
    }
}

std::shared_ptr<BoardContour> BuildBoardContour(GomokuBoardState const &board) {
    auto contour = std::make_shared<BoardContour>();

    for (GomokuActionRecord const &record : board.History()) {
        switch (record.game_phase) {
        case GP_PLACE_3_STONES:
        case GP_SWAP2_PLACE_2_STONES:
        case GP_STANDARD_GOMOKU: {
            auto const &[action_id, action] = record.action;
            AddStone(board, *action.stone_pos, contour.get());
            break;
        }
        default: {
            // Do nothing.
            break;
        }
        }
    }

    return contour;
}

class RolloutData : public TaskStorageInterface {
  public:
    RolloutData(GomokuBoardState const &board, std::shared_ptr<BoardContour> const &contour,
                unsigned const num_samples, unsigned const job_idx);

    void Init();
    bool SampleNext(unsigned max_steps);
    float AccumulatedRewardFor(PlayerSide const stone_type) const;
    unsigned NumSamples() const;

  private:
    GomokuBoardState board_;
    std::shared_ptr<BoardContour> original_copy_;
    RandomSource random_source_;

    BoardContour contour_;
    std::array<unsigned, 2> num_wins_;
    unsigned num_steps_ = 0;
    unsigned const num_samples_;
};

RolloutData::RolloutData(GomokuBoardState const &board,
                         std::shared_ptr<BoardContour> const &contour, unsigned const num_samples,
                         unsigned const job_idx)
    : board_(board), original_copy_(contour), random_source_(job_idx), num_samples_(num_samples) {}

void RolloutData::Init() {
    contour_ = *original_copy_;

    while (num_steps_ > 0) {
        board_.RetractAction();
        --num_steps_;
    }
}

MovePosition SelectRandomMoveFromContour(BoardContour const &contour, RandomSource *random_source) {
    assert(!contour.positions.empty());

    std::vector<int> selector =
        DrawUniformInts(/*lower_bound=*/0, /*upper_bound=*/contour.positions.size(),
                        /*num_instances=*/1, random_source);
    assert(selector.size() == 1);

    int i = 0;
    MovePosition selected_move(/*x=*/-1, /*y=*/-1);
    for (auto const &pos : contour.positions) {
        if (i == selector[0]) {
            selected_move = pos;
            break;
        }
        ++i;
    }

    return selected_move;
}

bool RolloutData::SampleNext(unsigned max_steps) {
    ++num_steps_;

    MovePosition selected_move = SelectRandomMoveFromContour(contour_, &random_source_);
    AddStone(board_, selected_move, &contour_);

    GomokuActionId action_id = board_.MovePositionToActionId(selected_move);
    GameResult game_result = board_.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    switch (game_result) {
    case GR_TIE: {
        ++num_wins_[PlayerSide::PS_PLAYER_A];
        ++num_wins_[PlayerSide::PS_PLAYER_B];
        return false;
    }
    case GR_PLAYER_A_WIN:
    case GR_PLAYER_B_WIN: {
        ++num_wins_[(board_.CurrentPlayerSide() + 1) & 1];
        return false;
    }
    case GR_UNDETERMINED: {
        return num_steps_ < max_steps;
    }
    default: {
        assert(false);
        return false;
    }
    }
}

float RolloutData::AccumulatedRewardFor(PlayerSide const player_side) const {
    return num_wins_[player_side] - num_wins_[(player_side + 1) & 1];
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
        data->Init();
        while (data->SampleNext(kMaxSimulationSteps)) {
            // Do nothing.
        }
    }
}

bool RolloutTask::DropResourceOnCompletion() const { return false; }

} // namespace

struct GomokuLightRolloutEvaluator::GomokuLightRolloutEvaluatorInternal {
    ThreadPool thread_pool;
    RandomSource random_source;
    std::unordered_map<GomokuStateId, std::shared_ptr<BoardContour>> contour_cache;

    GomokuLightRolloutEvaluatorInternal();

    std::unordered_map<GomokuStateId, std::shared_ptr<BoardContour>>::const_iterator
    FetchContour(GomokuStateId const state_id, GomokuBoardState const &state);
};

GomokuLightRolloutEvaluator::GomokuLightRolloutEvaluatorInternal::
    GomokuLightRolloutEvaluatorInternal() {}

std::unordered_map<GomokuStateId, std::shared_ptr<BoardContour>>::const_iterator
GomokuLightRolloutEvaluator::GomokuLightRolloutEvaluatorInternal::FetchContour(
    GomokuStateId const state_id, GomokuBoardState const &state) {
    auto contour_cache_it = contour_cache.find(state_id);
    if (contour_cache_it == contour_cache.end()) {
        std::shared_ptr<BoardContour> contour = BuildBoardContour(state);
        contour_cache_it = contour_cache.insert(std::make_pair(state_id, contour)).first;
    }

    return contour_cache_it;
}

GomokuLightRolloutEvaluator::GomokuLightRolloutEvaluator() {}

GomokuLightRolloutEvaluator::~GomokuLightRolloutEvaluator() {}

float GomokuLightRolloutEvaluator::EvaluateReward(GomokuBoardState const &state,
                                                  GomokuStateId const state_id) const {
    float q_value;

    switch (state.CurrentGamePhase()) {
    case GP_PLACE_3_STONES:
    case GP_SWAP2_DECISION:
    case GP_SWAP2_PLACE_2_STONES:
    case GP_STONE_TYPE_DECISION: {
        q_value = 0.0f;
        break;
    }
    case GP_STANDARD_GOMOKU: {
        auto task = std::make_shared<RolloutTask>();

        auto contour_cache_it = pimpl_->FetchContour(state_id, state);

        for (unsigned job_idx = 0; job_idx < pimpl_->thread_pool.NumWorkers(); ++job_idx) {
            auto rollout_data = std::make_unique<RolloutData>(
                state, contour_cache_it->second,
                kNumQValueSamples / pimpl_->thread_pool.NumWorkers(), job_idx);
            pimpl_->thread_pool.Schedule(task, std::move(rollout_data));
        }

        float accumulated_reward = 0.0f;
        unsigned num_samples = 0;
        for (unsigned i = 0; i < pimpl_->thread_pool.NumWorkers(); ++i) {
            std::unique_ptr<TaskStorageInterface> rollout_data =
                pimpl_->thread_pool.WaitForNextCompleted();
            accumulated_reward += static_cast<RolloutData *>(rollout_data.get())
                                      ->AccumulatedRewardFor(state.CurrentPlayerSide());
            num_samples += static_cast<RolloutData *>(rollout_data.get())->NumSamples();
        }

        q_value = accumulated_reward / num_samples;

        break;
    }
    }

    return q_value;
}

std::unordered_map<GomokuActionId, float>
GomokuLightRolloutEvaluator::EvaluatePolicy(GomokuBoardState const &state,
                                            GomokuStateId const state_id) const {
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

        float uniform = 1.0f / contour_cache_it->second->positions.size();
        for (auto const &[action_id, action] : state.LegalActions()) {
            if (contour_cache_it->second->positions.find(*action.stone_pos) !=
                contour_cache_it->second->positions.end()) {
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

float GomokuLightRolloutEvaluator::ExplorationFactor() const { return std::sqrt(2); }

unsigned GomokuLightRolloutEvaluator::NumSimulations() const { return 1400; }

} // namespace e8
