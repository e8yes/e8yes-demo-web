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
#include <optional>
#include <unordered_map>

#include "common/container/mutable_priority_queue.h"
#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/mcts_agent_player.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/agent/search/mct_search.h"
#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"

namespace e8 {

MctsAgentPlayer::MctsAgentPlayer(std::shared_ptr<MctSearcher> const &searcher)
    : searcher_(searcher) {}

void MctsAgentPlayer::OnGomokuGameBegin(GomokuBoardState const & /*board_state*/) {
    searcher_->Reset();
    current_node_it_ = searcher_->Root();
}

GomokuActionId MctsAgentPlayer::NextPlayerAction(GomokuBoardState const &board_state) {
    assert(current_node_it_.has_value());

    std::unordered_map<GomokuActionId, float> optimal_policy =
        searcher_->SearchFrom(board_state, &current_node_it_.value(), /*temperature=*/1.0f);

    return BestAction(optimal_policy);
}

void MctsAgentPlayer::BeforeGomokuActionApplied(GomokuBoardState const &board_state,
                                                PlayerSide const /*action_performed_by*/,
                                                GomokuActionId const &incoming_action_id) {
    current_node_it_ = searcher_->SelectAction(*current_node_it_, board_state, incoming_action_id);
}

void MctsAgentPlayer::AfterGomokuActionApplied(GomokuBoardState const & /*board_state*/) {}

void MctsAgentPlayer::OnGameEnded(GomokuBoardState const & /*board_state*/) {}

bool MctsAgentPlayer::WantAnotherGame() { return true; }

} // namespace e8
