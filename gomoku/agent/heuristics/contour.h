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

#ifndef CONTOUR_H
#define CONTOUR_H

#include <cstdint>
#include <unordered_set>
#include <vector>

#include "gomoku/game/board_state.h"

namespace e8 {

/**
 * @brief IsContour Checks if the specified position (x, y) is a contour in the baord. A contour is
 * the adjacent 8 empty cells surrounding a stone, if there are any.
 */
bool IsContour(int8_t x, int8_t y, GomokuBoardState const &board);

/**
 * @brief IsDoubleContour Checks if the specified position (x, y) is a double contour in the board.
 * A double contour is the coutour of the contour when it is filled with stones.
 */
bool IsDoubleContour(int8_t x, int8_t y, GomokuBoardState const &board);

/**
 * @brief The ContourBuilder class A data structure that helps construct arbitrary orders of contour
 * efficient.
 */
class ContourBuilder {
  public:
    /**
     * @brief ContourBuilder Construct the initial contour from the specified board state.
     *
     * @param order See the definition of contour order in the above functions.
     */
    ContourBuilder(GomokuBoardState const &board, int8_t order);

    /**
     * @brief AddStone Incrementally updates the contour after a new stone is added to the board.
     */
    void AddStone(MovePosition const &stone_pos);

    /**
     * @brief Contour Returns the current contour.
     */
    std::unordered_set<MovePosition> const &Contour() const;

  private:
    std::unordered_set<MovePosition> contour_;
    std::vector<bool> blacklist_;
    int8_t width_;
    int8_t height_;
    int8_t order_;
};

} // namespace e8

#endif // CONTOUR_H
