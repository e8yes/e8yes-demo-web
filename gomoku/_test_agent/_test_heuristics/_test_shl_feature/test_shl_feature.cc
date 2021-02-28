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
#include <optional>

#include "common/unit_test_util/unit_test_util.h"
#include "gomoku/agent/heuristics/shl_feature.h"
#include "gomoku/game/board_state.h"

bool EmptyBoardShlFeatureTest() {
    e8::GomokuBoardState board(/*width=*/11, /*height=*/11);

    e8::ShlFeatures features = e8::ComputeShlFeatures(board, e8::StoneType::ST_BLACK, /*top_k=*/8);

    TEST_CONDITION(features.width == 11);
    TEST_CONDITION(features.height == 11);
    TEST_CONDITION(features.raw_map.empty());
    TEST_CONDITION(features.normalized_top_k_map.empty());
    TEST_CONDITION(features.shl_score_total == 0.0f);

    return true;
}

bool ExampleBoardShlFeatureTest() {
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
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(4, 2)),
                      /*cached_game_result=*/std::nullopt);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(4, 4)),
                      /*cached_game_result=*/std::nullopt);

    e8::ShlFeatures features = e8::ComputeShlFeatures(board, e8::StoneType::ST_BLACK, /*top_k=*/5);

    TEST_CONDITION(features.raw_map.size() == 63);

    TEST_CONDITION(features.normalized_top_k_map.size() == 5);
    TEST_CONDITION(features.normalized_top_k_map[0].first == e8::MovePosition(3, 3));
    TEST_CONDITION(
        std::abs(e8::ToShlScore(features.normalized_top_k_map[0].second) - 0.281946123f) < 1e-3f);

    TEST_CONDITION(features.normalized_top_k_map[1].first == e8::MovePosition(7, 7));
    TEST_CONDITION(
        std::abs(e8::ToShlScore(features.normalized_top_k_map[1].second) - 0.209840357f) < 1e-3f);

    TEST_CONDITION(features.normalized_top_k_map[2].first == e8::MovePosition(2, 2));
    TEST_CONDITION(
        std::abs(e8::ToShlScore(features.normalized_top_k_map[2].second) - 0.187177047f) < 1e-3f);

    TEST_CONDITION(features.normalized_top_k_map[3].first == e8::MovePosition(5, 3));
    TEST_CONDITION(
        std::abs(e8::ToShlScore(features.normalized_top_k_map[3].second) - 0.168336704f) < 1e-3f);

    TEST_CONDITION(features.normalized_top_k_map[4].first == e8::MovePosition(8, 8));
    TEST_CONDITION(
        std::abs(e8::ToShlScore(features.normalized_top_k_map[4].second) - 0.152699724f) < 1e-3f);

    return true;
}

int main() {
    e8::BeginTestSuite("shl_feature");
    e8::RunTest("EmptyBoardShlFeatureTest", EmptyBoardShlFeatureTest);
    e8::RunTest("ExampleBoardShlFeatureTest", ExampleBoardShlFeatureTest);
    e8::EndTestSuite();
    return 0;
}
