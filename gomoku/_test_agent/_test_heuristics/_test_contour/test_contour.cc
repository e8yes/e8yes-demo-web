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

#include <cstdint>
#include <unordered_set>

#include "common/unit_test_util/unit_test_util.h"
#include "gomoku/agent/heuristics/contour.h"
#include "gomoku/game/board_state.h"

std::unordered_set<e8::MovePosition> BruteForceContour(e8::GomokuBoardState const &board) {
    std::unordered_set<e8::MovePosition> contour;

    for (int8_t y = 0; y < board.Height(); ++y) {
        for (int8_t x = 0; x < board.Width(); ++x) {
            if (e8::IsContour(x, y, board)) {
                contour.insert(e8::MovePosition(x, y));
            }
        }
    }

    return contour;
}

bool FirstContourBuilderTest() {
    e8::GomokuBoardState board(/*width=*/11, /*height=*/11);

    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(4, 6)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(5, 5)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(5, 2)),
                      /*cached_game_result=*/std::nullopt);

    board.ApplyAction(board.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_WHITE),
                      /*cached_game_result=*/std::nullopt);

    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(9, 6)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(6, 6)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(6, 5)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(6, 3)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(8, 5)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(4, 3)),
                      /*cached_game_result=*/std::nullopt);

    // Initial build test.
    e8::ContourBuilder contour_builder(board, /*order=*/1);

    auto ground_truth = BruteForceContour(board);
    TEST_CONDITION(contour_builder.Contour() == ground_truth);

    // Incremental update test.
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(4, 2)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(4, 4)),
                      /*cached_game_result=*/std::nullopt);

    contour_builder.AddStone(e8::MovePosition(4, 2));
    contour_builder.AddStone(e8::MovePosition(4, 4));

    ground_truth = BruteForceContour(board);
    TEST_CONDITION(contour_builder.Contour() == ground_truth);

    return true;
}

int main() {
    e8::BeginTestSuite("contour");
    e8::RunTest("FirstContourBuilderTest", FirstContourBuilderTest);
    e8::EndTestSuite();
    return 0;
}
