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
#include <memory>
#include <unordered_map>

#include "gomoku/gomoku_agent/heuristics/evaluator.h"
#include "gomoku/gomoku_agent/mcts_agent_player.h"
#include "gomoku/gomoku_agent/search/mct_search.h"
#include "gomoku/gomoku_game/board_state.h"
#include "gomoku/gomoku_game/game.h"

namespace e8 {
namespace {

GomokuActionId BestAction(std::unordered_map<GomokuActionId, float> const &optimal_policy) {
    assert(!optimal_policy.empty());
    GomokuActionId best_action = -1;
    float best_p = -1.0f;
    for (auto const &[action_id, p] : optimal_policy) {
        if (p > best_p) {
            best_action = action_id;
            best_p = p;
        }
    }
    return best_action;
}

} // namespace

MctsAgentPlayer::MctsAgentPlayer(std::shared_ptr<GomokuEvaluatorInterface> const &evaluator)
    : evaluator_(evaluator) {}

GomokuActionId MctsAgentPlayer::NextPlayerAction(GomokuBoardState const &board_state) {
    std::unordered_map<GomokuActionId, float> optimal_policy =
        MctSearchFrom(board_state, evaluator_.get());
    return BestAction(optimal_policy);
}

void MctsAgentPlayer::OnGomokuGameBegin(GomokuBoardState const & /*board_state*/) {}

void MctsAgentPlayer::AfterGomokuActionApplied(GomokuBoardState const & /*board_state*/) {}

void MctsAgentPlayer::OnGameEnded(GomokuBoardState const & /*board_state*/) {}

bool MctsAgentPlayer::WantAnotherGame() { return true; }

} // namespace e8
