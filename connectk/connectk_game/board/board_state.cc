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

#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_set>
#include <vector>

#include "connectk/connectk_game/board/board_state.h"

namespace e8 {

PlayerSide FlipPlayerSide(PlayerSide const side) {
    switch (side) {
    case PS_NONE:
        return PS_NONE;
    case PS_WHITE:
        return PS_BLACK;
    case PS_BLACK:
        return PS_WHITE;
    }
    assert(false);
}

ChessPieceState::ChessPieceState() : side(PlayerSide::PS_NONE) {}

ChessPieceState::ChessPieceState(PlayerSide const side) : side(side) {}

MovePosition::MovePosition(int const x, int const y) : x(x), y(y) {}

bool MovePosition::operator==(MovePosition const &other) const {
    return x == other.x && y == other.y;
}

MoveRecord::MoveRecord(MovePosition const &pos, PlayerSide const side) : pos(pos), side(side) {}

BoardState::BoardState(unsigned const width, unsigned const height, unsigned const k)
    : width_(width), height_(height), k_(k), game_result_(GameResult::GR_UNDETERMINED),
      board_(std::unique_ptr<ChessPieceState[]>(new ChessPieceState[width * height])),
      legal_move_positions_(2 * width * height) {
    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            legal_move_positions_.insert(MovePosition(x, y));
        }
    }
}

BoardState::BoardState(BoardState const &other)
    : BoardState(other.width_, other.height_, other.k_) {
    game_result_ = other.game_result_;

    for (unsigned i = 0; i < width_ * height_; ++i) {
        board_[i] = other.board_[i];
    }

    legal_move_positions_ = other.legal_move_positions_;
}

std::unordered_set<MovePosition> const &
BoardState::LegalMovePositions(PlayerSide const & /*side*/) const {
    return legal_move_positions_;
}

GameResult BoardState::MakeMove(MoveRecord const &move) {
    assert(game_result_ == GameResult::GR_UNDETERMINED);
    assert(this->ChessPieceStateAt(move.pos)->side == PlayerSide::PS_NONE);

    *this->ChessPieceStateAt(move.pos) = ChessPieceState(move.side);

    move_history_.push_back(move);
    legal_move_positions_.erase(move.pos);

    if (this->LeadToWinStateFrom(move)) {
        switch (move.side) {
        case PS_BLACK:
            game_result_ = GameResult::GR_BLACK_WIN;
            break;
        case PS_WHITE:
            game_result_ = GameResult::GR_WHITE_WIN;
            break;
        default:
            assert(false);
            break;
        }
    } else {
        if (this->LegalMovePositions(FlipPlayerSide(move.side)).empty()) {
            game_result_ = GameResult::GR_TIE;
        }
    }

    return this->CurrentGameResult();
}

std::optional<MoveRecord> BoardState::RetractMove() {
    if (move_history_.empty()) {
        return std::nullopt;
    }

    game_result_ = GameResult::GR_UNDETERMINED;

    MoveRecord const &the_move = move_history_.back();
    move_history_.pop_back();
    legal_move_positions_.insert(the_move.pos);

    *this->ChessPieceStateAt(the_move.pos) = ChessPieceState();

    return the_move;
}

GameResult BoardState::CurrentGameResult() const { return game_result_; }

ChessPieceState *BoardState::CurrentBoard() const { return board_.get(); }

unsigned BoardState::Width() const { return width_; }

unsigned BoardState::Height() const { return height_; }

ChessPieceState *BoardState::ChessPieceStateAt(MovePosition const &pos) {
    return &board_[pos.x + pos.y * width_];
}

bool BoardState::LeadToWinStateFrom(MoveRecord const &move) {
    // Counter clockwise 135 degrees.
    MovePosition pos(move.pos.x - 1, move.pos.y - 1);
    while (pos.x >= 0 && pos.y >= 0 && this->ChessPieceStateAt(pos)->side == move.side) {
        --pos.x;
        --pos.y;
    }
    if (move.pos.x - pos.x > static_cast<int>(k_)) {
        return true;
    }

    // Counter clockwise 90 degrees.
    pos.x = move.pos.x;
    pos.y = move.pos.y - 1;
    while (pos.y >= 0 && this->ChessPieceStateAt(pos)->side == move.side) {
        --pos.y;
    }
    if (move.pos.y - pos.y > static_cast<int>(k_)) {
        return true;
    }

    // Counter clockwise 45 degrees.
    pos.x = move.pos.x + 1;
    pos.y = move.pos.y - 1;
    while (pos.x < static_cast<int>(width_) && pos.y >= 0 &&
           this->ChessPieceStateAt(pos)->side == move.side) {
        ++pos.x;
        --pos.y;
    }
    if (move.pos.y - pos.y > static_cast<int>(k_)) {
        return true;
    }

    // Counter clockwise 180 degrees.
    pos.x = move.pos.x - 1;
    pos.y = move.pos.y;
    while (pos.x >= 0 && this->ChessPieceStateAt(pos)->side == move.side) {
        --pos.x;
    }
    if (move.pos.x - pos.x > static_cast<int>(k_)) {
        return true;
    }

    // Counter clockwise 0 degrees.
    pos.x = move.pos.x + 1;
    pos.y = move.pos.y;
    while (pos.x < static_cast<int>(width_) && this->ChessPieceStateAt(pos)->side == move.side) {
        ++pos.x;
    }
    if (pos.x - move.pos.x > static_cast<int>(k_)) {
        return true;
    }

    // Counter clockwise -135 degrees.
    pos.x = move.pos.x - 1;
    pos.y = move.pos.y + 1;
    while (pos.x >= 0 && pos.y < static_cast<int>(height_) &&
           this->ChessPieceStateAt(pos)->side == move.side) {
        --pos.x;
        ++pos.y;
    }
    if (move.pos.x - pos.x > static_cast<int>(k_)) {
        return true;
    }

    // Counter clockwise -90 degrees.
    pos.x = move.pos.x;
    pos.y = move.pos.y + 1;
    while (pos.y < static_cast<int>(height_) && this->ChessPieceStateAt(pos)->side == move.side) {
        ++pos.y;
    }
    if (pos.y - move.pos.y > static_cast<int>(k_)) {
        return true;
    }

    // Counter clockwise -45 degrees.
    pos.x = move.pos.x + 1;
    pos.y = move.pos.y + 1;
    while (pos.x < static_cast<int>(width_) && pos.y < static_cast<int>(height_) &&
           this->ChessPieceStateAt(pos)->side == move.side) {
        ++pos.x;
        ++pos.y;
    }
    if (pos.y - move.pos.y > static_cast<int>(k_)) {
        return true;
    }

    return false;
}

} // namespace e8
