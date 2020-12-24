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

#ifndef BOARD_STATE_H
#define BOARD_STATE_H

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace e8 {

enum GameResult {
    GR_UNDETERMINED,
    GR_PLAYER_A_WIN,
    GR_PLAYER_B_WIN,
    GR_TIE,
};

enum PlayerSide { PS_PLAYER_A, PS_PLAYER_B };

enum StoneType { ST_NONE, ST_BLACK, ST_WHITE };

enum GamePhase {
    GP_PLACE_3_STONES,
    GP_SWAP2_DECISION,
    GP_SWAP2_PLACE_2_STONES,
    GP_STONE_TYPE_DECISION,
    GP_STANDARD_GOMOKU
};

/**
 * @brief OffensiveSide Indicate which side will initiate the move.
 */
PlayerSide OffensiveSide();

/**
 * @brief The StoneState struct Stores the state of a chess piece. If the stone_type is
 * ST_NONE, it means that no stone has been placed to the chess piece position.
 */
struct StoneState {
    StoneType stone_type;

    StoneState();
    explicit StoneState(StoneType const stone_type);
};

/**
 * @brief The MovePosition struct Zero-offset 2D board position. Orgin is at the top-left
 * corner.
 */
struct MovePosition {
    int x;
    int y;

    MovePosition(int const x, int const y);

    bool operator==(MovePosition const &other) const;
};

/**
 * @brief The Swap2Decision enum Decisions available during GP_SWAP2_DECISION.
 */
enum Swap2Decision { SW2D_CHOOSE_WHITE, SW2D_CHOOSE_BLACK, SW2D_PLACE_2_STONES };

/**
 * @brief The StoneTypeDecision enum Decisions avaliable during GP_SWAP2_DECISION_EXCHANGED.
 */
enum StoneTypeDecision { STD_CHOOSE_WHITE, STD_CHOOSE_BLACK };

/**
 * @brief The GomokuAction struct An action is one of the three types occurred during different
 * phases of the game. See the belowed comments for how each type of action is used.
 */
struct GomokuAction {
    // Placing a stone on the board during GP_PLACE_3_STONES, GP_SWAP2_PLACE_2_STONES and
    // GP_STANDARD_GOMOKU.
    std::optional<MovePosition> const stone_pos;

    // Making a swap2 decision during GP_SWAP2_DECISION.
    std::optional<Swap2Decision> const swap2_decision;

    // Making a stone type decision during GP_SWAP2_DECISION_EXCHANGED.
    std::optional<StoneTypeDecision> const stone_type_decision;

    explicit GomokuAction(MovePosition const &stone_pos);
    explicit GomokuAction(Swap2Decision const decision);
    explicit GomokuAction(StoneTypeDecision const decision);

    GomokuAction &operator=(GomokuAction const &other);
};

// Zero-offset action ID allowing all actions in the game are densely numbered.
using GomokuActionId = int;

/**
 * @brief The GomokuActionRecord struct
 */
struct GomokuActionRecord {
    GamePhase const game_phase;
    PlayerSide const side;
    std::pair<GomokuActionId, GomokuAction> const action;

    GomokuActionRecord(GamePhase const game_phase, PlayerSide const side,
                       GomokuActionId const action_id, GomokuAction const &action);
};

/**
 * @brief The BoardState class Represents the state of the chess board. Thread-safety is not
 * guaranteed.
 */
class GomokuBoardStates {
  public:
    /**
     * @brief BoardState Constructs an empty board.
     *
     * @param width Board width.
     * @param height Board height.
     */
    GomokuBoardStates(unsigned const width, unsigned const height);

    GomokuBoardStates(GomokuBoardStates const &other);
    GomokuBoardStates(GomokuBoardStates &&other) = default;
    ~GomokuBoardStates() = default;

    /**
     * @brief LegalActions The set of legal actions that can be made by the CurrentPlayerSide()
     * given the board state.
     */
    std::unordered_map<GomokuActionId, GomokuAction> const &LegalActions() const;

    /**
     * @brief CurrentPlayerSide The current player who is going to make the next action.
     */
    PlayerSide CurrentPlayerSide() const;

    /**
     * @brief CurrentGamePhase The current game phase.
     */
    GamePhase CurrentGamePhase() const;

    /**
     * @brief ApplyAction Make an action given the current board state and update the game result.
     * This function will fail if the game result has been changed from GR_UNDETERMINED. It will
     * also update the state of CurrentPlayerSide(), CurrentGamePhase(), CurrentGameResult() as well
     * as the history of records of the actions previously applied.
     *
     * @param action_id Id of the action in one of the LegalActions().
     * @param require_game_result_update Set this to true if the action can affect the game result.
     * @return The game result after the move is made.
     */
    GameResult ApplyAction(GomokuActionId const action_id, bool require_game_result_update);

    /**
     * @brief RetractAction Undo the last action made to the board and restore the game result back
     * to GR_UNDETERMINED.
     *
     * @return The last action if there is any, otherwise it returns empty, and the function will do
     * nothing.
     */
    std::optional<GomokuActionRecord> RetractAction();

    /**
     * @brief LastAction Return the last action made to the board if there was any.
     */
    std::optional<GomokuActionRecord> LastAction() const;

    /**
     * @brief CurrentGameResult Current game result.
     */
    GameResult CurrentGameResult() const;

    /**
     * @brief CurrentBoard It returns a 1d array representing the board in row major order.
     */
    StoneState *CurrentBoard() const;

    /**
     * @brief Width Board width.
     */
    unsigned Width() const;

    /**
     * @brief Height Board height.
     */
    unsigned Height() const;

  private:
    StoneState *ChessPieceStateAt(MovePosition const &pos);
    StoneState const *ChessPieceStateAt(MovePosition const &pos) const;

    unsigned MaxConnectedStonesFrom(MovePosition const &pos, StoneType const stone_type) const;

    unsigned const width_;
    unsigned const height_;
    GameResult game_result_;
    GamePhase current_game_phase_;
    PlayerSide current_player_side_;

    std::unique_ptr<StoneState[]> board_;
    std::array<std::optional<StoneType>, 2> player_stone_type_;

    std::vector<GomokuActionRecord> history_;

    std::unordered_map<GomokuActionId, GomokuAction> swap2_decision_legal_actions_;
    std::unordered_map<GomokuActionId, GomokuAction> stone_type_decision_legal_actions_;
    std::unordered_map<GomokuActionId, GomokuAction> standard_gomoku_legal_actions_;
};

} // namespace e8

#endif // BOARD_STATE_H
