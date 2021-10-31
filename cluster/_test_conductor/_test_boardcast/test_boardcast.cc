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

#include <cassert>
#include <vector>

#include "cluster/conductor/boardcast.h"
#include "cluster/conductor/conductor.h"
#include "cluster/conductor/integration_test/local_cluster.h"
#include "cluster/conductor/integration_test/mock_resource_service.h"
#include "cluster/placement/cluster_map.h"
#include "cluster/placement/common_types.h"
#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/cluster_revision.pb.h"
#include "proto_cc/cluster_revision_command.pb.h"

namespace {

class MockLeaderRevisionConductor : public e8::ClusterRevisionConductorInterface {
  public:
    MockLeaderRevisionConductor();
    ~MockLeaderRevisionConductor() override;

    bool ShouldBoardcast() const override;

    e8::ClusterRevisionResult RunCommand(e8::ClusterRevisionCommand const &command) override;
};

MockLeaderRevisionConductor::MockLeaderRevisionConductor() {}

MockLeaderRevisionConductor::~MockLeaderRevisionConductor() {}

bool MockLeaderRevisionConductor::ShouldBoardcast() const { return true; }

e8::ClusterRevisionResult
MockLeaderRevisionConductor::RunCommand(e8::ClusterRevisionCommand const & /*command*/) {
    assert(false);
}

e8::ClusterMapRevision::Action AddRootAction() {
    e8::Bucket root_bucket;
    root_bucket.mutable_uniform_bucket()->set_prime(13);

    e8::ClusterTreeNode root_node;
    root_node.set_hierarchy(e8::ClusterTreeNode::ROOT);
    *root_node.mutable_bucket() = root_bucket;

    e8::ClusterMapRevision::Action add_root;
    add_root.set_operation(e8::ClusterMapRevision::ADD_TREE_NODE);
    add_root.set_node_label("root");
    add_root.set_parent_label("root");
    *add_root.mutable_tree_node() = root_node;

    return add_root;
}

e8::ClusterMapRevision::Action AddChildAction(e8::ClusterTreeNodeLabel const &child_label,
                                              std::string const &machine_address) {
    e8::ClusterTreeNode child_node;
    child_node.set_hierarchy(e8::ClusterTreeNode::MACHINE);
    child_node.mutable_machine()->set_address(machine_address);

    e8::ClusterMapRevision::Action add_root;
    add_root.set_operation(e8::ClusterMapRevision::ADD_TREE_NODE);
    add_root.set_node_label(child_label);
    add_root.set_parent_label("root");
    *add_root.mutable_tree_node() = child_node;

    return add_root;
}

} // namespace

bool BoardcastRevisionAllSuccessfulTest() {
    e8::LocalCluster cluster(/*num_conductor=*/2, /*num_resource_worker=*/10);

    TEST_CONDITION(cluster.GetWorkerNode(0).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(1).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(2).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(3).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(4).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(5).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(6).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(7).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(8).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(9).service->GetClusterMap().Version() == 0);

    // In the first revision, we creates the root node and adds machine 1 into the cluster.
    e8::ClusterMapRevision revision1;
    revision1.set_from_version_epoch(0);
    revision1.set_to_version_epoch(1);
    *revision1.add_actions() = AddRootAction();
    *revision1.add_actions() =
        AddChildAction(/*child_label=*/"child1", cluster.GetWorkerNode(0).address);

    e8::ClusterRevisionWork revision_work;
    revision_work.set_machine_version_epoch(0);
    revision_work.add_targets();
    revision_work.mutable_targets(0)->set_node_label(revision1.actions(1).node_label());
    *revision_work.mutable_targets(0)->mutable_machine() =
        revision1.actions(1).tree_node().machine();

    std::vector<e8::ClusterMapRevision> all_revisions;
    all_revisions.push_back(revision1);

    e8::ClusterRevisionWork leftover_work;
    bool complete = e8::BoardcastRevision(revision_work, all_revisions, /*rate=*/0.5f,
                                          MockLeaderRevisionConductor(), &leftover_work);
    TEST_CONDITION(complete);
    TEST_CONDITION(leftover_work.targets().empty());
    TEST_CONDITION(cluster.GetWorkerNode(0).service->GetClusterMap().Version() == 1);
    TEST_CONDITION(cluster.GetWorkerNode(1).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(2).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(3).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(4).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(5).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(6).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(7).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(8).service->GetClusterMap().Version() == 0);
    TEST_CONDITION(cluster.GetWorkerNode(9).service->GetClusterMap().Version() == 0);

    // In the second revision, we add the rest of the machines in.
    e8::ClusterMapRevision revision2;
    revision2.set_from_version_epoch(1);
    revision2.set_to_version_epoch(2);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child2", cluster.GetWorkerNode(1).address);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child3", cluster.GetWorkerNode(2).address);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child4", cluster.GetWorkerNode(3).address);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child5", cluster.GetWorkerNode(4).address);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child6", cluster.GetWorkerNode(5).address);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child7", cluster.GetWorkerNode(6).address);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child8", cluster.GetWorkerNode(7).address);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child9", cluster.GetWorkerNode(8).address);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child10", cluster.GetWorkerNode(9).address);

    for (auto const &action : revision2.actions()) {
        e8::ClusterRevisionWork::Target *target = revision_work.add_targets();
        target->set_node_label(action.node_label());
        *target->mutable_machine() = action.tree_node().machine();
    }

    all_revisions.push_back(revision2);

    complete = e8::BoardcastRevision(revision_work, all_revisions, /*rate=*/0.5f,
                                     MockLeaderRevisionConductor(), &leftover_work);
    TEST_CONDITION(complete);
    TEST_CONDITION(leftover_work.targets().empty());
    TEST_CONDITION(cluster.GetWorkerNode(0).service->GetClusterMap().Version() == 2);
    TEST_CONDITION(cluster.GetWorkerNode(1).service->GetClusterMap().Version() == 2);
    TEST_CONDITION(cluster.GetWorkerNode(2).service->GetClusterMap().Version() == 2);
    TEST_CONDITION(cluster.GetWorkerNode(3).service->GetClusterMap().Version() == 2);
    TEST_CONDITION(cluster.GetWorkerNode(4).service->GetClusterMap().Version() == 2);
    TEST_CONDITION(cluster.GetWorkerNode(5).service->GetClusterMap().Version() == 2);
    TEST_CONDITION(cluster.GetWorkerNode(6).service->GetClusterMap().Version() == 2);
    TEST_CONDITION(cluster.GetWorkerNode(7).service->GetClusterMap().Version() == 2);
    TEST_CONDITION(cluster.GetWorkerNode(8).service->GetClusterMap().Version() == 2);
    TEST_CONDITION(cluster.GetWorkerNode(9).service->GetClusterMap().Version() == 2);

    return true;
}

int main() {
    e8::BeginTestSuite("cluster_conductor_boardcast");
    e8::RunTest("BoardcastRevisionTest", BoardcastRevisionAllSuccessfulTest);
    e8::EndTestSuite();
    return 0;
}
