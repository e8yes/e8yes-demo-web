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

#ifndef MCT_SEARCH_H
#define MCT_SEARCH_H

#include <unordered_map>

#include "gomoku/gomoku_agent/heuristics/evaluator.h"
#include "gomoku/gomoku_game/board_state.h"

namespace e8 {

/**
 * @brief MctSearchFrom Calculate a stochastically optimal policy for the specified board state with
 * the help of the heuristics.
 *
 * @param state Board state to search from.
 * @param evaluator Heuristics to help guide the tree search.
 * @param print_stats Whether the print out the final MCTS statistics after running the simulations.
 * @return A stochastic policy.
 */
std::unordered_map<GomokuActionId, float>
MctSearchFrom(GomokuBoardState state, GomokuEvaluatorInterface *evaluator, bool const print_stats);

/**
 * @brief BestAction Returns the ID of the most visited action.
 */
GomokuActionId BestAction(std::unordered_map<GomokuActionId, float> const &optimal_policy);

} // namespace e8

#endif // MCT_SEARCH_H
