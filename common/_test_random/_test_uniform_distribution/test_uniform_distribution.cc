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

#include <cmath>
#include <unordered_map>
#include <vector>

#include "common/random/uniform_distribution.h"
#include "common/unit_test_util/unit_test_util.h"

bool UniformFloatDistributionRelativeEntropyTest() {
    e8::RandomSource source(/*seed=*/1);
    std::vector<int> rns = e8::DrawUniformInts(/*lower_bound=*/-1, /*upper_bound=*/2,
                                               /*num_instances=*/1000, &source);

    TEST_CONDITION(rns.size() == 1000);

    std::unordered_map<int, int> empircal_pmf;
    for (int const rn : rns) {
        ++empircal_pmf[rn];
    }

    float relative_entropy = 0.0f;
    for (int i = -1; i < 2; ++i) {
        float q_i = 1.0f / 3.0f;

        auto freq_it = empircal_pmf.find(i);
        TEST_CONDITION(freq_it != empircal_pmf.end());

        float p_i = freq_it->second / 1000.0f;
        relative_entropy += p_i * std::log2(p_i / q_i);
    }

    TEST_CONDITION(relative_entropy < 1e-2f);

    return true;
}

int main() {
    e8::BeginTestSuite("uniform_distribution");
    e8::RunTest("UniformFloatDistributionRelativeEntropyTest",
                UniformFloatDistributionRelativeEntropyTest);
    e8::EndTestSuite();
    return 0;
}
