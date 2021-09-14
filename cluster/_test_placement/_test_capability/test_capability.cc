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
    a.set_cpu(1);
    a.set_ram(4);
    a.set_storage(32);
    a.set_coral(1);

    e8::WeightedCapabilities b;
    b.set_cpu(1);
    b.set_ram(-2);
    b.set_storage(0.5f);
    b.set_coral(0);

    for (auto type : e8::WeightedCapabilityTypes()) {
        float a_value = e8::GetCapabilityByType(type, a);
        float b_value = e8::GetCapabilityByType(type, b);
        e8::SetCapabilityByType(type, a_value + b_value, &a);
    }

    TEST_CONDITION(a.cpu() == 2.0f);
    TEST_CONDITION(a.ram() == 2.0f);
    TEST_CONDITION(a.storage() == 32.5f);
    TEST_CONDITION(a.coral() == 1.0f);

    return true;
}

int main() {
    e8::BeginTestSuite("placement_capability");
    e8::RunTest("AddCapabilitiesTest", AddCapabilitiesTest);
    e8::EndTestSuite();
    return 0;
}
