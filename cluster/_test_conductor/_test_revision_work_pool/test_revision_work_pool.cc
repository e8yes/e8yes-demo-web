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

#include "cluster/conductor/revision_work_pool.h"
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

bool PollResourceServiceTest() {
    e8::ClusterRevisionWorkPool work_pool;

    // Polls and checks if the work pool's initial state is empty.
    e8::ClusterRevisionCommand poll_resource_service_command;
    *poll_resource_service_command.mutable_poll_resource_service() =
        e8::PollClusterResourceServiceCommand();
    e8::ClusterRevisionResult result = work_pool.Run(poll_resource_service_command);
    TEST_CONDITION(result.has_poll_resource_service_result());
    TEST_CONDITION(result.poll_resource_service_result().resource_services_size() == 0);

    // Enqueues a revision for the test resource service.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(0);
    revision.set_to_version_epoch(1);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand enqueue_command;
    enqueue_command.set_resource_service_id("test");
    *enqueue_command.mutable_enqueue_revision()->mutable_revision() = revision;
    work_pool.Run(enqueue_command);

    result = work_pool.Run(poll_resource_service_command);
    TEST_CONDITION(result.has_poll_resource_service_result());
    TEST_CONDITION(result.poll_resource_service_result().resource_services_size() == 1);
    TEST_CONDITION(result.poll_resource_service_result().resource_services(0) == "test");

    // Enqueues a revision for the test2 resource service.
    e8::ClusterRevisionCommand enqueue_command2;
    enqueue_command2.set_resource_service_id("test2");
    *enqueue_command2.mutable_enqueue_revision()->mutable_revision() = revision;
    work_pool.Run(enqueue_command2);

    result = work_pool.Run(poll_resource_service_command);
    TEST_CONDITION(result.has_poll_resource_service_result());
    TEST_CONDITION(result.poll_resource_service_result().resource_services_size() == 2);

    return true;
}

bool EnqueueRejectionTest() {
    e8::ClusterRevisionWorkPool work_pool;

    // The first revision must start from version 0.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(1);
    revision.set_to_version_epoch(2);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand command;
    command.set_resource_service_id("test");
    *command.mutable_enqueue_revision()->mutable_revision() = revision;

    e8::ClusterRevisionResult result = work_pool.Run(command);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(!result.enqueue_result().successful());

    // Fixes the version and successfully enqueues the first revision.
    command.mutable_enqueue_revision()->mutable_revision()->set_from_version_epoch(0);
    command.mutable_enqueue_revision()->mutable_revision()->set_to_version_epoch(1);
    result = work_pool.Run(command);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(result.enqueue_result().successful());

    // Shouldn't be able to enqueue the same revision mutliple times.
    result = work_pool.Run(command);
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
    result = work_pool.Run(command2);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(!result.enqueue_result().successful());

    // Fixes the version and successfully enqueues the second revision.
    command2.mutable_enqueue_revision()->mutable_revision()->set_from_version_epoch(1);
    command2.mutable_enqueue_revision()->mutable_revision()->set_to_version_epoch(2);
    result = work_pool.Run(command2);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(result.enqueue_result().successful());

    return true;
}

bool EnqueueAndPollTest() {
    e8::ClusterRevisionWorkPool work_pool;

    // Enqueue the root node and check that it's in the pending queue.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(0);
    revision.set_to_version_epoch(1);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand enqueue_command;
    enqueue_command.set_resource_service_id("test");
    *enqueue_command.mutable_enqueue_revision()->mutable_revision() = revision;
    e8::ClusterRevisionResult result = work_pool.Run(enqueue_command);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(result.enqueue_result().successful());

    // Checks if the pending revision is the one we just enqueued.
    e8::ClusterRevisionCommand poll_command;
    poll_command.set_resource_service_id("test");
    *poll_command.mutable_poll_revision() = e8::PollPendingClusterRevisionCommand();
    result = work_pool.Run(poll_command);
    TEST_CONDITION(result.has_poll_result());
    TEST_CONDITION(result.poll_result().has_pending_revision());
    TEST_CONDITION(result.poll_result().pending_revision().DebugString() == revision.DebugString());

    // Enqueues another revision.
    e8::ClusterMapRevision revision2;
    revision2.set_from_version_epoch(1);
    revision2.set_to_version_epoch(2);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child1", /*machine_address=*/"192.168.0.1:1234");

    e8::ClusterRevisionCommand enqueue_command2;
    enqueue_command2.set_resource_service_id("test");
    *enqueue_command2.mutable_enqueue_revision()->mutable_revision() = revision2;
    result = work_pool.Run(enqueue_command2);
    TEST_CONDITION(result.has_enqueue_result());
    TEST_CONDITION(result.enqueue_result().successful());

    // Polls to ses if the second revision gets aggregated into the pending revision object.
    result = work_pool.Run(poll_command);
    TEST_CONDITION(result.has_poll_result());
    TEST_CONDITION(result.poll_result().pending_revision().from_version_epoch() == 0);
    TEST_CONDITION(result.poll_result().pending_revision().to_version_epoch() == 2);
    TEST_CONDITION(result.poll_result().pending_revision().actions_size() ==
                   revision.actions_size() + revision2.actions_size());

    return true;
}

bool EnqueueAndCreateWorkTest() {
    e8::ClusterRevisionWorkPool work_pool;

    // Creates work while the pending queue is empty.
    e8::ClusterRevisionCommand create_work_command;
    create_work_command.set_resource_service_id("test");
    create_work_command.mutable_create_work()->set_from_version_epoch(0);
    create_work_command.mutable_create_work()->set_to_version_epoch(1);

    e8::ClusterRevisionResult result = work_pool.Run(create_work_command);
    TEST_CONDITION(result.has_create_work_result());
    TEST_CONDITION(!result.create_work_result().successful());
    TEST_CONDITION(!result.create_work_result().has_pending());
    TEST_CONDITION(!result.create_work_result().pending_versions_mismatch());

    // Enqueues the add-root and add-child revisions.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(0);
    revision.set_to_version_epoch(1);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand enqueue_command;
    enqueue_command.set_resource_service_id("test");
    *enqueue_command.mutable_enqueue_revision()->mutable_revision() = revision;
    work_pool.Run(enqueue_command);

    e8::ClusterMapRevision revision2;
    revision2.set_from_version_epoch(1);
    revision2.set_to_version_epoch(2);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child1", /*machine_address=*/"192.168.0.1:1234");

    e8::ClusterRevisionCommand enqueue_command2;
    enqueue_command2.set_resource_service_id("test");
    *enqueue_command2.mutable_enqueue_revision()->mutable_revision() = revision2;
    work_pool.Run(enqueue_command2);

    // Creates new work with the wrong version epochs, so it should fail.
    create_work_command.mutable_create_work()->set_from_version_epoch(0);
    create_work_command.mutable_create_work()->set_to_version_epoch(1);
    result = work_pool.Run(create_work_command);
    TEST_CONDITION(result.has_create_work_result());
    TEST_CONDITION(!result.create_work_result().successful());
    TEST_CONDITION(result.create_work_result().has_pending());
    TEST_CONDITION(result.create_work_result().pending_versions_mismatch());

    // Creates new work with proper version epochs.
    create_work_command.mutable_create_work()->set_from_version_epoch(0);
    create_work_command.mutable_create_work()->set_to_version_epoch(2);

    e8::ClusterRevisionWork::Target target;
    target.set_node_label(revision2.actions(0).node_label());
    *target.mutable_machine() = revision2.actions(0).tree_node().machine();
    *create_work_command.mutable_create_work()->add_targets() = target;

    result = work_pool.Run(create_work_command);
    TEST_CONDITION(result.has_create_work_result());
    TEST_CONDITION(result.create_work_result().successful());

    // Checks that the pending queue is empty after the successful work creation.
    e8::ClusterRevisionCommand poll_command;
    poll_command.set_resource_service_id("test");
    *poll_command.mutable_poll_revision() = e8::PollPendingClusterRevisionCommand();
    result = work_pool.Run(poll_command);
    TEST_CONDITION(result.has_poll_result());
    TEST_CONDITION(!result.poll_result().has_pending_revision());
    TEST_CONDITION(result.poll_result().all_revision().from_version_epoch() == 0);
    TEST_CONDITION(result.poll_result().all_revision().to_version_epoch() == 2);

    return true;
}

bool EnqueueAndCreateWorkThenGetWorkTest() {
    e8::ClusterRevisionWorkPool work_pool;

    // Enqueues the add-root and add-child revisions.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(0);
    revision.set_to_version_epoch(1);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand enqueue_command;
    enqueue_command.set_resource_service_id("test");
    *enqueue_command.mutable_enqueue_revision()->mutable_revision() = revision;
    work_pool.Run(enqueue_command);

    e8::ClusterMapRevision revision2;
    revision2.set_from_version_epoch(1);
    revision2.set_to_version_epoch(2);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child1", /*machine_address=*/"192.168.0.1:1234");

    e8::ClusterRevisionCommand enqueue_command2;
    enqueue_command2.set_resource_service_id("test");
    *enqueue_command2.mutable_enqueue_revision()->mutable_revision() = revision2;
    work_pool.Run(enqueue_command2);

    // Creates new work.
    e8::ClusterRevisionCommand create_work_command;
    create_work_command.set_resource_service_id("test");
    create_work_command.mutable_create_work()->set_from_version_epoch(0);
    create_work_command.mutable_create_work()->set_to_version_epoch(2);

    e8::ClusterRevisionWork::Target target;
    target.set_node_label(revision2.actions(0).node_label());
    *target.mutable_machine() = revision2.actions(0).tree_node().machine();
    *create_work_command.mutable_create_work()->add_targets() = target;

    e8::ClusterRevisionResult result = work_pool.Run(create_work_command);
    TEST_CONDITION(result.has_create_work_result());
    TEST_CONDITION(result.create_work_result().successful());

    // Get work.
    e8::ClusterRevisionCommand get_work_command;
    get_work_command.set_resource_service_id("test");
    *get_work_command.mutable_get_all_work() = e8::GetAllClusterRevisionWorkCommand();

    result = work_pool.Run(get_work_command);
    TEST_CONDITION(result.has_get_all_work_result());
    TEST_CONDITION(result.get_all_work_result().all_revisions().size() == 2);
    TEST_CONDITION(result.get_all_work_result().all_work().size() == 1);
    TEST_CONDITION(result.get_all_work_result().all_work(0).machine_version_epoch() == 0);
    TEST_CONDITION(result.get_all_work_result().all_work(0).targets().size() == 1);
    TEST_CONDITION(result.get_all_work_result().all_work(0).targets(0).node_label() ==
                   revision2.actions(0).node_label());
    TEST_CONDITION(result.get_all_work_result().all_work(0).targets(0).machine().DebugString() ==
                   revision2.actions(0).tree_node().machine().DebugString());

    return true;
}

bool EnqueueAndCreateWorkThenUpdateWorkTest() {
    e8::ClusterRevisionWorkPool work_pool;

    // Enqueues the root node and creates work off of it.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(0);
    revision.set_to_version_epoch(1);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand enqueue_command;
    enqueue_command.set_resource_service_id("test");
    *enqueue_command.mutable_enqueue_revision()->mutable_revision() = revision;
    work_pool.Run(enqueue_command);

    e8::ClusterRevisionCommand create_work_command;
    create_work_command.set_resource_service_id("test");
    create_work_command.mutable_create_work()->set_from_version_epoch(0);
    create_work_command.mutable_create_work()->set_to_version_epoch(1);
    work_pool.Run(create_work_command);

    // Enqueues a child node and creates the second work.
    e8::ClusterMapRevision revision2;
    revision2.set_from_version_epoch(1);
    revision2.set_to_version_epoch(2);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child1", /*machine_address=*/"192.168.0.1:1234");

    e8::ClusterRevisionCommand enqueue_command2;
    enqueue_command2.set_resource_service_id("test");
    *enqueue_command2.mutable_enqueue_revision()->mutable_revision() = revision2;
    work_pool.Run(enqueue_command2);

    e8::ClusterRevisionCommand create_work_command2;
    create_work_command2.set_resource_service_id("test");
    create_work_command2.mutable_create_work()->set_from_version_epoch(1);
    create_work_command2.mutable_create_work()->set_to_version_epoch(2);
    work_pool.Run(create_work_command2);

    // Updates the second work.
    e8::ClusterRevisionCommand update_work_command;
    update_work_command.set_resource_service_id("test");
    update_work_command.mutable_update_work()->set_machine_version_epoch(1);

    e8::ClusterRevisionWork::Target target;
    target.set_node_label(revision2.actions(0).node_label());
    *target.mutable_machine() = revision2.actions(0).tree_node().machine();
    *update_work_command.mutable_update_work()->add_targets() = target;

    e8::ClusterRevisionResult result = work_pool.Run(update_work_command);
    TEST_CONDITION(result.has_update_work_result());
    TEST_CONDITION(result.update_work_result().successful());

    // Checks if the second work gets updated.
    e8::ClusterRevisionCommand get_work_command;
    get_work_command.set_resource_service_id("test");
    *get_work_command.mutable_get_all_work() = e8::GetAllClusterRevisionWorkCommand();
    result = work_pool.Run(get_work_command);
    TEST_CONDITION(result.has_get_all_work_result());
    TEST_CONDITION(result.get_all_work_result().all_work().size() == 2);
    TEST_CONDITION(result.get_all_work_result().all_work(1).machine_version_epoch() == 1);
    TEST_CONDITION(result.get_all_work_result().all_work(1).targets().size() == 1);
    TEST_CONDITION(result.get_all_work_result().all_work(1).targets(0).node_label() ==
                   revision2.actions(0).node_label());
    TEST_CONDITION(result.get_all_work_result().all_work(1).targets(0).machine().DebugString() ==
                   revision2.actions(0).tree_node().machine().DebugString());

    return true;
}

bool EnqueueAndCreateWorkThenFinishWorkTest() {
    e8::ClusterRevisionWorkPool work_pool;

    // Enqueues the root node and creates work off of it.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(0);
    revision.set_to_version_epoch(1);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand enqueue_command;
    enqueue_command.set_resource_service_id("test");
    *enqueue_command.mutable_enqueue_revision()->mutable_revision() = revision;
    work_pool.Run(enqueue_command);

    e8::ClusterRevisionCommand create_work_command;
    create_work_command.set_resource_service_id("test");
    create_work_command.mutable_create_work()->set_from_version_epoch(0);
    create_work_command.mutable_create_work()->set_to_version_epoch(1);
    work_pool.Run(create_work_command);

    // Finish the work just created, but with wrong version epoch.
    e8::ClusterRevisionCommand finish_work_command;
    finish_work_command.set_resource_service_id("test");
    finish_work_command.mutable_finish_work()->set_machine_version_epoch(1);
    e8::ClusterRevisionResult result = work_pool.Run(finish_work_command);
    TEST_CONDITION(result.has_finish_work_result());
    TEST_CONDITION(!result.finish_work_result().successful());

    // The work should still be in the work pool.
    e8::ClusterRevisionCommand get_work_command;
    get_work_command.set_resource_service_id("test");
    *get_work_command.mutable_get_all_work() = e8::GetAllClusterRevisionWorkCommand();
    result = work_pool.Run(get_work_command);
    TEST_CONDITION(result.has_get_all_work_result());
    TEST_CONDITION(result.get_all_work_result().all_work().size() == 1);

    // Corrects the version epoch and tries again.
    finish_work_command.mutable_finish_work()->set_machine_version_epoch(0);
    result = work_pool.Run(finish_work_command);
    TEST_CONDITION(result.has_finish_work_result());
    TEST_CONDITION(result.finish_work_result().successful());

    // The work should no longer exist.
    result = work_pool.Run(get_work_command);
    TEST_CONDITION(result.has_get_all_work_result());
    TEST_CONDITION(result.get_all_work_result().all_work().empty());

    return true;
}

bool ListHistoryTest() {
    e8::ClusterRevisionWorkPool work_pool;

    e8::ClusterRevisionCommand list_history_command;
    list_history_command.set_resource_service_id("test");
    *list_history_command.mutable_list_revision_history() = e8::ListRevisionHistoryCommand();
    e8::ClusterRevisionResult result = work_pool.Run(list_history_command);
    TEST_CONDITION(result.has_list_result());
    TEST_CONDITION(result.list_result().history_size() == 0);

    // Enqueues the root node.
    e8::ClusterMapRevision revision;
    revision.set_from_version_epoch(0);
    revision.set_to_version_epoch(1);
    *revision.add_actions() = AddRootAction();

    e8::ClusterRevisionCommand enqueue_command;
    enqueue_command.set_resource_service_id("test");
    *enqueue_command.mutable_enqueue_revision()->mutable_revision() = revision;
    work_pool.Run(enqueue_command);

    // We should see the add-root revision with pending status.
    result = work_pool.Run(list_history_command);
    TEST_CONDITION(result.has_list_result());
    TEST_CONDITION(result.list_result().history_size() == 1);
    TEST_CONDITION(result.list_result().history(0).revision().DebugString() ==
                   revision.DebugString());
    TEST_CONDITION(result.list_result().history(0).status() ==
                   e8::ListRevisionHistoryResult::Revision::PENDING);

    // Creates work, then the pending revision should become work-in-progress.
    e8::ClusterRevisionCommand create_work_command;
    create_work_command.set_resource_service_id("test");
    create_work_command.mutable_create_work()->set_from_version_epoch(0);
    create_work_command.mutable_create_work()->set_to_version_epoch(1);
    work_pool.Run(create_work_command);

    result = work_pool.Run(list_history_command);
    TEST_CONDITION(result.has_list_result());
    TEST_CONDITION(result.list_result().history_size() == 1);
    TEST_CONDITION(result.list_result().history(0).revision().DebugString() ==
                   revision.DebugString());
    TEST_CONDITION(result.list_result().history(0).status() ==
                   e8::ListRevisionHistoryResult::Revision::WORK_IN_PROGRESS);

    // Enqueues another revision.
    e8::ClusterMapRevision revision2;
    revision2.set_from_version_epoch(1);
    revision2.set_to_version_epoch(2);
    *revision2.add_actions() =
        AddChildAction(/*child_label=*/"child1", /*machine_address=*/"192.168.0.1:1234");

    e8::ClusterRevisionCommand enqueue_command2;
    enqueue_command2.set_resource_service_id("test");
    *enqueue_command2.mutable_enqueue_revision()->mutable_revision() = revision2;
    work_pool.Run(enqueue_command2);

    // Checks that we have 1 work-in-progress revision and 1 pending revision.
    result = work_pool.Run(list_history_command);
    TEST_CONDITION(result.has_list_result());
    TEST_CONDITION(result.list_result().history_size() == 2);
    TEST_CONDITION(result.list_result().history(0).revision().DebugString() ==
                   revision.DebugString());
    TEST_CONDITION(result.list_result().history(0).status() ==
                   e8::ListRevisionHistoryResult::Revision::WORK_IN_PROGRESS);
    TEST_CONDITION(result.list_result().history(1).revision().DebugString() ==
                   revision2.DebugString());
    TEST_CONDITION(result.list_result().history(1).status() ==
                   e8::ListRevisionHistoryResult::Revision::PENDING);

    // Finishes the revision work.
    e8::ClusterRevisionCommand finish_work_command;
    finish_work_command.set_resource_service_id("test");
    finish_work_command.mutable_finish_work()->set_machine_version_epoch(0);
    work_pool.Run(finish_work_command);

    // Checks that we have 1 applied revision and 1 pending revision.
    result = work_pool.Run(list_history_command);
    TEST_CONDITION(result.has_list_result());
    TEST_CONDITION(result.list_result().history_size() == 2);
    TEST_CONDITION(result.list_result().history(0).revision().DebugString() ==
                   revision.DebugString());
    TEST_CONDITION(result.list_result().history(0).status() ==
                   e8::ListRevisionHistoryResult::Revision::APPLIED);
    TEST_CONDITION(result.list_result().history(1).revision().DebugString() ==
                   revision2.DebugString());
    TEST_CONDITION(result.list_result().history(1).status() ==
                   e8::ListRevisionHistoryResult::Revision::PENDING);

    // Creates the second work, then we should see 1 applied revision and 1 work-in-progress
    // revision.
    e8::ClusterRevisionCommand create_work_command2;
    create_work_command2.set_resource_service_id("test");
    create_work_command2.mutable_create_work()->set_from_version_epoch(1);
    create_work_command2.mutable_create_work()->set_to_version_epoch(2);
    work_pool.Run(create_work_command2);

    result = work_pool.Run(list_history_command);
    TEST_CONDITION(result.has_list_result());
    TEST_CONDITION(result.list_result().history_size() == 2);
    TEST_CONDITION(result.list_result().history(0).revision().DebugString() ==
                   revision.DebugString());
    TEST_CONDITION(result.list_result().history(0).status() ==
                   e8::ListRevisionHistoryResult::Revision::APPLIED);
    TEST_CONDITION(result.list_result().history(1).revision().DebugString() ==
                   revision2.DebugString());
    TEST_CONDITION(result.list_result().history(1).status() ==
                   e8::ListRevisionHistoryResult::Revision::WORK_IN_PROGRESS);

    // Enqueues the third revision, then we should see1 applied revision, 1 work-in-progress
    // revision and 1 pending revision.
    e8::ClusterMapRevision revision3;
    revision3.set_from_version_epoch(2);
    revision3.set_to_version_epoch(3);
    *revision3.add_actions() =
        AddChildAction(/*child_label=*/"child2", /*machine_address=*/"192.168.0.2:1234");

    e8::ClusterRevisionCommand enqueue_command3;
    enqueue_command3.set_resource_service_id("test");
    *enqueue_command3.mutable_enqueue_revision()->mutable_revision() = revision3;
    work_pool.Run(enqueue_command3);

    result = work_pool.Run(list_history_command);
    TEST_CONDITION(result.has_list_result());
    TEST_CONDITION(result.list_result().history_size() == 3);
    TEST_CONDITION(result.list_result().history(0).revision().DebugString() ==
                   revision.DebugString());
    TEST_CONDITION(result.list_result().history(0).status() ==
                   e8::ListRevisionHistoryResult::Revision::APPLIED);
    TEST_CONDITION(result.list_result().history(1).revision().DebugString() ==
                   revision2.DebugString());
    TEST_CONDITION(result.list_result().history(1).status() ==
                   e8::ListRevisionHistoryResult::Revision::WORK_IN_PROGRESS);
    TEST_CONDITION(result.list_result().history(2).revision().DebugString() ==
                   revision3.DebugString());
    TEST_CONDITION(result.list_result().history(2).status() ==
                   e8::ListRevisionHistoryResult::Revision::PENDING);

    return true;
}

int main() {
    e8::BeginTestSuite("cluster_conductor_revision_store");
    e8::RunTest("PollResourceServiceTest", PollResourceServiceTest);
    e8::RunTest("EnqueueRejectionTest", EnqueueRejectionTest);
    e8::RunTest("EnqueueAndPollTest", EnqueueAndPollTest);
    e8::RunTest("EnqueueAndCreateWorkTest", EnqueueAndCreateWorkTest);
    e8::RunTest("EnqueueAndCreateWorkThenGetWorkTest", EnqueueAndCreateWorkThenGetWorkTest);
    e8::RunTest("EnqueueAndCreateWorkThenUpdateWorkTest", EnqueueAndCreateWorkThenUpdateWorkTest);
    e8::RunTest("EnqueueAndCreateWorkThenFinishWorkTest", EnqueueAndCreateWorkThenFinishWorkTest);
    e8::RunTest("ListHistoryTest", ListHistoryTest);
    e8::EndTestSuite();
    return 0;
}
