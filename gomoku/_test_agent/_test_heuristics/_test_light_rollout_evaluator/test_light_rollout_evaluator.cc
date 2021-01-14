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
#include "gomoku/agent/heuristics/light_rollout_evaluator.h"
#include "gomoku/game/board_state.h"

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

bool RewardEvaluationTest() {
    e8::GomokuBoardState board(/*width=*/7, /*height=*/7);

    // Player A will very likely win in this case whereas Player B will lose.
    // - - - - - - o
    // - - - - - - -
    // - - x - o - -
    // - - - x - - -
    // - - - - x - -
    // - - - - - - -
    // - - - - - - -
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/2, /*y=*/2)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/3, /*y=*/3)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/4, /*y=*/2)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_WHITE),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/6, /*y=*/0)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/4, /*y=*/4)),
                      /*cached_game_result=*/std::nullopt);

    e8::GomokuLightRolloutEvaluator evaluator;
    float player_b_reward = evaluator.EvaluateReward(board, /*state_id=*/3);
    TEST_CONDITION(player_b_reward < 0);

    // - - - - - - o
    // - - - - - o -
    // - - x - o - -
    // - - - x - - -
    // - - - - x - -
    // - - - - - - -
    // - - - - - - -
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/5, /*y=*/1)),
                      /*cached_game_result=*/std::nullopt);

    float player_a_reward = evaluator.EvaluateReward(board, /*state_id=*/4);
    TEST_CONDITION(player_a_reward > 0);

    // Using the cache should give the same result.
    float player_a_reward_cached = evaluator.EvaluateReward(board, /*state_id=*/4);
    TEST_CONDITION(player_a_reward_cached == player_a_reward);

    return true;
}

bool PolicyEvaluationTest() {
    e8::GomokuBoardState board(/*width=*/7, /*height=*/7);

    // - - - - - - o
    // - - - - - - -
    // - - x - o - -
    // - - - x - - -
    // - - - - x - -
    // - - - - - - -
    // - - - - - - -
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/2, /*y=*/2)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/3, /*y=*/3)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/4, /*y=*/2)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_WHITE),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/6, /*y=*/0)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(/*x=*/4, /*y=*/4)),
                      /*cached_game_result=*/std::nullopt);

    e8::GomokuLightRolloutEvaluator evaluator;
    std::unordered_map<e8::GomokuActionId, float> policy =
        evaluator.EvaluatePolicy(board, /*state_id=*/2);

    // Contour
    // - - - - - ? o
    // - ? ? ? ? ? ?
    // - ? x ? o ? -
    // - ? ? x ? ? -
    // - - ? ? x ? -
    // - - - ? ? ? -
    // - - - - - - -
    TEST_CONDITION(policy.size() == 7 * 7 - 5);
    unsigned num_zero = 0;
    unsigned num_nonzero = 0;
    for (auto const &[action_id, p] : policy) {
        if (p == 0) {
            ++num_zero;
        } else {
            TEST_CONDITION(p == 1.0f / 20.0f);
            ++num_nonzero;
        }
    }

    TEST_CONDITION(num_zero == 24);
    TEST_CONDITION(num_nonzero == 20);

    return true;
}

int main() {
    e8::BeginTestSuite("light_rollout_evaluator");
    e8::RunTest("ContourTest", ContourTest);
    e8::RunTest("RewardEvaluationTest", RewardEvaluationTest);
    e8::RunTest("PolicyEvaluationTest", PolicyEvaluationTest);
    e8::EndTestSuite();
    return 0;
}
