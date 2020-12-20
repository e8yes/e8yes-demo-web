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
#include <unordered_set>
#include <vector>

namespace e8 {

enum GameResult {
    GR_UNDETERMINED,
    GR_WHITE_WIN,
    GR_BLACK_WIN,
};

enum PlayerSide { PS_NONE, PS_WHITE, PS_BLACK };

/**
 * @brief The ChessPieceState struct Stores the state of a chess piece. If the side is PS_NONE, it
 * means that no move has been made to the chess piece position.
 */
struct ChessPieceState {
    PlayerSide side;

    ChessPieceState();
    explicit ChessPieceState(PlayerSide const side);
};

/**
 * @brief The MovePosition struct
 */
struct MovePosition {
    int x;
    int y;

    MovePosition(int const x, int const y);

    bool operator==(MovePosition const &other) const;
};

} // namespace e8

namespace std {

template <> struct hash<::e8::MovePosition> {
    std::size_t operator()(::e8::MovePosition const &pos) const {
        return std::hash<uint64_t>{}((static_cast<uint64_t>(pos.x) << 32) | pos.y);
    }
};

} // namespace std

namespace e8 {

/**
 * @brief The MoveRecord struct Stores information about a move. It consists of the board position
 * of the move and which player made the move.
 */
struct MoveRecord {
    MovePosition const pos;
    PlayerSide const side;

    MoveRecord(MovePosition const &pos, PlayerSide const side);
};

/**
 * @brief The BoardState class Represents the state of the chess board.
 */
class BoardState {
  public:
    /**
     * @brief BoardState Constructs an empty board.
     *
     * @param width Board width.
     * @param height Board height.
     * @param k The number of connection to win the game.
     */
    BoardState(unsigned const width, unsigned const height, unsigned const k);

    BoardState(BoardState const &other);
    BoardState(BoardState &&other) = default;
    ~BoardState() = default;

    /**
     * @brief LegalMovePositions Legal positions that a new move can be made by the specified side.
     * The set of positions will be updated after either MakeMove() or RetractMove() is called.
     */
    std::unordered_set<MovePosition> const &LegalMovePositions(PlayerSide const &side) const;

    /**
     * @brief MakeMove Make a move at a empty position and update the game result. This function
     * will fail if the game result was changed from GR_UNDETERMINED.
     *
     * @param move The position has to be within the legal range of the board [0, width) x [0,
     * height).
     * @return The game result after the move is made.
     */
    GameResult MakeMove(MoveRecord const &move);

    /**
     * @brief RetractMove Undo the last move made to the board and restore the game result back to
     * GR_UNDETERMINED if it wasn't.
     *
     * @return The last move if there is any, otherwise it returns a empty.
     */
    std::optional<MoveRecord> RetractMove();

    /**
     * @brief CurrentGameResult Current game result.
     */
    GameResult CurrentGameResult() const;

    /**
     * @brief CurrentBoard It returns a 1d array representing the board in row major order.
     */
    ChessPieceState *CurrentBoard() const;

    /**
     * @brief Width Board width.
     */
    unsigned Width() const;

    /**
     * @brief Height Board height.
     */
    unsigned Height() const;

  private:
    ChessPieceState *ChessPieceStateAt(MovePosition const &pos);

    bool LeadToWinStateFrom(MoveRecord const &move);

    unsigned const width_;
    unsigned const height_;
    unsigned const k_;
    GameResult game_result_;

    std::unique_ptr<ChessPieceState[]> board_;

    std::vector<MoveRecord> move_history_;
    std::unordered_set<MovePosition> legal_move_positions_;
};

} // namespace e8

#endif // BOARD_STATE_H
