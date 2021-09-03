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

bool CapabilityOneLayerTest() {
    // There are 4 machines.
    e8::WeightedCapabilities machine1_capabilities;
    machine1_capabilities.set_cpu(4.0f);
    machine1_capabilities.set_ram(1.0f);
    machine1_capabilities.set_storage(32.0f);

    e8::WeightedCapabilities machine2_capabilities;
    machine2_capabilities.set_cpu(4.0f);
    machine2_capabilities.set_ram(1.0f);
    machine2_capabilities.set_storage(16.0f);

    e8::WeightedCapabilities machine3_capabilities;
    machine3_capabilities.set_cpu(8.0f);
    machine3_capabilities.set_ram(4.0f);
    machine3_capabilities.set_storage(16.0f);

    e8::WeightedCapabilities machine4_capabilities;
    machine4_capabilities.set_cpu(8.0f);
    machine4_capabilities.set_ram(4.0f);
    machine4_capabilities.set_storage(32.0f);
    machine4_capabilities.set_coral(1.0f);

    // 4 machines organizes under the root.
    e8::ClusterCapability capability(/*root=*/"root");
    bool added = capability.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine1",
                                        machine1_capabilities);
    TEST_CONDITION(added == true);
    added = capability.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine2",
                                   machine2_capabilities);
    TEST_CONDITION(added == true);
    added = capability.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine3",
                                   machine3_capabilities);
    TEST_CONDITION(added == true);
    added = capability.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine4",
                                   machine4_capabilities);
    TEST_CONDITION(added == true);

    added = capability.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine1",
                                   machine1_capabilities);
    TEST_CONDITION(added == false);
    added = capability.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine2",
                                   machine2_capabilities);
    TEST_CONDITION(added == false);
    added = capability.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine3",
                                   machine3_capabilities);
    TEST_CONDITION(added == false);
    added = capability.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine4",
                                   machine4_capabilities);
    TEST_CONDITION(added == false);

    // Checks the last layer (external).
    TEST_CONDITION(capability.Capability("machine1").SerializeAsString() ==
                   machine1_capabilities.SerializeAsString());
    TEST_CONDITION(capability.Capability("machine2").SerializeAsString() ==
                   machine2_capabilities.SerializeAsString());
    TEST_CONDITION(capability.Capability("machine3").SerializeAsString() ==
                   machine3_capabilities.SerializeAsString());
    TEST_CONDITION(capability.Capability("machine4").SerializeAsString() ==
                   machine4_capabilities.SerializeAsString());

    // Checks the root layer.
    e8::WeightedCapabilities root_capability = capability.Capability("root");
    TEST_CONDITION(root_capability.cpu() == 24.0f);
    TEST_CONDITION(root_capability.ram() == 10.0f);
    TEST_CONDITION(root_capability.storage() == 96.0f);
    TEST_CONDITION(root_capability.coral() == 1.0f);

    // Removes machine4 and re-checks the root layer.
    bool removed = capability.Remove(/*node_label=*/"machine4");
    TEST_CONDITION(removed == true);
    removed = capability.Remove(/*node_label=*/"machine4");
    TEST_CONDITION(removed == false);

    root_capability = capability.Capability("root");
    TEST_CONDITION(root_capability.cpu() == 16.0f);
    TEST_CONDITION(root_capability.ram() == 6.0f);
    TEST_CONDITION(root_capability.storage() == 64.0f);
    TEST_CONDITION(root_capability.coral() == 0.0f);

    return true;
}

bool CapabilityMultiLayerTest() {
    // There are 4 machines.
    e8::WeightedCapabilities machine1_capabilities;
    machine1_capabilities.set_cpu(4.0f);
    machine1_capabilities.set_ram(1.0f);
    machine1_capabilities.set_storage(32.0f);

    e8::WeightedCapabilities machine2_capabilities;
    machine2_capabilities.set_cpu(4.0f);
    machine2_capabilities.set_ram(1.0f);
    machine2_capabilities.set_storage(16.0f);

    e8::WeightedCapabilities machine3_capabilities;
    machine3_capabilities.set_cpu(8.0f);
    machine3_capabilities.set_ram(4.0f);
    machine3_capabilities.set_storage(16.0f);

    e8::WeightedCapabilities machine4_capabilities;
    machine4_capabilities.set_cpu(8.0f);
    machine4_capabilities.set_ram(4.0f);
    machine4_capabilities.set_storage(32.0f);
    machine4_capabilities.set_coral(1.0f);

    /*           root
     *     |              |
     *    row1           row2
     *   |    |         |    |
     *  m1    m2       m3    m4
     */
    e8::ClusterCapability capability(/*root=*/"root");

    // Second layer (internal).
    bool added = capability.AddBucket(/*parent_label=*/"root", /*node_label=*/"row1");
    TEST_CONDITION(added == true);
    added = capability.AddBucket(/*parent_label=*/"root", /*node_label=*/"row2");
    TEST_CONDITION(added == true);

    added = capability.AddBucket(/*parent_label=*/"root", /*node_label=*/"row1");
    TEST_CONDITION(added == false);
    added = capability.AddBucket(/*parent_label=*/"root", /*node_label=*/"row2");
    TEST_CONDITION(added == false);

    // Third layer (external).
    added = capability.AddMachine(/*parent_label=*/"row1", /*node_label=*/"machine1",
                                   machine1_capabilities);
    TEST_CONDITION(added == true);
    added = capability.AddMachine(/*parent_label=*/"row1", /*node_label=*/"machine2",
                                   machine2_capabilities);
    TEST_CONDITION(added == true);
    added = capability.AddMachine(/*parent_label=*/"row2", /*node_label=*/"machine3",
                                   machine3_capabilities);
    TEST_CONDITION(added == true);
    added = capability.AddMachine(/*parent_label=*/"row2", /*node_label=*/"machine4",
                                   machine4_capabilities);
    TEST_CONDITION(added == true);

    added = capability.AddMachine(/*parent_label=*/"row1", /*node_label=*/"machine1",
                                   machine1_capabilities);
    TEST_CONDITION(added == false);
    added = capability.AddMachine(/*parent_label=*/"row1", /*node_label=*/"machine2",
                                   machine2_capabilities);
    TEST_CONDITION(added == false);
    added = capability.AddMachine(/*parent_label=*/"row2", /*node_label=*/"machine3",
                                   machine3_capabilities);
    TEST_CONDITION(added == false);
    added = capability.AddMachine(/*parent_label=*/"row2", /*node_label=*/"machine4",
                                   machine4_capabilities);
    TEST_CONDITION(added == false);

    // Checks the last layer (external).
    TEST_CONDITION(capability.Capability("machine1").SerializeAsString() ==
                   machine1_capabilities.SerializeAsString());
    TEST_CONDITION(capability.Capability("machine2").SerializeAsString() ==
                   machine2_capabilities.SerializeAsString());
    TEST_CONDITION(capability.Capability("machine3").SerializeAsString() ==
                   machine3_capabilities.SerializeAsString());
    TEST_CONDITION(capability.Capability("machine4").SerializeAsString() ==
                   machine4_capabilities.SerializeAsString());

    // Checks the root and row layers (internal).
    e8::WeightedCapabilities root_capability = capability.Capability("root");
    TEST_CONDITION(root_capability.cpu() == 24.0f);
    TEST_CONDITION(root_capability.ram() == 10.0f);
    TEST_CONDITION(root_capability.storage() == 96.0f);
    TEST_CONDITION(root_capability.coral() == 1.0f);

    e8::WeightedCapabilities row1_capability = capability.Capability("row1");
    TEST_CONDITION(row1_capability.cpu() == 8.0f);
    TEST_CONDITION(row1_capability.ram() == 2.0f);
    TEST_CONDITION(row1_capability.storage() == 48.0f);
    TEST_CONDITION(row1_capability.coral() == 0.0f);

    e8::WeightedCapabilities row2_capability = capability.Capability("row2");
    TEST_CONDITION(row2_capability.cpu() == 16.0f);
    TEST_CONDITION(row2_capability.ram() == 8.0f);
    TEST_CONDITION(row2_capability.storage() == 48.0f);
    TEST_CONDITION(row2_capability.coral() == 1.0f);

    // Removes machine4 and re-checks the root and row layers.
    bool removed = capability.Remove(/*node_label=*/"machine4");
    TEST_CONDITION(removed == true);
    removed = capability.Remove(/*node_label=*/"machine4");
    TEST_CONDITION(removed == false);

    root_capability = capability.Capability("root");
    TEST_CONDITION(root_capability.cpu() == 16.0f);
    TEST_CONDITION(root_capability.ram() == 6.0f);
    TEST_CONDITION(root_capability.storage() == 64.0f);
    TEST_CONDITION(root_capability.coral() == 0.0f);

    row1_capability = capability.Capability("row1");
    TEST_CONDITION(row1_capability.cpu() == 8.0f);
    TEST_CONDITION(row1_capability.ram() == 2.0f);
    TEST_CONDITION(row1_capability.storage() == 48.0f);
    TEST_CONDITION(row1_capability.coral() == 0.0f);

    row2_capability = capability.Capability("row2");
    TEST_CONDITION(row2_capability.cpu() == 8.0f);
    TEST_CONDITION(row2_capability.ram() == 4.0f);
    TEST_CONDITION(row2_capability.storage() == 16.0f);
    TEST_CONDITION(row2_capability.coral() == 0.0f);

    // Removes row2 and re-checks the root and row layers.
    removed = capability.Remove(/*node_label=*/"row2");
    TEST_CONDITION(removed == true);
    removed = capability.Remove(/*node_label=*/"row2");
    TEST_CONDITION(removed == false);

    root_capability = capability.Capability("root");
    TEST_CONDITION(root_capability.cpu() == 8.0f);
    TEST_CONDITION(root_capability.ram() == 2.0f);
    TEST_CONDITION(root_capability.storage() == 48.0f);
    TEST_CONDITION(root_capability.coral() == 0.0f);

    row1_capability = capability.Capability("row1");
    TEST_CONDITION(row1_capability.cpu() == 8.0f);
    TEST_CONDITION(row1_capability.ram() == 2.0f);
    TEST_CONDITION(row1_capability.storage() == 48.0f);
    TEST_CONDITION(row1_capability.coral() == 0.0f);

    return true;
}

int main() {
    e8::BeginTestSuite("placement_capability");
    e8::RunTest("CapabilityOneLayerTest", CapabilityOneLayerTest);
    e8::RunTest("CapabilityMultiLayerTest", CapabilityMultiLayerTest);
    e8::EndTestSuite();
    return 0;
}
