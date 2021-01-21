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

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>
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
using StoneState = StoneType;

/**
 * @brief The MovePosition struct Zero-offset 2D board position. Orgin is at the top-left
 * corner.
 */
struct MovePosition {
    int8_t x;
    int8_t y;

    MovePosition(int8_t const x, int8_t const y);

    bool operator==(MovePosition const &other) const;
};

} // namespace e8

namespace std {
template <> struct hash<e8::MovePosition> {
    size_t operator()(e8::MovePosition const &pos) const {
        return std::hash<int16_t>()(static_cast<int16_t>(pos.x) << 8 | pos.y);
    }
};
} // namespace std

namespace e8 {

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
using GomokuActionId = int16_t;

/**
 * @brief The GomokuActionRecord struct Stores the information about an action made so as to keep
 * track of the board history.
 */
struct GomokuActionRecord {
    GamePhase const game_phase;
    PlayerSide const side;
    std::pair<GomokuActionId, GomokuAction> const action;

    GomokuActionRecord(GamePhase const game_phase, PlayerSide const side,
                       GomokuActionId const action_id, GomokuAction const &action);
};

/**
 * @brief The GomokuBoardState class Represents the state of the chess board.
 *
 * The board state is defined by the tuple <ChessBoard, PlayerSide, GamePhase, GameResult>.
 * Thread-safety is not guaranteed.
 */
class GomokuBoardState {
  public:
    /**
     * @brief BoardState Constructs an empty board.
     *
     * @param width Board width.
     * @param height Board height.
     */
    GomokuBoardState(int16_t const width, int16_t const height);

    GomokuBoardState(GomokuBoardState const &other);
    GomokuBoardState(GomokuBoardState &&other) = default;
    ~GomokuBoardState() = default;

    /**
     * @brief LegalActions The set of legal actions that can be made by the CurrentPlayerSide()
     * given the board state.
     */
    std::unordered_map<GomokuActionId, GomokuAction> const &LegalActions() const;

    /**
     * @brief ActionIdRange Since action IDs are compact. Knowing the range the action IDs this
     * board will take makes flattening action data possible.
     */
    std::pair<GomokuActionId, GomokuActionId> ActionIdRange() const;

    /**
     * @brief MovePositionToActionId Converts a move position to the standard action ID acceptable
     * by the ApplyAction() function.
     */
    GomokuActionId MovePositionToActionId(MovePosition const &pos) const;

    /**
     * @brief Swap2DecisionToActionId Converts a swap2 decision to the standard action ID acceptable
     * by the ApplyAction() function.
     */
    GomokuActionId Swap2DecisionToActionId(Swap2Decision const decision) const;

    /**
     * @brief StoneTypeDecisionToActionId Converts a stone type decision to the standard action ID
     * acceptable by the ApplyAction() function.
     */
    GomokuActionId StoneTypeDecisionToActionId(StoneTypeDecision const decision) const;

    /**
     * @brief CurrentPlayerSide The current player who is going to make the next action.
     */
    PlayerSide CurrentPlayerSide() const;

    /**
     * @brief PlayerStoneType Returns the type of stone the specified player is currently holding.
     * The player might not hold any specific type of stone during the game. In that case, the
     * return value will be ST_NONE.
     */
    StoneType PlayerStoneType(PlayerSide const player_side) const;

    /**
     * @brief CurrentGamePhase The current game phase.
     */
    GamePhase CurrentGamePhase() const;

    /**
     * @brief ApplyAction Make an action and update the board state. See the class explanation above
     * for what board state refers to. This function will fail if the game result has changed from
     * GR_UNDETERMINED. It will also update the history of records of the actions previously
     * applied.
     *
     * @param action_id Id of the action in one of the LegalActions().
     * @param cached_game_result Externally provide the cached game result to avoid repeatedly
     * checking the board.
     * @return The game result after the move is made.
     */
    GameResult ApplyAction(GomokuActionId const action_id,
                           std::optional<GameResult> const cached_game_result);

    /**
     * @brief RetractAction Undo the last action made to the board and restore the state.
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
     * @brief ChessPieceStateAt Retrieve the state of the move position.
     */
    StoneState *ChessPieceStateAt(MovePosition const &pos);

    /**
     * @brief ChessPieceStateAt Constant version of the above function.
     */
    StoneState const *ChessPieceStateAt(MovePosition const &pos) const;

    /**
     * @brief History Returns a history of action records.
     */
    std::vector<GomokuActionRecord> const &History() const;

    /**
     * @brief Width Board width.
     */
    int16_t Width() const;

    /**
     * @brief Height Board height.
     */
    int16_t Height() const;

    /**
     * @brief PrintBoard For debugging purposes, prints the board out to the standard output stream
     * in a human readable form.
     */
    void PrintBoard() const;

  private:
    uint8_t MaxConnectedStonesFrom(MovePosition const &pos, StoneType const stone_type) const;

    int16_t const width_;
    int16_t const height_;
    GameResult game_result_;
    GamePhase current_game_phase_;
    PlayerSide current_player_side_;

    std::unique_ptr<StoneState[]> board_;
    std::array<StoneType, 2> player_stone_type_;

    std::vector<GomokuActionRecord> history_;

    std::unordered_map<GomokuActionId, GomokuAction> swap2_decision_legal_actions_;
    std::unordered_map<GomokuActionId, GomokuAction> stone_type_decision_legal_actions_;
    std::unordered_map<GomokuActionId, GomokuAction> standard_gomoku_legal_actions_;
};

} // namespace e8

#endif // BOARD_STATE_H
