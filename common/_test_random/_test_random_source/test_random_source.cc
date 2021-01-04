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
#include "common/unit_test_util/unit_test_util.h"

bool RandomSeedTest() {
    e8::RandomSource source1(/*seed=*/1);
    e8::RandomSource source2(/*seed=*/2);
    e8::RandomSource source3(/*seed=*/2);

    unsigned const kSeqLen = 100;

    std::vector<float> seq1(kSeqLen);
    std::vector<float> seq2(kSeqLen);
    std::vector<float> seq3(kSeqLen);

    for (unsigned i = 0; i < kSeqLen; ++i) {
        seq1[i] = source1.Draw();
    }

    for (unsigned i = 0; i < kSeqLen; ++i) {
        seq2[i] = source2.Draw();
    }

    for (unsigned i = 0; i < kSeqLen; ++i) {
        seq3[i] = source3.Draw();
    }

    TEST_CONDITION(seq1 != seq2);
    TEST_CONDITION(seq2 == seq3);

    return true;
}

bool RandomDrawRangeTest() {
    e8::RandomSource source;

    unsigned const kNumDraws = 100;

    for (unsigned i = 0; i < kNumDraws; ++i) {
        float draw = source.Draw();
        TEST_CONDITION(draw >= 0 && draw < 1);
    }

    return true;
}

int main() {
    e8::BeginTestSuite("random_source");
    e8::RunTest("RandomSeedTest", RandomSeedTest);
    e8::RunTest("RandomDrawRangeTest", RandomDrawRangeTest);
    e8::EndTestSuite();
    return 0;
}
