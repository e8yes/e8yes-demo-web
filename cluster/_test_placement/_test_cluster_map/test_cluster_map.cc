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

#include <string>
#include <unordered_set>
#include <vector>

#include "cluster/placement/cluster_map.h"
#include "cluster/placement/common_types.h"
#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/machine.pb.h"

namespace {

e8::ClusterMapData Proto() {
    e8::ClusterMapData proto;

    e8::ClusterTreeNode root;
    root.set_hierarchy(e8::ClusterTreeNode::ROOT);
    *root.mutable_bucket()->mutable_list_bucket()->add_child_labels() = "row1";
    *root.mutable_bucket()->mutable_list_bucket()->add_child_labels() = "row2";

    e8::ClusterTreeNode row1;
    e8::ClusterTreeNode row2;
    row1.set_hierarchy(e8::ClusterTreeNode::ROW);
    *row1.mutable_bucket()->mutable_list_bucket()->add_child_labels() = "machine1";
    *row1.mutable_bucket()->mutable_list_bucket()->add_child_labels() = "machine2";
    row2.set_hierarchy(e8::ClusterTreeNode::ROW);
    *row2.mutable_bucket()->mutable_list_bucket()->add_child_labels() = "machine3";
    *row2.mutable_bucket()->mutable_list_bucket()->add_child_labels() = "machine4";

    e8::ClusterTreeNode machine1;
    e8::ClusterTreeNode machine2;
    e8::ClusterTreeNode machine3;
    e8::ClusterTreeNode machine4;
    machine1.set_hierarchy(e8::ClusterTreeNode::MACHINE);
    machine1.mutable_machine()->set_address("192.168.1.201");
    machine1.mutable_machine()->set_reachable(true);
    machine1.mutable_machine()->mutable_capabilities()->set_cpu(1);

    machine2.set_hierarchy(e8::ClusterTreeNode::MACHINE);
    machine2.mutable_machine()->set_address("192.168.1.202");
    machine2.mutable_machine()->set_reachable(true);
    machine2.mutable_machine()->mutable_capabilities()->set_ram(1);

    machine3.set_hierarchy(e8::ClusterTreeNode::MACHINE);
    machine3.mutable_machine()->set_address("192.168.1.203");
    machine3.mutable_machine()->set_reachable(true);
    machine3.mutable_machine()->mutable_capabilities()->set_storage(1);

    machine4.set_hierarchy(e8::ClusterTreeNode::MACHINE);
    machine4.mutable_machine()->set_address("192.168.1.204");
    machine4.mutable_machine()->set_reachable(true);
    machine4.mutable_machine()->mutable_capabilities()->set_coral(1);

    (*proto.mutable_tree_nodes())["root"] = root;
    (*proto.mutable_tree_nodes())["row1"] = row1;
    (*proto.mutable_tree_nodes())["row2"] = row2;
    (*proto.mutable_tree_nodes())["machine1"] = machine1;
    (*proto.mutable_tree_nodes())["machine2"] = machine2;
    (*proto.mutable_tree_nodes())["machine3"] = machine3;
    (*proto.mutable_tree_nodes())["machine4"] = machine4;

    return proto;
}

bool OneOfMachineSet(e8::Machine const &machine,
                     std::unordered_set<std::string> const &machine_set) {
    auto it = machine_set.find(machine.address());
    return it != machine_set.end();
}

} // namespace

bool ConstructAndExportTest() {
    e8::ClusterMapData proto = Proto();

    e8::ClusterMap cluster_map(proto);
    TEST_CONDITION(proto.DebugString() == cluster_map.ToProto().DebugString());

    return true;
}

bool ValidPlacementTest() {
    e8::ClusterMapData proto = Proto();

    e8::ResourceDescriptor resource(/*key=*/"a");

    e8::ClusterMap cluster_map(proto);
    std::vector<e8::Machine> machines = cluster_map.TakeRootFor(resource)
                                            .Select(e8::ClusterTreeNode::ROW, /*num_items=*/2)
                                            .Select(e8::ClusterTreeNode::MACHINE, /*num_items=*/1)
                                            .Emit();

    TEST_CONDITION(machines.size() == 2);
    TEST_CONDITION(machines[0].address() != machines[1].address());

    std::unordered_set<std::string> row1_machines{"192.168.1.201", "192.168.1.202"};
    std::unordered_set<std::string> row2_machines{"192.168.1.203", "192.168.1.204"};
    TEST_CONDITION((OneOfMachineSet(machines[0], row1_machines) &&
                    OneOfMachineSet(machines[1], row2_machines)) ||
                   (OneOfMachineSet(machines[0], row2_machines) &&
                    OneOfMachineSet(machines[1], row1_machines)));

    return true;
}

bool RepeatablePlacementTest() {
    e8::ClusterMapData proto = Proto();

    e8::ResourceDescriptor resource(/*key=*/"b");

    e8::ClusterMap cluster_map(proto);
    std::vector<e8::Machine> machines1 =
        cluster_map.TakeRootFor(resource)
            .Select(e8::ClusterTreeNode::ROW, /*num_replicas=*/2)
            .Select(e8::ClusterTreeNode::MACHINE, /*num_replicas=*/1)
            .Emit();

    std::vector<e8::Machine> machines2 =
        cluster_map.TakeRootFor(resource)
            .Select(e8::ClusterTreeNode::ROW, /*num_replicas=*/2)
            .Select(e8::ClusterTreeNode::MACHINE, /*num_replicas=*/1)
            .Emit();

    TEST_CONDITION(machines1.size() == 2);
    TEST_CONDITION(machines2.size() == 2);

    TEST_CONDITION(machines1[0].DebugString() == machines2[0].DebugString());
    TEST_CONDITION(machines1[1].DebugString() == machines2[1].DebugString());

    return true;
}

int main() {
    e8::BeginTestSuite("placement_cluster_map");
    e8::RunTest("ConstructAndExportTest", ConstructAndExportTest);
    e8::RunTest("ValidPlacementTest", ValidPlacementTest);
    e8::RunTest("RepeatablePlacementTest", RepeatablePlacementTest);
    e8::EndTestSuite();
    return 0;
}
