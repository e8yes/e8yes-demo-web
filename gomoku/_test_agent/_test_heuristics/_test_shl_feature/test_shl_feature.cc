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

#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <unordered_set>
#include <utility>
#include <vector>

#include "common/unit_test_util/unit_test_util.h"
#include "gomoku/agent/heuristics/shl_feature.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/game/board_state.h"

bool EmptyBoardShlFeatureTest() {
    e8::GomokuBoardState board(/*width=*/11, /*height=*/11);

    e8::ShlFeatureBuilder features_builder(board);
    std::vector<std::pair<e8::MovePosition, e8::ShlComponents>> shl_map =
        features_builder.TopKMapSparse(/*top_k=*/8, /*normalized=*/true, e8::StoneType::ST_BLACK);

    TEST_CONDITION(shl_map.empty());

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

    e8::ShlFeatureBuilder features_builder(board);
    std::vector<std::pair<e8::MovePosition, e8::ShlComponents>> raw_shl_map =
        features_builder.TopKMapSparse(/*top_k=*/121, /*normalized=*/true, e8::StoneType::ST_BLACK);
    TEST_CONDITION(raw_shl_map.size() == 55);

    std::vector<std::pair<e8::MovePosition, e8::ShlComponents>> shl_map =
        features_builder.TopKMapSparse(/*top_k=*/5, /*normalized=*/true, e8::StoneType::ST_BLACK);

    TEST_CONDITION(shl_map.size() == 5);
    TEST_CONDITION(shl_map[0].first == e8::MovePosition(3, 3));
    TEST_CONDITION(std::abs(e8::ToShlScore(shl_map[0].second) - 0.281946123f) < 1e-3f);

    TEST_CONDITION(shl_map[1].first == e8::MovePosition(7, 7));
    TEST_CONDITION(std::abs(e8::ToShlScore(shl_map[1].second) - 0.209840357f) < 1e-3f);

    TEST_CONDITION(shl_map[2].first == e8::MovePosition(2, 2));
    TEST_CONDITION(std::abs(e8::ToShlScore(shl_map[2].second) - 0.187177047f) < 1e-3f);

    TEST_CONDITION(shl_map[3].first == e8::MovePosition(5, 3));
    TEST_CONDITION(std::abs(e8::ToShlScore(shl_map[3].second) - 0.168336704f) < 1e-3f);

    TEST_CONDITION(shl_map[4].first == e8::MovePosition(8, 8));
    TEST_CONDITION(std::abs(e8::ToShlScore(shl_map[4].second) - 0.152699724f) < 1e-3f);

    return true;
}

bool ExampleBoardIncrementalShlFeatureTest() {
    e8::GomokuBoardState board(/*width=*/11, /*height=*/11);

    e8::ShlFeatureBuilder features_builder(board);

    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(4, 6)),
                      /*cached_game_result=*/std::nullopt);
    features_builder.AddStone(board);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(5, 5)),
                      /*cached_game_result=*/std::nullopt);
    features_builder.AddStone(board);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(5, 2)),
                      /*cached_game_result=*/std::nullopt);
    features_builder.AddStone(board);

    board.ApplyAction(board.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_WHITE),
                      /*cached_game_result=*/std::nullopt);

    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(9, 6)),
                      /*cached_game_result=*/std::nullopt);
    features_builder.AddStone(board);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(6, 6)),
                      /*cached_game_result=*/std::nullopt);
    features_builder.AddStone(board);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(6, 5)),
                      /*cached_game_result=*/std::nullopt);
    features_builder.AddStone(board);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(6, 3)),
                      /*cached_game_result=*/std::nullopt);
    features_builder.AddStone(board);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(8, 5)),
                      /*cached_game_result=*/std::nullopt);
    features_builder.AddStone(board);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(4, 3)),
                      /*cached_game_result=*/std::nullopt);
    features_builder.AddStone(board);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(4, 2)),
                      /*cached_game_result=*/std::nullopt);
    features_builder.AddStone(board);
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(4, 4)),
                      /*cached_game_result=*/std::nullopt);
    features_builder.AddStone(board);

    std::vector<std::pair<e8::MovePosition, e8::ShlComponents>> raw_shl_map =
        features_builder.TopKMapSparse(/*top_k=*/121, /*normalized=*/true, e8::StoneType::ST_BLACK);
    TEST_CONDITION(raw_shl_map.size() == 55);

    std::vector<std::pair<e8::MovePosition, e8::ShlComponents>> shl_map =
        features_builder.TopKMapSparse(/*top_k=*/5, /*normalized=*/true, e8::StoneType::ST_BLACK);

    TEST_CONDITION(shl_map.size() == 5);
    TEST_CONDITION(shl_map[0].first == e8::MovePosition(3, 3));
    TEST_CONDITION(std::abs(e8::ToShlScore(shl_map[0].second) - 0.281946123f) < 1e-3f);

    TEST_CONDITION(shl_map[1].first == e8::MovePosition(7, 7));
    TEST_CONDITION(std::abs(e8::ToShlScore(shl_map[1].second) - 0.209840357f) < 1e-3f);

    TEST_CONDITION(shl_map[2].first == e8::MovePosition(2, 2));
    TEST_CONDITION(std::abs(e8::ToShlScore(shl_map[2].second) - 0.187177047f) < 1e-3f);

    TEST_CONDITION(shl_map[3].first == e8::MovePosition(5, 3));
    TEST_CONDITION(std::abs(e8::ToShlScore(shl_map[3].second) - 0.168336704f) < 1e-3f);

    TEST_CONDITION(shl_map[4].first == e8::MovePosition(8, 8));
    TEST_CONDITION(std::abs(e8::ToShlScore(shl_map[4].second) - 0.152699724f) < 1e-3f);

    return true;
}

bool ShlFeatureBuilderCacheStartsFromEmptyBoardTest() {
    e8::GomokuBoardState board(/*width=*/11, /*height=*/11);

    e8::MctNodeId next_id = 1;

    e8::ShlFeatureBuilderCache cache;
    e8::ShlFeatureBuilder const &builder1 =
        cache.Update(/*parent_state_id=*/std::nullopt, /*state_id=*/next_id, board);
    auto feature_map = builder1.TopKMapSparse(/*top_k=*/121, /*normalized=*/true,
                                              /*next_move_stone_type=*/std::nullopt);
    TEST_CONDITION(feature_map.empty());

    ++next_id;
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(4, 6)),
                      /*cached_game_result=*/std::nullopt);
    e8::ShlFeatureBuilder const &builder2 =
        cache.Update(/*parent_state_id=*/next_id - 1, /*state_id=*/next_id, board);
    auto feature_map_dense = builder2.TopKMapDense(/*top_k=*/121, /*normalized=*/true,
                                                   /*next_move_stone_type=*/std::nullopt);
    auto ground_truth_feature_map =
        e8::ShlFeatureBuilder(board).TopKMapDense(/*top_k=*/121, /*normalized=*/true,
                                                  /*next_move_stone_type=*/std::nullopt);
    TEST_CONDITION(feature_map_dense == ground_truth_feature_map);

    ++next_id;
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(5, 5)),
                      /*cached_game_result=*/std::nullopt);
    e8::ShlFeatureBuilder const &builder3 =
        cache.Update(/*parent_state_id=*/next_id - 1, /*state_id=*/next_id, board);
    feature_map_dense = builder3.TopKMapDense(/*top_k=*/121, /*normalized=*/true,
                                              /*next_move_stone_type=*/std::nullopt);
    ground_truth_feature_map =
        e8::ShlFeatureBuilder(board).TopKMapDense(/*top_k=*/121, /*normalized=*/true,
                                                  /*next_move_stone_type=*/std::nullopt);
    TEST_CONDITION(feature_map_dense == ground_truth_feature_map);

    return true;
}

bool ShlFeatureBuilderCacheMissingParentTest() {
    e8::GomokuBoardState board(/*width=*/11, /*height=*/11);

    e8::MctNodeId next_id = 1;

    e8::ShlFeatureBuilderCache cache;
    ++next_id;
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(4, 6)),
                      /*cached_game_result=*/std::nullopt);
    e8::ShlFeatureBuilder const &builder2 =
        cache.Update(/*parent_state_id=*/next_id - 1, /*state_id=*/next_id, board);
    auto feature_map = builder2.TopKMapDense(/*top_k=*/121, /*normalized=*/true,
                                             /*next_move_stone_type=*/std::nullopt);
    auto ground_truth_feature_map =
        e8::ShlFeatureBuilder(board).TopKMapDense(/*top_k=*/121, /*normalized=*/true,
                                                  /*next_move_stone_type=*/std::nullopt);
    TEST_CONDITION(feature_map == ground_truth_feature_map);

    ++next_id;
    board.ApplyAction(board.MovePositionToActionId(e8::MovePosition(5, 5)),
                      /*cached_game_result=*/std::nullopt);
    e8::ShlFeatureBuilder const &builder3 =
        cache.Update(/*parent_state_id=*/next_id - 1, /*state_id=*/next_id, board);
    feature_map = builder3.TopKMapDense(/*top_k=*/121, /*normalized=*/true,
                                        /*next_move_stone_type=*/std::nullopt);
    ground_truth_feature_map =
        e8::ShlFeatureBuilder(board).TopKMapDense(/*top_k=*/121, /*normalized=*/true,
                                                  /*next_move_stone_type=*/std::nullopt);
    TEST_CONDITION(feature_map == ground_truth_feature_map);

    return true;
}

int main() {
    e8::BeginTestSuite("shl_feature");
    e8::RunTest("EmptyBoardShlFeatureTest", EmptyBoardShlFeatureTest);
    e8::RunTest("ExampleBoardShlFeatureTest", ExampleBoardShlFeatureTest);
    e8::RunTest("ExampleBoardIncrementalShlFeatureTest", ExampleBoardIncrementalShlFeatureTest);
    e8::RunTest("ShlFeatureBuilderCacheStartsFromEmptyBoardTest",
                ShlFeatureBuilderCacheStartsFromEmptyBoardTest);
    e8::RunTest("ShlFeatureBuilderCacheMissingParentTest", ShlFeatureBuilderCacheMissingParentTest);
    e8::EndTestSuite();
    return 0;
}
