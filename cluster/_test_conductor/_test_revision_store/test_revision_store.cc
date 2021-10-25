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

#include <optional>

#include "cluster/conductor/revision_store.h"
#include "cluster/placement/cluster_map.h"
#include "cluster/placement/common_types.h"
#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/bucket.pb.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/cluster_revision.pb.h"
#include "proto_cc/cluster_revision_command.pb.h"

namespace {

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

bool EnqueueRejectionTest() {
    e8::ClusterRevisionStore store;

    // The first revision must start from version 0.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(1);
    revision.set_to_version_epoch(2);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand command;
    command.set_resource_service_id("test");
    *command.mutable_enqueue_revision()->mutable_revision() = revision;

    e8::ClusterRevisionResult result = store.Run(command);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(!result.enqueue_result().successful());

    // Fixes the version and successfully enqueues the first revision.
    command.mutable_enqueue_revision()->mutable_revision()->set_from_version_epoch(0);
    command.mutable_enqueue_revision()->mutable_revision()->set_to_version_epoch(1);

    result = store.Run(command);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(result.enqueue_result().successful());

    // Shouldn't be able to enqueue the same revision mutliple times.
    result = store.Run(command);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(!result.enqueue_result().successful());

    // Tries to enqueue a revision with leaping version.
    e8::ClusterMapRevision revision2;
    revision2.set_from_version_epoch(2);
    revision2.set_to_version_epoch(3);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child1", /*machine_address=*/"192.168.0.1:1234");

    e8::ClusterRevisionCommand command2;
    command2.set_resource_service_id("test");
    *command2.mutable_enqueue_revision()->mutable_revision() = revision2;

    result = store.Run(command2);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(!result.enqueue_result().successful());

    // Fixes the version and successfully enqueues the second revision.
    command2.mutable_enqueue_revision()->mutable_revision()->set_from_version_epoch(1);
    command2.mutable_enqueue_revision()->mutable_revision()->set_to_version_epoch(2);

    result = store.Run(command2);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(result.enqueue_result().successful());

    return true;
}

bool EnqueueAndGetTest() {
    e8::ClusterRevisionStore store;

    // Enqueue the root node and check that it's going to be worked on but not applied.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(0);
    revision.set_to_version_epoch(1);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand enqueue_command;
    enqueue_command.set_resource_service_id("test");
    *enqueue_command.mutable_enqueue_revision()->mutable_revision() = revision;

    e8::ClusterRevisionResult result = store.Run(enqueue_command);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(result.enqueue_result().successful());

    // The revision is expected not to have applied but work-in-progress
    e8::ClusterRevisionCommand get_command;
    get_command.set_resource_service_id("test");
    get_command.mutable_get_revision()->set_starting_from_epoch(1);
    get_command.mutable_get_revision()->set_scope(e8::GetClusterRevisionCommand::APPLIED);

    result = store.Run(get_command);
    TEST_CONDITION(result.has_get_result());
    TEST_CONDITION(!result.get_result().has_revision());

    get_command.mutable_get_revision()->set_scope(e8::GetClusterRevisionCommand::APPLIED_WIP);
    result = store.Run(get_command);
    TEST_CONDITION(result.has_get_result());
    TEST_CONDITION(result.get_result().revision().DebugString() == revision.DebugString());

    // Enqueue another revision. It is expected to be pending.
    e8::ClusterMapRevision revision2;
    revision2.set_from_version_epoch(1);
    revision2.set_to_version_epoch(2);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child1", /*machine_address=*/"192.168.0.1:1234");

    e8::ClusterRevisionCommand enqueue_command2;
    enqueue_command2.set_resource_service_id("test");
    *enqueue_command2.mutable_enqueue_revision()->mutable_revision() = revision2;

    result = store.Run(enqueue_command2);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(result.enqueue_result().successful());

    get_command.mutable_get_revision()->set_scope(e8::GetClusterRevisionCommand::APPLIED_WIP);
    result = store.Run(get_command);
    TEST_CONDITION(result.has_get_result());
    TEST_CONDITION(result.get_result().revision().DebugString() == revision.DebugString());

    get_command.mutable_get_revision()->set_scope(
        e8::GetClusterRevisionCommand::APPLIED_WIP_PENDING);
    result = store.Run(get_command);
    TEST_CONDITION(result.has_get_result());
    TEST_CONDITION(result.get_result().revision().from_version_epoch() == 0);
    TEST_CONDITION(result.get_result().revision().to_version_epoch() == 2);

    return true;
}

bool EnqueueAndApplyThenGetTest() {
    e8::ClusterRevisionStore store;

    // Enqueues and applies the add-root command.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(0);
    revision.set_to_version_epoch(1);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand enqueue_command;
    enqueue_command.set_resource_service_id("test");
    *enqueue_command.mutable_enqueue_revision()->mutable_revision() = revision;

    e8::ClusterRevisionResult result = store.Run(enqueue_command);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(result.enqueue_result().successful());

    e8::ClusterRevisionCommand apply_command;
    apply_command.set_resource_service_id("test");
    *apply_command.mutable_apply_revision()->mutable_revision() = revision;

    result = store.Run(apply_command);
    TEST_CONDITION(result.has_apply_result());
    TEST_CONDITION(result.apply_result().successful());

    // Tests that the store won't double apply a revision.
    result = store.Run(apply_command);
    TEST_CONDITION(result.has_apply_result());
    TEST_CONDITION(!result.apply_result().successful());

    e8::ClusterRevisionCommand get_command;
    get_command.set_resource_service_id("test");
    get_command.mutable_get_revision()->set_starting_from_epoch(1);
    get_command.mutable_get_revision()->set_scope(e8::GetClusterRevisionCommand::APPLIED);

    result = store.Run(get_command);
    TEST_CONDITION(result.has_get_result());
    TEST_CONDITION(result.get_result().revision().DebugString() == revision.DebugString());

    return true;
}

bool EnqueueAndWorkInProgressTest() {
    e8::ClusterRevisionStore store;

    TEST_CONDITION(!store.WorkInProgress().has_value());

    // Enqueues then checks that the revision is marked as work-in-progress.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(0);
    revision.set_to_version_epoch(1);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand enqueue_command;
    enqueue_command.set_resource_service_id("test");
    *enqueue_command.mutable_enqueue_revision()->mutable_revision() = revision;

    e8::ClusterRevisionResult result = store.Run(enqueue_command);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(result.enqueue_result().successful());

    std::optional<e8::ClusterRevisionStore::RevisionSpecs> wip_specs = store.WorkInProgress();
    TEST_CONDITION(wip_specs.has_value());
    TEST_CONDITION(wip_specs->resource_service_id == "test");
    TEST_CONDITION(wip_specs->cluster_map.Version() == 0);
    TEST_CONDITION(wip_specs->revisions.size() == 1);
    TEST_CONDITION(wip_specs->revisions.rbegin()->DebugString() == revision.DebugString());
    TEST_CONDITION(wip_specs->wip_from_version_epoch == 0);

    return true;
}

bool EnqueueAndApplyThenWorkInProgressTest() {
    e8::ClusterRevisionStore store;

    TEST_CONDITION(!store.WorkInProgress().has_value());

    // Enqueues two revisions.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(0);
    revision.set_to_version_epoch(1);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand enqueue_command;
    enqueue_command.set_resource_service_id("test");
    *enqueue_command.mutable_enqueue_revision()->mutable_revision() = revision;

    e8::ClusterRevisionResult result = store.Run(enqueue_command);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(result.enqueue_result().successful());

    e8::ClusterMapRevision revision2;
    revision2.set_from_version_epoch(1);
    revision2.set_to_version_epoch(2);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child1", /*machine_address=*/"192.168.0.1:1234");

    e8::ClusterRevisionCommand enqueue_command2;
    enqueue_command2.set_resource_service_id("test");
    *enqueue_command2.mutable_enqueue_revision()->mutable_revision() = revision2;

    result = store.Run(enqueue_command2);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(result.enqueue_result().successful());

    // Applies the add-root revision.
    e8::ClusterRevisionCommand apply_command;
    apply_command.set_resource_service_id("test");
    *apply_command.mutable_apply_revision()->mutable_revision() = revision;

    result = store.Run(apply_command);
    TEST_CONDITION(result.has_apply_result());
    TEST_CONDITION(result.apply_result().successful());

    // Checks that the add-child revision is currently marked as work-in-progress.
    std::optional<e8::ClusterRevisionStore::RevisionSpecs> wip_specs = store.WorkInProgress();
    TEST_CONDITION(wip_specs.has_value());
    TEST_CONDITION(wip_specs->resource_service_id == "test");
    TEST_CONDITION(wip_specs->cluster_map.Version() == 1);
    TEST_CONDITION(wip_specs->revisions.size() == 2);
    TEST_CONDITION(wip_specs->revisions.rbegin()->DebugString() == revision2.DebugString());
    TEST_CONDITION(wip_specs->wip_from_version_epoch == 1);

    // Applies the add-child revision, then nothing is work-in-progress.
    e8::ClusterRevisionCommand apply_command2;
    apply_command2.set_resource_service_id("test");
    *apply_command2.mutable_apply_revision()->mutable_revision() = revision2;

    result = store.Run(apply_command2);
    TEST_CONDITION(result.has_apply_result());
    TEST_CONDITION(result.apply_result().successful());

    std::optional<e8::ClusterRevisionStore::RevisionSpecs> wip_specs2 = store.WorkInProgress();
    TEST_CONDITION(!wip_specs2.has_value());

    return true;
}

int main() {
    e8::BeginTestSuite("cluster_conductor_revision_store");
    e8::RunTest("EnqueueRejectionTest", EnqueueRejectionTest);
    e8::RunTest("EnqueueAndGetTest", EnqueueAndGetTest);
    e8::RunTest("EnqueueAndApplyThenGetTest", EnqueueAndApplyThenGetTest);
    e8::RunTest("EnqueueAndWorkInProgressTest", EnqueueAndWorkInProgressTest);
    e8::RunTest("EnqueueAndApplyThenWorkInProgressTest", EnqueueAndApplyThenWorkInProgressTest);
    e8::EndTestSuite();
    return 0;
}
