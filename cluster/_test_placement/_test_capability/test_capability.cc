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

int main() {
    e8::BeginTestSuite("placement_capability");
    e8::RunTest("AddCapabilitiesTest", AddCapabilitiesTest);
    e8::EndTestSuite();
    return 0;
}
