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
#include "gomoku/gomoku_game/board_state.h"

bool BasicGameStateTest() {
    e8::GomokuBoardState board(/*width=*/5, /*height=*/4);

    TEST_CONDITION(board.Width() == 5);
    TEST_CONDITION(board.Height() == 4);
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_PLACE_3_STONES);
    TEST_CONDITION(board.CurrentGameResult() == e8::GameResult::GR_UNDETERMINED);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.LegalActions().size() == 4 * 5);

    return true;
}

bool ApplyAndRetractActionsSwap2DecisionTest() {
    e8::GomokuBoardState board(/*width=*/5, /*height=*/4);

    // Places 3 stones.
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_PLACE_3_STONES);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_BLACK);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 4 * 5);
    e8::GomokuActionId action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/0, /*y=*/0));
    TEST_CONDITION(board.LegalActions().find(action_id) != board.LegalActions().end());
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(board.LegalActions().find(action_id) == board.LegalActions().end());

    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_PLACE_3_STONES);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_BLACK);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 4 * 5 - 1);
    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/4, /*y=*/0));
    TEST_CONDITION(board.LegalActions().find(action_id) != board.LegalActions().end());
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(board.LegalActions().find(action_id) == board.LegalActions().end());

    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_PLACE_3_STONES);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_WHITE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 4 * 5 - 2);
    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/4, /*y=*/3));
    TEST_CONDITION(board.LegalActions().find(action_id) != board.LegalActions().end());
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(board.LegalActions().find(action_id) == board.LegalActions().end());

    // Swap2 chooses black.
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_SWAP2_DECISION);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_B);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 3);
    action_id = board.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_BLACK);
    TEST_CONDITION(board.LegalActions().find(action_id) != board.LegalActions().end());
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_STANDARD_GOMOKU);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_WHITE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_BLACK);

    board.RetractAction();

    // Swap2 chooses white.
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_SWAP2_DECISION);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_B);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 3);
    action_id = board.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_WHITE);
    TEST_CONDITION(board.LegalActions().find(action_id) != board.LegalActions().end());
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_STANDARD_GOMOKU);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_B);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_BLACK);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_WHITE);

    board.RetractAction();

    // Swap2 places 2 more stones.
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_SWAP2_DECISION);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_B);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 3);
    action_id = board.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_PLACE_2_STONES);
    TEST_CONDITION(board.LegalActions().find(action_id) != board.LegalActions().end());
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    // Places 2 more stones.
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_SWAP2_PLACE_2_STONES);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_B);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_WHITE);
    TEST_CONDITION(board.LegalActions().size() == 4 * 5 - 3);
    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/3, /*y=*/3));
    TEST_CONDITION(board.LegalActions().find(action_id) != board.LegalActions().end());
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(board.LegalActions().find(action_id) == board.LegalActions().end());

    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_SWAP2_PLACE_2_STONES);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_BLACK);
    TEST_CONDITION(board.LegalActions().size() == 4 * 5 - 4);
    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/3, /*y=*/0));
    TEST_CONDITION(board.LegalActions().find(action_id) != board.LegalActions().end());
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(board.LegalActions().find(action_id) == board.LegalActions().end());

    // Chooses black stone.
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_STONE_TYPE_DECISION);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 2);
    action_id = board.StoneTypeDecisionToActionId(e8::STD_CHOOSE_BLACK);
    TEST_CONDITION(board.LegalActions().find(action_id) != board.LegalActions().end());
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_STANDARD_GOMOKU);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_B);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::ST_BLACK);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::ST_WHITE);

    board.RetractAction();

    // Chooses white stone.
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_STONE_TYPE_DECISION);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 2);
    action_id = board.StoneTypeDecisionToActionId(e8::STD_CHOOSE_WHITE);
    TEST_CONDITION(board.LegalActions().find(action_id) != board.LegalActions().end());
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_STANDARD_GOMOKU);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::ST_WHITE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::ST_BLACK);

    // Retract all the way back to the beginning.
    board.RetractAction();
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_STONE_TYPE_DECISION);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 2);

    board.RetractAction();
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_SWAP2_PLACE_2_STONES);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_B);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_BLACK);
    TEST_CONDITION(board.LegalActions().size() == 4 * 5 - 4);

    board.RetractAction();
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_SWAP2_PLACE_2_STONES);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_B);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_WHITE);
    TEST_CONDITION(board.LegalActions().size() == 4 * 5 - 3);

    board.RetractAction();
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_SWAP2_DECISION);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_B);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 3);

    board.RetractAction();
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_PLACE_3_STONES);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_WHITE);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 4 * 5 - 2);

    board.RetractAction();
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_PLACE_3_STONES);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_BLACK);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 4 * 5 - 1);

    board.RetractAction();
    TEST_CONDITION(board.CurrentGamePhase() == e8::GamePhase::GP_PLACE_3_STONES);
    TEST_CONDITION(board.CurrentPlayerSide() == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A) == e8::StoneType::ST_BLACK);
    TEST_CONDITION(board.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B) == e8::StoneType::ST_NONE);
    TEST_CONDITION(board.LegalActions().size() == 4 * 5);

    return true;
}

bool GameResultTest() {
    e8::GomokuBoardState board(/*width=*/5, /*height=*/5);

    // x - - - -
    // - - - - -
    // - - - - -
    // - - - - -
    // - - - - -
    e8::GomokuActionId action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/0, /*y=*/0));
    e8::GameResult game_result = board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(game_result == e8::GameResult::GR_UNDETERMINED);

    // x - - - -
    // - x - - -
    // - - - - -
    // - - - - -
    // - - - - -
    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/1, /*y=*/1));
    game_result = board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(game_result == e8::GameResult::GR_UNDETERMINED);

    // x - - - -
    // o x - - -
    // - - - - -
    // - - - - -
    // - - - - -
    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/0, /*y=*/1));
    game_result = board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(game_result == e8::GameResult::GR_UNDETERMINED);

    action_id = board.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_BLACK);
    game_result = board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(game_result == e8::GameResult::GR_UNDETERMINED);

    // x - - - -
    // o x - - -
    // o - - - -
    // - - - - -
    // - - - - -
    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/0, /*y=*/2));
    game_result = board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(game_result == e8::GameResult::GR_UNDETERMINED);

    // x - - - -
    // o x - - -
    // o - - - -
    // - - - - -
    // - - - - x
    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/4, /*y=*/4));
    game_result = board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(game_result == e8::GameResult::GR_UNDETERMINED);

    // x - - - -
    // o x - - -
    // o - - - -
    // o - - - -
    // - - - - x
    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/0, /*y=*/3));
    game_result = board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(game_result == e8::GameResult::GR_UNDETERMINED);

    // x - - - -
    // o x - - -
    // o - - - -
    // o - - x -
    // - - - - x
    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/3, /*y=*/3));
    game_result = board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(game_result == e8::GameResult::GR_UNDETERMINED);

    // x - - - -
    // o x - - -
    // o - - - -
    // o - - x -
    // o - - - x
    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/0, /*y=*/4));
    game_result = board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(game_result == e8::GameResult::GR_UNDETERMINED);

    // x - - - -
    // o x - - -
    // o - x - -
    // o - - x -
    // o - - - x
    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/2, /*y=*/2));
    game_result = board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);
    TEST_CONDITION(game_result == e8::GameResult::GR_PLAYER_B_WIN);

    return true;
}

bool HistoryRecordTest() {
    e8::GomokuBoardState board(/*width=*/5, /*height=*/5);

    e8::GomokuActionId action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/0, /*y=*/0));
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/1, /*y=*/0));
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/2, /*y=*/0));
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    action_id = board.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_WHITE);
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    action_id = board.MovePositionToActionId(e8::MovePosition(/*x=*/3, /*y=*/0));
    board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

    TEST_CONDITION(board.History().size() == 5);

    TEST_CONDITION(board.History()[0].side == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.History()[0].game_phase == e8::GamePhase::GP_PLACE_3_STONES);
    TEST_CONDITION(board.History()[0].action.second.stone_pos.has_value());
    TEST_CONDITION(board.History()[0].action.second.stone_pos->x == 0);
    TEST_CONDITION(board.History()[0].action.second.stone_pos->y == 0);

    TEST_CONDITION(board.History()[1].side == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.History()[1].game_phase == e8::GamePhase::GP_PLACE_3_STONES);
    TEST_CONDITION(board.History()[1].action.second.stone_pos.has_value());
    TEST_CONDITION(board.History()[1].action.second.stone_pos->x == 1);
    TEST_CONDITION(board.History()[1].action.second.stone_pos->y == 0);

    TEST_CONDITION(board.History()[2].side == e8::PlayerSide::PS_PLAYER_A);
    TEST_CONDITION(board.History()[2].game_phase == e8::GamePhase::GP_PLACE_3_STONES);
    TEST_CONDITION(board.History()[2].action.second.stone_pos.has_value());
    TEST_CONDITION(board.History()[2].action.second.stone_pos->x == 2);
    TEST_CONDITION(board.History()[2].action.second.stone_pos->y == 0);

    TEST_CONDITION(board.History()[3].side == e8::PlayerSide::PS_PLAYER_B);
    TEST_CONDITION(board.History()[3].game_phase == e8::GamePhase::GP_SWAP2_DECISION);
    TEST_CONDITION(board.History()[3].action.second.swap2_decision.has_value());
    TEST_CONDITION(*board.History()[3].action.second.swap2_decision ==
                   e8::Swap2Decision::SW2D_CHOOSE_WHITE);

    TEST_CONDITION(board.History()[4].side == e8::PlayerSide::PS_PLAYER_B);
    TEST_CONDITION(board.History()[4].game_phase == e8::GamePhase::GP_STANDARD_GOMOKU);
    TEST_CONDITION(board.History()[4].action.second.stone_pos.has_value());
    TEST_CONDITION(board.History()[4].action.second.stone_pos->x == 3);
    TEST_CONDITION(board.History()[4].action.second.stone_pos->y == 0);

    return true;
}

int main() {
    e8::BeginTestSuite("board_state");
    e8::RunTest("BasicGameStateTest", BasicGameStateTest);
    e8::RunTest("ApplyAndRetractActionsSwap2DecisionTest", ApplyAndRetractActionsSwap2DecisionTest);
    e8::RunTest("GameResultTest", GameResultTest);
    e8::RunTest("HistoryRecordTest", HistoryRecordTest);
    e8::EndTestSuite();
    return 0;
}
