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

#include "cluster/placement/capability.h"
#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/machine.pb.h"

bool AddCapabilitiesTest() {
    e8::WeightedCapabilities a;
    *a.mutable_cpu() = e8::CapabilityFixedPointFromFloat(1.0f);
    *a.mutable_ram() = e8::CapabilityFixedPointFromFloat(4.0f);
    *a.mutable_storage() = e8::CapabilityFixedPointFromFloat(32.0f);
    *a.mutable_coral() = e8::CapabilityFixedPointFromFloat(1.0f);

    e8::WeightedCapabilities b;
    *b.mutable_cpu() = e8::CapabilityFixedPointFromFloat(1.0f);
    *b.mutable_ram() = e8::CapabilityFixedPointFromFloat(-2.0f);
    *b.mutable_storage() = e8::CapabilityFixedPointFromFloat(0.5f);
    *b.mutable_coral() = e8::CapabilityFixedPointFromFloat(0.0f);

    e8::WeightedCapabilities c = a + b;

    TEST_CONDITION(e8::ToFloat(c.cpu()) == 2.0f);
    TEST_CONDITION(e8::ToFloat(c.ram()) == 2.0f);
    TEST_CONDITION(e8::ToFloat(c.storage()) == 32.5f);
    TEST_CONDITION(e8::ToFloat(c.coral()) == 1.0f);

    return true;
}

bool SubtractCapabilitiesTest() {
    e8::WeightedCapabilities a;
    *a.mutable_cpu() = e8::CapabilityFixedPointFromFloat(1.0f);
    *a.mutable_ram() = e8::CapabilityFixedPointFromFloat(4.0f);
    *a.mutable_storage() = e8::CapabilityFixedPointFromFloat(32.0f);
    *a.mutable_coral() = e8::CapabilityFixedPointFromFloat(1.0f);

    e8::WeightedCapabilities b;
    *b.mutable_cpu() = e8::CapabilityFixedPointFromFloat(2.0f);
    *b.mutable_ram() = e8::CapabilityFixedPointFromFloat(2.0f);
    *b.mutable_storage() = e8::CapabilityFixedPointFromFloat(0.5f);
    *b.mutable_coral() = e8::CapabilityFixedPointFromFloat(0.0f);

    e8::WeightedCapabilities c = a - b;

    TEST_CONDITION(e8::ToFloat(c.cpu()) == -1.0f);
    TEST_CONDITION(e8::ToFloat(c.ram()) == 2.0f);
    TEST_CONDITION(e8::ToFloat(c.storage()) == 31.5f);
    TEST_CONDITION(e8::ToFloat(c.coral()) == 1.0f);

    return true;
}

bool NegateCapabilitiesTest() {
    e8::WeightedCapabilities a;
    *a.mutable_cpu() = e8::CapabilityFixedPointFromFloat(1.0f);
    *a.mutable_ram() = e8::CapabilityFixedPointFromFloat(4.0f);
    *a.mutable_storage() = e8::CapabilityFixedPointFromFloat(32.0f);
    *a.mutable_coral() = e8::CapabilityFixedPointFromFloat(1.0f);

    e8::WeightedCapabilities c = -a;

    TEST_CONDITION(e8::ToFloat(c.cpu()) == -1.0f);
    TEST_CONDITION(e8::ToFloat(c.ram()) == -4.0f);
    TEST_CONDITION(e8::ToFloat(c.storage()) == -32.0f);
    TEST_CONDITION(e8::ToFloat(c.coral()) == -1.0f);

    return true;
}

bool FixedPointConversionTest() {
    e8::WeightedCapabilities::FixedPoint fp = e8::CapabilityFixedPointFromFloat(10.5f);
    TEST_CONDITION(e8::ToFloat(fp) == 10.5f);
    return true;
}

bool AddFixedPointsTest() {
    e8::WeightedCapabilities::FixedPoint a = e8::CapabilityFixedPointFromFloat(10.25f);
    e8::WeightedCapabilities::FixedPoint b = e8::CapabilityFixedPointFromFloat(10.05f);

    e8::WeightedCapabilities::FixedPoint c = a + b;
    e8::WeightedCapabilities::FixedPoint d = b + a;

    TEST_CONDITION(c == d);
    TEST_CONDITION(std::abs(e8::ToFloat(c) - 20.3f) < 1e-3f);

    return true;
}

bool SubtractFixedPointsTest() {
    e8::WeightedCapabilities::FixedPoint a = e8::CapabilityFixedPointFromFloat(10.5f);
    e8::WeightedCapabilities::FixedPoint b = e8::CapabilityFixedPointFromFloat(10.05f);

    e8::WeightedCapabilities::FixedPoint c = a - b;
    e8::WeightedCapabilities::FixedPoint d = -(b - a);

    TEST_CONDITION(c == d);
    TEST_CONDITION(std::abs(e8::ToFloat(c) - 0.45f) < 1e-3f);

    return true;
}

bool NegateFixedPointsTest() {
    e8::WeightedCapabilities::FixedPoint a = e8::CapabilityFixedPointFromFloat(10.5f);
    e8::WeightedCapabilities::FixedPoint b = -a;

    TEST_CONDITION(-b == a);
    TEST_CONDITION(e8::ToFloat(b) == -10.5f);
    return true;
}

bool FixedPointComparisonTest() {
    e8::WeightedCapabilities::FixedPoint a = e8::CapabilityFixedPointFromFloat(-1.5f);
    e8::WeightedCapabilities::FixedPoint b = e8::CapabilityFixedPointFromFloat(2.5f);
    e8::WeightedCapabilities::FixedPoint c = e8::CapabilityFixedPointFromFloat(3.5f);
    e8::WeightedCapabilities::FixedPoint d = e8::CapabilityFixedPointFromFloat(3.5f);

    TEST_CONDITION(a == a);
    TEST_CONDITION(!(a != a));

    TEST_CONDITION(a < b);
    TEST_CONDITION(a <= b);
    TEST_CONDITION(a != b);
    TEST_CONDITION(b >= a);
    TEST_CONDITION(b > a);
    TEST_CONDITION(!(b < a));
    TEST_CONDITION(!(b <= a));
    TEST_CONDITION(!(a > b));
    TEST_CONDITION(!(a >= b));
    TEST_CONDITION(!(b == a));
    TEST_CONDITION(!(a == b));

    TEST_CONDITION(c == d);
    TEST_CONDITION(c <= d);
    TEST_CONDITION(c >= d);
    TEST_CONDITION(!(c < d));
    TEST_CONDITION(!(c > d));
    TEST_CONDITION(!(c != d));
    TEST_CONDITION(!(d != c));
    return true;
}

int main() {
    e8::BeginTestSuite("placement_capability");
    e8::RunTest("AddCapabilitiesTest", AddCapabilitiesTest);
    e8::RunTest("SubtractCapabilitiesTest", SubtractCapabilitiesTest);
    e8::RunTest("NegateCapabilitiesTest", NegateCapabilitiesTest);
    e8::RunTest("FixedPointConversionTest", FixedPointConversionTest);
    e8::RunTest("AddFixedPointsTest", AddFixedPointsTest);
    e8::RunTest("SubtractFixedPointsTest", SubtractFixedPointsTest);
    e8::RunTest("NegateFixedPointsTest", NegateFixedPointsTest);
    e8::RunTest("FixedPointComparisonTest", FixedPointComparisonTest);
    e8::EndTestSuite();
    return 0;
}
