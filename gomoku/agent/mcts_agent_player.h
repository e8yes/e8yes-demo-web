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

#ifndef MCTS_AGENT_PLAYER_H
#define MCTS_AGENT_PLAYER_H

#include <memory>
#include <optional>

#include "common/container/mutable_priority_queue.h"
#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/agent/search/mct_search.h"
#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"

namespace e8 {

/**
 * @brief The MctsAgentPlayer class An artificial intelligent agent that uses MCTS to derive optimal
 * board actions.
 */
class MctsAgentPlayer : public GomokuPlayerInterface {
  public:
    MctsAgentPlayer(std::shared_ptr<MctSearcher> const &searcher);
    ~MctsAgentPlayer() override = default;

    GomokuActionId NextPlayerAction(GomokuBoardState const &board_state) override;

    void BeforeGomokuActionApplied(GomokuBoardState const &board_state,
                                   PlayerSide const action_performed_by,
                                   GomokuActionId const &incoming_action_id) override;

    virtual void OnGomokuGameBegin(GomokuBoardState const &board_state) override;

    virtual void AfterGomokuActionApplied(GomokuBoardState const &board_state) override;

    virtual void OnGameEnded(GomokuBoardState const &board_state) override;

    virtual bool WantAnotherGame() override;

  private:
    std::shared_ptr<MctSearcher> searcher_;
    std::optional<MutablePriorityQueue<MctNode>::iterator> current_node_it_;
};

} // namespace e8

#endif // MCTS_AGENT_PLAYER_H
