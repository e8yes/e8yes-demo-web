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

#include <vector>

#include "common/random/random_source.h"

namespace e8 {

std::vector<float> RandomPmf(unsigned const num_states, RandomSource *random_source) {
    std::vector<float> pmf(num_states);

    float normalization_factor = 0.0f;

    for (unsigned i = 0; i < num_states; ++i) {
        float draw = random_source->Draw();
        normalization_factor += draw;
        pmf[i] = draw;
    }

    for (unsigned i = 0; i < num_states; ++i) {
        pmf[i] /= normalization_factor;
    }

    return pmf;
}

} // namespace e8
