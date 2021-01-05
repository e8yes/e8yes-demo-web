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

#ifndef RANDOM_DISTRIBUTION_H
#define RANDOM_DISTRIBUTION_H

#include <vector>

#include "common/random/random_source.h"

namespace e8 {

/**
 * @brief RandomPmf Returns a random PMF to a variable with the specified number of states.
 */
std::vector<float> RandomPmf(unsigned const num_states, RandomSource *random_source);

} // namespace e8

#endif // RANDOM_DISTRIBUTION_H
