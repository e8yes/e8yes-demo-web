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

#include <iostream>
#include <memory>
#include <optional>

#include "common/unit_test_util/unit_test_util.h"
#include "gomoku/gomoku_agent/heuristics/light_rollout_evaluator.h"
#include "gomoku/gomoku_game/board_state.h"

bool ContourTest() {
    e8::GomokuBoardState board(/*width=*/5, /*height=*/5);

    // - - - - -
    // - x - - -
    // - - - - -
    // - - - x o
    // - - - - -
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/1, /*y=*/1)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/3, /*y=*/3)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/4, /*y=*/3)),
                      /*cached_game_result=*/std::nullopt);
    board.PrintBoard();

    // Contour
    // ? ? ? - -
    // ? x ? - -
    // ? ? ? ? ?
    // - - ? x o
    // - - ? ? ?
    std::shared_ptr<e8::light_rollout_evaluator_internal::BoardContour> contour =
        e8::light_rollout_evaluator_internal::BuildBoardContour(board);

    TEST_CONDITION(contour->positions.size() == 14);
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/0, /*y=*/0)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/1, /*y=*/0)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/2, /*y=*/0)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/0, /*y=*/1)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/2, /*y=*/1)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/0, /*y=*/2)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/1, /*y=*/2)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/2, /*y=*/2)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/3, /*y=*/2)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/4, /*y=*/2)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/2, /*y=*/3)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/2, /*y=*/4)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/3, /*y=*/4)) !=
                   contour->positions.end());
    TEST_CONDITION(contour->positions.find(e8::MovePosition(/*x=*/4, /*y=*/4)) !=
                   contour->positions.end());

    return true;
}

int main() {
    e8::BeginTestSuite("light_rollout_evaluator");
    e8::RunTest("ContourTest", ContourTest);
    e8::EndTestSuite();
    return 0;
}
