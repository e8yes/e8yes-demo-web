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
#include <cstdint>
#include <optional>
#include <unordered_set>
#include <vector>

#include "gomoku/agent/heuristics/contour.h"
#include "gomoku/game/board_state.h"

namespace e8 {
namespace {

template <int8_t order> bool IsAnyOrderContour(int8_t x, int8_t y, GomokuBoardState const &board) {
    if (*board.ChessPieceStateAt(MovePosition(x, y)) != StoneType::ST_NONE) {
        return false;
    }

    int8_t min_x = std::max(0, x - order);
    int8_t max_x = std::min(board.Width() - 1, x + order);

    int8_t min_y = std::max(0, y - order);
    int8_t max_y = std::min(board.Height() - 1, y + order);

    for (int8_t shifted_y = min_y; shifted_y <= max_y; ++shifted_y) {
        for (int8_t shifted_x = min_x; shifted_x <= max_x; ++shifted_x) {
            if (*board.ChessPieceStateAt(MovePosition(shifted_x, shifted_y)) !=
                StoneType::ST_NONE) {
                return true;
            }
        }
    }

    return false;
}

} // namespace

bool IsContour(int8_t x, int8_t y, GomokuBoardState const &board) {
    return IsAnyOrderContour</*order=*/1>(x, y, board);
}

bool IsDoubleContour(int8_t x, int8_t y, GomokuBoardState const &board) {
    return IsAnyOrderContour</*order=*/2>(x, y, board);
}

ContourBuilder::ContourBuilder(GomokuBoardState const &board, int8_t order)
    : blacklist_(board.Width() * board.Height()), width_(board.Width()), height_(board.Height()),
      order_(order) {
    for (GomokuActionRecord const &action_record : board.History()) {
        std::optional<MovePosition> pos = action_record.action.second.stone_pos;

        if (pos.has_value()) {
            this->AddStone(*pos);
        }
    }
}

void ContourBuilder::AddStone(MovePosition const &stone_pos) {
    contour_.erase(stone_pos);
    blacklist_[stone_pos.x + stone_pos.y * width_] = true;

    int8_t min_x = std::max(0, stone_pos.x - order_);
    int8_t max_x = std::min(width_ - 1, stone_pos.x + order_);

    int8_t min_y = std::max(0, stone_pos.y - order_);
    int8_t max_y = std::min(height_ - 1, stone_pos.y + order_);

    for (int8_t shifted_y = min_y; shifted_y <= max_y; ++shifted_y) {
        for (int8_t shifted_x = min_x; shifted_x <= max_x; ++shifted_x) {
            if (!blacklist_[shifted_x + shifted_y * width_]) {
                blacklist_[shifted_x + shifted_y * width_] = true;
                contour_.insert(MovePosition(shifted_x, shifted_y));
            }
        }
    }
}

std::unordered_set<MovePosition> const &ContourBuilder::Contour() const { return contour_; }

} // namespace e8
