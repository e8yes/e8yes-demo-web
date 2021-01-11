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

void AddStone(GomokuBoardState const &board, MovePosition const &stone_pos,
              light_rollout_evaluator_internal::BoardContour *contour) {
    contour->positions.erase(stone_pos);
    if (stone_pos.x - 1 >= 0 && *board.ChessPieceStateAt(MovePosition(
                                    stone_pos.x - 1, stone_pos.y)) == StoneType::ST_NONE) {
        contour->positions.insert(MovePosition(stone_pos.x - 1, stone_pos.y));
    }
    if (stone_pos.x + 1 < board.Width() &&
        *board.ChessPieceStateAt(MovePosition(stone_pos.x + 1, stone_pos.y)) ==
            StoneType::ST_NONE) {
        contour->positions.insert(MovePosition(stone_pos.x + 1, stone_pos.y));
    }

    if (stone_pos.y - 1 >= 0) {
        if (*board.ChessPieceStateAt(MovePosition(stone_pos.x, stone_pos.y - 1)) ==
            StoneType::ST_NONE) {
            contour->positions.insert(MovePosition(stone_pos.x, stone_pos.y - 1));
        }
        if (stone_pos.x - 1 >= 0 && *board.ChessPieceStateAt(MovePosition(
                                        stone_pos.x - 1, stone_pos.y - 1)) == StoneType::ST_NONE) {
            contour->positions.insert(MovePosition(stone_pos.x - 1, stone_pos.y - 1));
        }
        if (stone_pos.x + 1 < board.Width() &&
            *board.ChessPieceStateAt(MovePosition(stone_pos.x + 1, stone_pos.y - 1)) ==
                StoneType::ST_NONE) {
            contour->positions.insert(MovePosition(stone_pos.x + 1, stone_pos.y - 1));
        }
    }

    if (stone_pos.y + 1 < board.Height()) {
        if (*board.ChessPieceStateAt(MovePosition(stone_pos.x, stone_pos.y + 1)) ==
            StoneType::ST_NONE) {
            contour->positions.insert(MovePosition(stone_pos.x, stone_pos.y + 1));
        }
        if (stone_pos.x - 1 >= 0 && *board.ChessPieceStateAt(MovePosition(
                                        stone_pos.x - 1, stone_pos.y + 1)) == StoneType::ST_NONE) {
            contour->positions.insert(MovePosition(stone_pos.x - 1, stone_pos.y + 1));
        }
        if (stone_pos.x + 1 < board.Width() &&
            *board.ChessPieceStateAt(MovePosition(stone_pos.x + 1, stone_pos.y + 1)) ==
                StoneType::ST_NONE) {
            contour->positions.insert(MovePosition(stone_pos.x + 1, stone_pos.y + 1));
        }
    }
}

} // namespace

namespace light_rollout_evaluator_internal {

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

} // namespace light_rollout_evaluator_internal

namespace {

unsigned const kNumQValueSamples = 4096;
unsigned const kEarlyGameMaxSimulationSteps = 7;
unsigned const kMidGameMaxSimulationSteps = 5;

class RolloutData : public TaskStorageInterface {
  public:
    RolloutData(GomokuBoardState const &board,
                std::shared_ptr<light_rollout_evaluator_internal::BoardContour> const &contour,
                unsigned const num_samples, unsigned const job_idx);

    void Init();
    bool SampleNext();
    unsigned AccumulatedWinsFor(PlayerSide const stone_type) const;
    unsigned NumSamples() const;

  private:
    GomokuBoardState board_;
    std::shared_ptr<light_rollout_evaluator_internal::BoardContour> original_copy_;
    RandomSource random_source_;

    light_rollout_evaluator_internal::BoardContour contour_;
    std::array<unsigned, 2> num_wins_;
    unsigned num_steps_ = 0;
    unsigned const num_samples_;
    unsigned max_sim_steps_;
};

RolloutData::RolloutData(
    GomokuBoardState const &board,
    std::shared_ptr<light_rollout_evaluator_internal::BoardContour> const &contour,
    unsigned const num_samples, unsigned const job_idx)
    : board_(board), original_copy_(contour), random_source_(31 * (job_idx + 47128)),
      num_wins_({0, 0}), num_samples_(num_samples) {
    if (board_.History().size() <= 5) {
        max_sim_steps_ = kEarlyGameMaxSimulationSteps;
    } else {
        max_sim_steps_ = kMidGameMaxSimulationSteps;
    }
}

void RolloutData::Init() {
    contour_ = *original_copy_;

    while (num_steps_ > 0) {
        board_.RetractAction();
        --num_steps_;
    }
}

MovePosition
SelectRandomMoveFromContour(light_rollout_evaluator_internal::BoardContour const &contour,
                            RandomSource *random_source) {
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

    assert(selected_move.x != -1 && selected_move.y != -1);

    return selected_move;
}

bool RolloutData::SampleNext() {
    ++num_steps_;

    MovePosition selected_move = SelectRandomMoveFromContour(contour_, &random_source_);
    assert(selected_move.x >= 0 && selected_move.x < board_.Width());
    assert(selected_move.y >= 0 && selected_move.y < board_.Height());
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
        return num_steps_ < max_sim_steps_;
    }
    default: {
        assert(false);
        return false;
    }
    }
}

unsigned RolloutData::AccumulatedWinsFor(PlayerSide const player_side) const {
    return num_wins_[player_side];
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
        while (data->SampleNext()) {
            // Do nothing.
        }
    }
}

bool RolloutTask::DropResourceOnCompletion() const { return false; }

} // namespace

struct GomokuLightRolloutEvaluator::GomokuLightRolloutEvaluatorInternal {
    ThreadPool thread_pool;
    RandomSource random_source;
    std::unordered_map<GomokuStateId,
                       std::shared_ptr<light_rollout_evaluator_internal::BoardContour>>
        contour_cache;

    GomokuLightRolloutEvaluatorInternal();

    std::unordered_map<
        GomokuStateId,
        std::shared_ptr<light_rollout_evaluator_internal::BoardContour>>::const_iterator
    FetchContour(GomokuStateId const state_id, GomokuBoardState const &state);
};

GomokuLightRolloutEvaluator::GomokuLightRolloutEvaluatorInternal::
    GomokuLightRolloutEvaluatorInternal()
    : random_source(13) {}

std::unordered_map<GomokuStateId,
                   std::shared_ptr<light_rollout_evaluator_internal::BoardContour>>::const_iterator
GomokuLightRolloutEvaluator::GomokuLightRolloutEvaluatorInternal::FetchContour(
    GomokuStateId const state_id, GomokuBoardState const &state) {
    auto contour_cache_it = contour_cache.find(state_id);
    if (contour_cache_it == contour_cache.end()) {
        std::shared_ptr<light_rollout_evaluator_internal::BoardContour> contour =
            light_rollout_evaluator_internal::BuildBoardContour(state);
        contour_cache_it = contour_cache.insert(std::make_pair(state_id, contour)).first;
    }

    return contour_cache_it;
}

GomokuLightRolloutEvaluator::GomokuLightRolloutEvaluator()
    : pimpl_(std::make_unique<GomokuLightRolloutEvaluatorInternal>()) {}

GomokuLightRolloutEvaluator::~GomokuLightRolloutEvaluator() {}

float GomokuLightRolloutEvaluator::EvaluateReward(GomokuBoardState const &state,
                                                  GomokuStateId const state_id) {
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

        unsigned const num_parallelism = pimpl_->thread_pool.NumWorkers();
        // unsigned const num_parallelism = 1;
        for (unsigned job_idx = 0; job_idx < num_parallelism; ++job_idx) {
            auto rollout_data = std::make_unique<RolloutData>(
                state, contour_cache_it->second, kNumQValueSamples / num_parallelism, job_idx);
            pimpl_->thread_pool.Schedule(task, std::move(rollout_data));
        }

        float accumulated_reward = 0.0f;
        unsigned num_valid_samples = 0;
        for (unsigned i = 0; i < num_parallelism; ++i) {
            std::unique_ptr<TaskStorageInterface> rollout_data =
                pimpl_->thread_pool.WaitForNextCompleted();
            int wins = static_cast<RolloutData *>(rollout_data.get())
                           ->AccumulatedWinsFor(state.CurrentPlayerSide());
            int losses = static_cast<RolloutData *>(rollout_data.get())
                             ->AccumulatedWinsFor(static_cast<PlayerSide>(
                                 (static_cast<int>(state.CurrentPlayerSide()) + 1) & 1));
            accumulated_reward += wins - losses;
            num_valid_samples += wins + losses;
        }

        if (num_valid_samples > 0) {
            q_value = accumulated_reward / num_valid_samples;

            assert(q_value < 1.05f && q_value > -1.05f);
            assert(!std::isinf(q_value));
            assert(!std::isnan(q_value));
        } else {
            q_value = 0;
        }

        break;
    }
    }

    return q_value;
}

std::unordered_map<GomokuActionId, float>
GomokuLightRolloutEvaluator::EvaluatePolicy(GomokuBoardState const &state,
                                            GomokuStateId const state_id) {
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

void GomokuLightRolloutEvaluator::ClearCache() { pimpl_->contour_cache.clear(); }

} // namespace e8
