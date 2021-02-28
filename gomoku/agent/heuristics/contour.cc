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

#include "gomoku/agent/heuristics/contour.h"
#include "gomoku/game/board_state.h"

namespace e8 {
namespace {

template <int8_t order> bool IsAnyOrderContour(int8_t x, int8_t y, GomokuBoardState const &board) {
    if (*board.ChessPieceStateAt(MovePosition(x, y)) != StoneType::ST_NONE) {
        return false;
    }

    int8_t min_delta_x = std::max(0, x - order) - x;
    int8_t max_delta_x = std::min(board.Width() - 1, x + order) - x;

    int8_t min_delta_y = std::max(0, y - order) - y;
    int8_t max_delta_y = std::min(board.Height() - 1, y + order) - y;

    for (int8_t delta_y = min_delta_y; delta_y <= max_delta_y; ++delta_y) {
        int8_t shifted_y = y + delta_y;

        for (int8_t delta_x = min_delta_x; delta_x <= max_delta_x; ++delta_x) {
            int8_t shifted_x = x + delta_x;

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

} // namespace e8
