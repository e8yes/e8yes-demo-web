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
#include <memory>
#include <optional>
#include <vector>

#include "connectk/connectk_game/board/board_state.h"

namespace e8 {

ChessPieceState::ChessPieceState() : side(PlayerSide::PS_NONE) {}

ChessPieceState::ChessPieceState(PlayerSide const side) : side(side) {}

MoveRecord::MoveRecord(unsigned const x, unsigned const y, PlayerSide const side)
    : x(x), y(y), side(side) {}

BoardState::BoardState(unsigned const width, unsigned const height, unsigned const k)
    : width_(width), height_(height), k_(k), game_result_(GameResult::GR_UNDETERMINED),
      board_(std::unique_ptr<ChessPieceState[]>(new ChessPieceState[width * height])) {}

BoardState::BoardState(BoardState const &other)
    : BoardState(other.width_, other.height_, other.k_) {
    game_result_ = other.game_result_;

    for (unsigned i = 0; i < width_ * height_; ++i) {
        board_[i] = other.board_[i];
    }
}

GameResult BoardState::MakeMove(MoveRecord const &move) {
    assert(game_result_ == GameResult::GR_UNDETERMINED);

    *this->ChessPieceStateAt(move.x, move.y) = ChessPieceState(move.side);

    move_history_.push_back(move);

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

    *this->ChessPieceStateAt(the_move.x, the_move.y) = ChessPieceState();

    return the_move;
}

GameResult BoardState::CurrentGameResult() const { return game_result_; }

ChessPieceState *BoardState::CurrentBoard() const { return board_.get(); }

unsigned BoardState::Width() const { return width_; }

unsigned BoardState::Height() const { return height_; }

ChessPieceState *BoardState::ChessPieceStateAt(unsigned x, unsigned y) {
    return &board_[x + y * width_];
}

bool BoardState::LeadToWinStateFrom(MoveRecord const &move) {
    // Counter clockwise 135 degrees.
    int x = static_cast<int>(move.x) - 1;
    int y = static_cast<int>(move.y) - 1;
    while (x >= 0 && y >= 0 && this->ChessPieceStateAt(x, y)->side == move.side) {
        --x;
        --y;
    }
    if (move.x - x > k_) {
        return true;
    }

    // Counter clockwise 90 degrees.
    x = static_cast<int>(move.x);
    y = static_cast<int>(move.y) - 1;
    while (y >= 0 && this->ChessPieceStateAt(x, y)->side == move.side) {
        --y;
    }
    if (move.y - y > k_) {
        return true;
    }

    // Counter clockwise 45 degrees.
    x = static_cast<int>(move.x) + 1;
    y = static_cast<int>(move.y) - 1;
    while (x < static_cast<int>(width_) && y >= 0 &&
           this->ChessPieceStateAt(x, y)->side == move.side) {
        ++x;
        --y;
    }
    if (move.y - y > k_) {
        return true;
    }

    // Counter clockwise 180 degrees.
    x = static_cast<int>(move.x) - 1;
    y = static_cast<int>(move.y);
    while (x >= 0 && this->ChessPieceStateAt(x, y)->side == move.side) {
        --x;
    }
    if (move.x - x > k_) {
        return true;
    }

    // Counter clockwise 0 degrees.
    x = static_cast<int>(move.x) + 1;
    y = static_cast<int>(move.y);
    while (x < static_cast<int>(width_) && this->ChessPieceStateAt(x, y)->side == move.side) {
        ++x;
    }
    if (x - move.x > k_) {
        return true;
    }

    // Counter clockwise -135 degrees.
    x = static_cast<int>(move.x) - 1;
    y = static_cast<int>(move.y) + 1;
    while (x >= 0 && y < static_cast<int>(height_) &&
           this->ChessPieceStateAt(x, y)->side == move.side) {
        --x;
        ++y;
    }
    if (move.x - x > k_) {
        return true;
    }

    // Counter clockwise -90 degrees.
    x = static_cast<int>(move.x);
    y = static_cast<int>(move.y) + 1;
    while (y < static_cast<int>(height_) && this->ChessPieceStateAt(x, y)->side == move.side) {
        ++y;
    }
    if (y - move.y > k_) {
        return true;
    }

    // Counter clockwise -45 degrees.
    x = static_cast<int>(move.x) + 1;
    y = static_cast<int>(move.y) + 1;
    while (x < static_cast<int>(width_) && y < static_cast<int>(height_) &&
           this->ChessPieceStateAt(x, y)->side == move.side) {
        ++x;
        ++y;
    }
    if (y - move.y > k_) {
        return true;
    }

    return false;
}

} // namespace e8
