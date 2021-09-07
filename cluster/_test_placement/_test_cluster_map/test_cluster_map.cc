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

#include <iostream>

#include "cluster/placement/cluster_map.h"
#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/cluster.pb.h"

e8::ClusterMapData Proto() {
    e8::ClusterMapData proto;

    e8::ClusterTreeNode root;
    *root.mutable_bucket()->mutable_list_bucket()->add_child_labels() = "row1";
    *root.mutable_bucket()->mutable_list_bucket()->add_child_labels() = "row2";

    e8::ClusterTreeNode row1;
    e8::ClusterTreeNode row2;
    *row1.mutable_bucket()->mutable_list_bucket()->add_child_labels() = "machine1";
    *row1.mutable_bucket()->mutable_list_bucket()->add_child_labels() = "machine2";
    *row2.mutable_bucket()->mutable_list_bucket()->add_child_labels() = "machine3";
    *row2.mutable_bucket()->mutable_list_bucket()->add_child_labels() = "machine4";

    e8::ClusterTreeNode machine1;
    e8::ClusterTreeNode machine2;
    e8::ClusterTreeNode machine3;
    e8::ClusterTreeNode machine4;
    machine1.mutable_machine()->set_address("192.168.1.201");
    machine1.mutable_machine()->set_reachable(true);
    machine1.mutable_machine()->mutable_capabilities()->set_cpu(1);

    machine2.mutable_machine()->set_address("192.168.1.202");
    machine2.mutable_machine()->set_reachable(true);
    machine2.mutable_machine()->mutable_capabilities()->set_ram(1);

    machine3.mutable_machine()->set_address("192.168.1.203");
    machine3.mutable_machine()->set_reachable(true);
    machine3.mutable_machine()->mutable_capabilities()->set_storage(1);

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

bool ConstructAndExportTest() {
    e8::ClusterMapData proto = Proto();

    e8::ClusterMap cluster_map(proto);
    TEST_CONDITION(proto.DebugString() == cluster_map.ToProto().DebugString());

    return true;
}

int main() {
    e8::BeginTestSuite("placement_cluster_map");
    e8::RunTest("ConstructAndExportTest", ConstructAndExportTest);
    e8::EndTestSuite();
    return 0;
}
