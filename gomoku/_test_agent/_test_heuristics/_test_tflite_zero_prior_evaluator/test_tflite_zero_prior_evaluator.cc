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

#include <cmath>
#include <memory>
#include <optional>

#include "common/unit_test_util/unit_test_util.h"
#include "gomoku/agent/heuristics/tflite_zero_prior_evaluator.h"
#include "gomoku/agent/search/mct_search.h"
#include "gomoku/game/board_state.h"

bool EvaluationResultPythonConsistencyTest() {
    e8::GomokuBoardState board(/*width=*/11, /*height=*/11);

    // Player A will very likely win in this case whereas Player B will lose.
    //    0 1 2 3 4 5 6 7 8 9 10
    // 0  - - - - - - - - - - -
    // 1  - - - - - - - - - x -
    // 2  - - - - - - - - - - -
    // 3  - - - o o o - - - - -
    // 4  x - - x - o - - - - -
    // 5  - - - - - o - - - - -
    // 6  - x - - - x - - - - -
    // 7  - - x - - - - - - - -
    // 8  - - - - - - - - - - -
    // 9  - - - - - - - - - - -
    // 10 - - - - - - - - - - -

    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/3, /*y=*/4)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/5, /*y=*/6)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/3, /*y=*/3)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_BLACK),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/4, /*y=*/3)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/0, /*y=*/4)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/5, /*y=*/3)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/9, /*y=*/1)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/5, /*y=*/4)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/1, /*y=*/6)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/5, /*y=*/5)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/2, /*y=*/7)),
                      /*cached_game_result=*/std::nullopt);

    e8::GomokuTfliteZeroPriorEvaluator evaluator(
        /*model_path=*/"./gomoku/agent_classroom/tfmodel/gomoku_cnn_shared_i11.tflite");

    float reward = evaluator.EvaluateReward(board, /*state_id=*/3);
    TEST_CONDITION(std::abs(reward - 0.922) < 0.05f);

    std::unordered_map<e8::GomokuActionId, float> policy =
        evaluator.EvaluatePolicy(board, /*state_id=*/3);
    e8::GomokuActionId best_action_id = e8::BestAction(policy);
    TEST_CONDITION(best_action_id ==
                   board.MovePositionToActionId(e8::MovePosition(/*x=*/6, /*y=*/3)));

    return true;
}

int main() {
    e8::BeginTestSuite("tflite_zero_prior_evaluator");
    // e8::RunTest("EvaluationResultPythonConsistencyTest", EvaluationResultPythonConsistencyTest);
    e8::EndTestSuite();
    return 0;
}
