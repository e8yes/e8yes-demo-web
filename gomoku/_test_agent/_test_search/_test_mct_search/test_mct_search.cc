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

#include <memory>
#include <optional>
#include <unordered_map>

#include "common/container/mutable_priority_queue.h"
#include "common/unit_test_util/unit_test_util.h"
#include "gomoku/agent/heuristics/light_rollout_evaluator.h"
#include "gomoku/agent/search/mct_search.h"
#include "gomoku/game/board_state.h"

bool Test1() {
    auto evaluator = std::make_shared<e8::GomokuLightRolloutEvaluator>();
    e8::MctSearcher searcher(std::static_pointer_cast<e8::GomokuEvaluatorInterface>(evaluator),
                             /*print_stats=*/true);
    e8::MutablePriorityQueue<e8::MctNode>::iterator node = searcher.Root();

    // - - - - - - - - - - -
    // - - - - - - - - - - -
    // - - - - - - - - - - -
    // - - - - x - - - - - -
    // - - - - - - - - - - -
    // - - - - - - - - - - -
    // - - - x - - - - - o -
    // - - - - - - - - - o -
    // - - - - - - - - - - -
    // - - - - - - - - - - -
    // - - - - - - - - - - -
    e8::GomokuBoardState board(/*width=*/11, /*height=*/11);
    e8::GomokuActionId action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/3, /*y=*/6));
    node = searcher.SelectAction(node, board, action_id);
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/4, /*y=*/3));
    node = searcher.SelectAction(node, board, action_id);
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/9, /*y=*/7));
    node = searcher.SelectAction(node, board, action_id);
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    action_id = board.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_WHITE);
    node = searcher.SelectAction(node, board, action_id);
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/9, /*y=*/6));
    node = searcher.SelectAction(node, board, action_id);
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    e8::GomokuLightRolloutEvaluator eval;
    std::unordered_map<e8::GomokuActionId, float> policy =
        searcher.SearchFrom(board, &node, /*temperature=*/1.0f);

    e8::GomokuActionId best_action = e8::BestAction(policy);

    TEST_CONDITION(best_action ==
                       board.MovePositionToActionId(e8::MovePosition(/*x=*/9, /*y=*/5)) ||
                   best_action == board.MovePositionToActionId(e8::MovePosition(/*x=*/9,
                                                                                /*y=*/8)));
    searcher.SelectAction(node, board, best_action);

    return true;
}

int main() {
    e8::BeginTestSuite("mct_search");
    e8::RunTest("Test1", Test1);
    e8::EndTestSuite();
    return 0;
}
