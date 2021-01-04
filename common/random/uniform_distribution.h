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

#ifndef UNIFORM_DISTRIBUTION_H
#define UNIFORM_DISTRIBUTION_H

#include "common/random/random_source.h"

namespace e8 {

/**
 * @brief DrawUniformInt Draw a uniformly distributed integer within the interval [lower_bound,
 * upper_bound).
 */
int DrawUniformInt(int const lower_bound, int const upper_bound, RandomSource *random_source);

/**
 * @brief DrawUniformFloat Draw a uniformly distributed floating point number within the interval
 * [lower_bound, upper_bound).
 */
float DrawUniformFloat(float const lower_bound, float const upper_bound,
                       RandomSource *random_source);

} // namespace e8

#endif // UNIFORM_DISTRIBUTION_H
