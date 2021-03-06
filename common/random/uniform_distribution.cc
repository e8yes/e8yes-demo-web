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
#include <cmath>
#include <vector>

#include "common/random/random_source.h"
#include "common/random/uniform_distribution.h"

namespace e8 {
namespace {

template <typename ValueType>
std::vector<ValueType> DrawUniforms(ValueType const lower_bound, ValueType const upper_bound,
                                    unsigned const num_instances, RandomSource *random_source) {
    std::vector<ValueType> values(num_instances);

    for (unsigned i = 0; i < num_instances; ++i) {
        double val = random_source->Draw();
        assert(val >= 0 && val < 1);
        values[i] = lower_bound + std::floor(val * (upper_bound - lower_bound));
    }

    return values;
}

} // namespace

std::vector<int> DrawUniformInts(int const lower_bound, int const upper_bound,
                                 unsigned const num_instances, RandomSource *random_source) {
    return DrawUniforms(lower_bound, upper_bound, num_instances, random_source);
}

std::vector<float> DrawUniformFloats(float const lower_bound, float const upper_bound,
                                     unsigned const num_instances, RandomSource *random_source) {
    return DrawUniforms(lower_bound, upper_bound, num_instances, random_source);
}

} // namespace e8
