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

#include <memory>
#include <vector>

#include "cluster/placement/bucket.h"
#include "cluster/placement/bucket_uniform.h"
#include "cluster/placement/capability.h"
#include "cluster/placement/hierarchy.h"
#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/bucket.pb.h"
#include "proto_cc/machine.pb.h"

bool OneLayerTest() {
    e8::ClusterHierarchy hierarchy;

    // There are 4 machines.
    e8::Machine machine1;
    machine1.set_address("192.168.1.1");
    *machine1.mutable_capabilities()->mutable_cpu() = e8::CapabilityFixedPointFromFloat(4.0f);
    *machine1.mutable_capabilities()->mutable_ram() = e8::CapabilityFixedPointFromFloat(1.0f);
    *machine1.mutable_capabilities()->mutable_storage() = e8::CapabilityFixedPointFromFloat(32.0f);

    e8::Machine machine2;
    machine1.set_address("192.168.1.2");
    *machine2.mutable_capabilities()->mutable_cpu() = e8::CapabilityFixedPointFromFloat(4.0f);
    *machine2.mutable_capabilities()->mutable_ram() = e8::CapabilityFixedPointFromFloat(1.0f);
    *machine2.mutable_capabilities()->mutable_storage() = e8::CapabilityFixedPointFromFloat(16.0f);

    e8::Machine machine3;
    machine1.set_address("192.168.1.3");
    *machine3.mutable_capabilities()->mutable_cpu() = e8::CapabilityFixedPointFromFloat(8.0f);
    *machine3.mutable_capabilities()->mutable_ram() = e8::CapabilityFixedPointFromFloat(4.0f);
    *machine3.mutable_capabilities()->mutable_storage() = e8::CapabilityFixedPointFromFloat(16.0f);

    e8::Machine machine4;
    machine1.set_address("192.168.1.4");
    *machine4.mutable_capabilities()->mutable_cpu() = e8::CapabilityFixedPointFromFloat(8.0f);
    *machine4.mutable_capabilities()->mutable_ram() = e8::CapabilityFixedPointFromFloat(4.0f);
    *machine4.mutable_capabilities()->mutable_storage() = e8::CapabilityFixedPointFromFloat(32.0f);
    *machine4.mutable_capabilities()->mutable_coral() = e8::CapabilityFixedPointFromFloat(1.0f);

    // 4 machines organized under the root.
    e8::UniformBucketData root_bucket_proto;
    root_bucket_proto.set_prime(13);
    *root_bucket_proto.add_child_labels() = "machine1";
    *root_bucket_proto.add_child_labels() = "machine2";
    *root_bucket_proto.add_child_labels() = "machine3";
    *root_bucket_proto.add_child_labels() = "machine4";

    bool added = hierarchy.AddRoot(std::make_unique<e8::UniformBucket>(root_bucket_proto));
    TEST_CONDITION(added == true);

    added = hierarchy.AddRoot(std::make_unique<e8::UniformBucket>(root_bucket_proto));
    TEST_CONDITION(added == false);

    added = hierarchy.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine1", machine1);
    TEST_CONDITION(added == true);
    added = hierarchy.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine2", machine2);
    TEST_CONDITION(added == true);
    added = hierarchy.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine3", machine3);
    TEST_CONDITION(added == true);
    added = hierarchy.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine4", machine4);
    TEST_CONDITION(added == true);

    added = hierarchy.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine1", machine1);
    TEST_CONDITION(added == false);
    added = hierarchy.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine2", machine2);
    TEST_CONDITION(added == false);
    added = hierarchy.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine3", machine3);
    TEST_CONDITION(added == false);
    added = hierarchy.AddMachine(/*parent_label=*/"root", /*node_label=*/"machine4", machine4);
    TEST_CONDITION(added == false);

    // Reads hierarchy back to check if the information was correctly recorded.
    TEST_CONDITION(hierarchy.NodeCount() == 5);

    e8::ClusterHierarchy::BucketOrMachine const *root = hierarchy.Root();
    TEST_CONDITION(root != nullptr);
    TEST_CONDITION(root->hierarchy == e8::ClusterTreeNode::ROOT);
    TEST_CONDITION(root->bucket != nullptr);
    TEST_CONDITION(!root->machine.has_value());
    TEST_CONDITION(root->parent_label == "root");

    std::vector<e8::BucketInterface::Child> children = root->bucket->Children();
    TEST_CONDITION(children.size() == 4);

    TEST_CONDITION(children[0].label == "machine1");
    TEST_CONDITION(children[1].label == "machine2");
    TEST_CONDITION(children[2].label == "machine3");
    TEST_CONDITION(children[3].label == "machine4");

    TEST_CONDITION(children[0].capabilities.DebugString() == machine1.capabilities().DebugString());
    TEST_CONDITION(children[1].capabilities.DebugString() == machine2.capabilities().DebugString());
    TEST_CONDITION(children[2].capabilities.DebugString() == machine3.capabilities().DebugString());
    TEST_CONDITION(children[3].capabilities.DebugString() == machine4.capabilities().DebugString());

    e8::ClusterHierarchy::BucketOrMachine const *machine1_node = hierarchy.Node("machine1");
    e8::ClusterHierarchy::BucketOrMachine const *machine2_node = hierarchy.Node("machine2");
    e8::ClusterHierarchy::BucketOrMachine const *machine3_node = hierarchy.Node("machine3");
    e8::ClusterHierarchy::BucketOrMachine const *machine4_node = hierarchy.Node("machine4");

    TEST_CONDITION(machine1_node != nullptr);
    TEST_CONDITION(machine2_node != nullptr);
    TEST_CONDITION(machine3_node != nullptr);
    TEST_CONDITION(machine4_node != nullptr);

    TEST_CONDITION(machine1_node->machine.has_value());
    TEST_CONDITION(machine2_node->machine.has_value());
    TEST_CONDITION(machine3_node->machine.has_value());
    TEST_CONDITION(machine4_node->machine.has_value());

    TEST_CONDITION(machine1_node->machine->DebugString() == machine1.DebugString());
    TEST_CONDITION(machine2_node->machine->DebugString() == machine2.DebugString());
    TEST_CONDITION(machine3_node->machine->DebugString() == machine3.DebugString());
    TEST_CONDITION(machine4_node->machine->DebugString() == machine4.DebugString());

    TEST_CONDITION(machine1_node->hierarchy == e8::ClusterTreeNode::MACHINE);
    TEST_CONDITION(machine2_node->hierarchy == e8::ClusterTreeNode::MACHINE);
    TEST_CONDITION(machine3_node->hierarchy == e8::ClusterTreeNode::MACHINE);
    TEST_CONDITION(machine4_node->hierarchy == e8::ClusterTreeNode::MACHINE);

    TEST_CONDITION(machine1_node->parent_label == "root");
    TEST_CONDITION(machine2_node->parent_label == "root");
    TEST_CONDITION(machine3_node->parent_label == "root");
    TEST_CONDITION(machine4_node->parent_label == "root");

    // Checks removal.
    bool removed = hierarchy.Remove("machine4");
    TEST_CONDITION(removed == true);
    removed = hierarchy.Remove("machine4");
    TEST_CONDITION(removed == false);

    TEST_CONDITION(hierarchy.NodeCount() == 4);

    root = hierarchy.Root();

    children = root->bucket->Children();
    TEST_CONDITION(children.size() == 3);

    TEST_CONDITION(children[0].label == "machine1");
    TEST_CONDITION(children[1].label == "machine2");
    TEST_CONDITION(children[2].label == "machine3");

    return true;
}

bool MultiLayerTest() {
    e8::ClusterHierarchy hierarchy;

    // There are 4 machines.
    e8::Machine machine1;
    machine1.set_address("192.168.1.1");
    *machine1.mutable_capabilities()->mutable_cpu() = e8::CapabilityFixedPointFromFloat(4.0f);
    *machine1.mutable_capabilities()->mutable_ram() = e8::CapabilityFixedPointFromFloat(1.0f);
    *machine1.mutable_capabilities()->mutable_storage() = e8::CapabilityFixedPointFromFloat(32.0f);

    e8::Machine machine2;
    machine1.set_address("192.168.1.2");
    *machine2.mutable_capabilities()->mutable_cpu() = e8::CapabilityFixedPointFromFloat(4.0f);
    *machine2.mutable_capabilities()->mutable_ram() = e8::CapabilityFixedPointFromFloat(1.0f);
    *machine2.mutable_capabilities()->mutable_storage() = e8::CapabilityFixedPointFromFloat(16.0f);

    e8::Machine machine3;
    machine1.set_address("192.168.1.3");
    *machine3.mutable_capabilities()->mutable_cpu() = e8::CapabilityFixedPointFromFloat(8.0f);
    *machine3.mutable_capabilities()->mutable_ram() = e8::CapabilityFixedPointFromFloat(4.0f);
    *machine3.mutable_capabilities()->mutable_storage() = e8::CapabilityFixedPointFromFloat(16.0f);

    e8::Machine machine4;
    machine1.set_address("192.168.1.4");
    *machine4.mutable_capabilities()->mutable_cpu() = e8::CapabilityFixedPointFromFloat(8.0f);
    *machine4.mutable_capabilities()->mutable_ram() = e8::CapabilityFixedPointFromFloat(4.0f);
    *machine4.mutable_capabilities()->mutable_storage() = e8::CapabilityFixedPointFromFloat(32.0f);
    *machine4.mutable_capabilities()->mutable_coral() = e8::CapabilityFixedPointFromFloat(1.0f);

    /*           root
     *     |              |
     *    row1           row2
     *   |    |         |    |
     *  m1    m2       m3    m4
     */
    // 4 machines organized under the root.
    e8::UniformBucketData root_bucket_proto;
    root_bucket_proto.set_prime(13);
    *root_bucket_proto.add_child_labels() = "row1";
    *root_bucket_proto.add_child_labels() = "row2";

    e8::UniformBucketData row1_bucket_proto;
    row1_bucket_proto.set_prime(13);
    *row1_bucket_proto.add_child_labels() = "machine1";
    *row1_bucket_proto.add_child_labels() = "machine2";

    e8::UniformBucketData row2_bucket_proto;
    row2_bucket_proto.set_prime(13);
    *row2_bucket_proto.add_child_labels() = "machine3";
    *row2_bucket_proto.add_child_labels() = "machine4";

    bool added = hierarchy.AddRoot(std::make_unique<e8::UniformBucket>(root_bucket_proto));
    TEST_CONDITION(added == true);

    added = hierarchy.AddBucket(/*parent_label=*/"root", /*node_label=*/"row1",
                                /*hierarchy=*/e8::ClusterTreeNode::ROW,
                                std::make_unique<e8::UniformBucket>(row1_bucket_proto));
    TEST_CONDITION(added == true);
    added = hierarchy.AddBucket(/*parent_label=*/"root", /*node_label=*/"row2",
                                /*hierarchy=*/e8::ClusterTreeNode::ROW,
                                std::make_unique<e8::UniformBucket>(row2_bucket_proto));
    TEST_CONDITION(added == true);

    added = hierarchy.AddMachine(/*parent_label=*/"row1", /*node_label=*/"machine1", machine1);
    TEST_CONDITION(added == true);
    added = hierarchy.AddMachine(/*parent_label=*/"row1", /*node_label=*/"machine2", machine2);
    TEST_CONDITION(added == true);
    added = hierarchy.AddMachine(/*parent_label=*/"row2", /*node_label=*/"machine3", machine3);
    TEST_CONDITION(added == true);
    added = hierarchy.AddMachine(/*parent_label=*/"row2", /*node_label=*/"machine4", machine4);
    TEST_CONDITION(added == true);

    // Double adding.
    added = hierarchy.AddRoot(std::make_unique<e8::UniformBucket>(root_bucket_proto));
    TEST_CONDITION(added == false);

    added = hierarchy.AddBucket(/*parent_label=*/"root", /*node_label=*/"row1",
                                /*hierarchy=*/e8::ClusterTreeNode::ROW,
                                std::make_unique<e8::UniformBucket>(row1_bucket_proto));
    TEST_CONDITION(added == false);
    added = hierarchy.AddBucket(/*parent_label=*/"root", /*node_label=*/"row2",
                                /*hierarchy=*/e8::ClusterTreeNode::ROW,
                                std::make_unique<e8::UniformBucket>(row2_bucket_proto));
    TEST_CONDITION(added == false);

    added = hierarchy.AddMachine(/*parent_label=*/"row1", /*node_label=*/"machine1", machine1);
    TEST_CONDITION(added == false);
    added = hierarchy.AddMachine(/*parent_label=*/"row1", /*node_label=*/"machine2", machine2);
    TEST_CONDITION(added == false);
    added = hierarchy.AddMachine(/*parent_label=*/"row2", /*node_label=*/"machine3", machine3);
    TEST_CONDITION(added == false);
    added = hierarchy.AddMachine(/*parent_label=*/"row2", /*node_label=*/"machine4", machine4);
    TEST_CONDITION(added == false);

    // Reads hierarchy back to check if the information was correctly recorded.
    TEST_CONDITION(hierarchy.NodeCount() == 7);

    e8::ClusterHierarchy::BucketOrMachine const *root = hierarchy.Root();
    TEST_CONDITION(root != nullptr);
    TEST_CONDITION(root->hierarchy == e8::ClusterTreeNode::ROOT);
    TEST_CONDITION(root->bucket != nullptr);
    TEST_CONDITION(!root->machine.has_value());
    TEST_CONDITION(root->parent_label == "root");

    std::vector<e8::BucketInterface::Child> children = root->bucket->Children();
    TEST_CONDITION(children.size() == 2);

    TEST_CONDITION(children[0].label == "row1");
    TEST_CONDITION(children[1].label == "row2");

    TEST_CONDITION(e8::ToFloat(children[0].capabilities.cpu()) == 8.0f);
    TEST_CONDITION(e8::ToFloat(children[0].capabilities.ram()) == 2.0f);
    TEST_CONDITION(e8::ToFloat(children[0].capabilities.storage()) == 48.0f);

    TEST_CONDITION(e8::ToFloat(children[1].capabilities.cpu()) == 16.0f);
    TEST_CONDITION(e8::ToFloat(children[1].capabilities.ram()) == 8.0f);
    TEST_CONDITION(e8::ToFloat(children[1].capabilities.storage()) == 48.0f);
    TEST_CONDITION(e8::ToFloat(children[1].capabilities.coral()) == 1.0f);

    // Checks removal.
    bool removed = hierarchy.Remove("row1");
    TEST_CONDITION(removed == true);
    removed = hierarchy.Remove("row1");
    TEST_CONDITION(removed == false);

    TEST_CONDITION(hierarchy.NodeCount() == 4);

    root = hierarchy.Root();

    children = root->bucket->Children();
    TEST_CONDITION(children.size() == 1);

    TEST_CONDITION(children[0].label == "row2");

    return true;
}

int main() {
    e8::BeginTestSuite("placement_hierarchy");
    e8::RunTest("OneLayerTest", OneLayerTest);
    e8::RunTest("MultiLayerTest", MultiLayerTest);
    e8::EndTestSuite();
    return 0;
}
