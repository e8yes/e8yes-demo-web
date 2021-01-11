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

#include <optional>
#include <unordered_map>

#include "common/unit_test_util/unit_test_util.h"
#include "gomoku/gomoku_agent/heuristics/light_rollout_evaluator.h"
#include "gomoku/gomoku_agent/search/mct_search.h"
#include "gomoku/gomoku_game/board_state.h"

bool Test1() {
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
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/3, /*y=*/6)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/4, /*y=*/3)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/9, /*y=*/7)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_WHITE),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/9, /*y=*/6)),
                      /*cached_game_result=*/std::nullopt);

    e8::GomokuLightRolloutEvaluator eval;
    std::unordered_map<e8::GomokuActionId, float> policy =
        e8::MctSearchFrom(board, &eval, /*print_stats=*/true);
    e8::GomokuActionId best_action = e8::BestAction(policy);

    TEST_CONDITION(best_action ==
                       board.MovePositionToActionId(e8::MovePosition(/*x=*/9, /*y=*/5)) ||
                   best_action == board.MovePositionToActionId(e8::MovePosition(/*x=*/9,
                                                                                /*y=*/8)));

    return true;
}

int main() {
    e8::BeginTestSuite("mct_search");
    e8::RunTest("Test1", Test1);
    e8::EndTestSuite();
    return 0;
}
