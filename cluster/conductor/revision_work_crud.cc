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

#include <google/protobuf/repeated_field.h>
#include <optional>
#include <vector>

#include "cluster/conductor/conductor.h"
#include "cluster/conductor/revision_work_crud.h"
#include "cluster/conductor/revision_work_pool.h"
#include "cluster/placement/cluster_map.h"
#include "common/random/random_source.h"
#include "common/random/uniform_distribution.h"
#include "proto_cc/cluster_revision.pb.h"
#include "proto_cc/cluster_revision_command.pb.h"

namespace e8 {
namespace {

void ExtractTargets(ClusterMapRevision const &all_revision,
                    ClusterMapRevision const &pending_revision,
                    google::protobuf::RepeatedPtrField<ClusterRevisionWork::Target> *targets) {
    ClusterMap cluster_map;
    cluster_map.Revise(all_revision);

    std::vector<ClusterMap::LabelAndMachine> existing_targets = cluster_map.Machines();

    for (auto const &action : pending_revision.actions()) {
        if (action.operation() == ClusterMapRevision::ADD_TREE_NODE &&
            action.tree_node().has_machine()) {

            ClusterRevisionWork::Target *new_target = targets->Add();
            new_target->set_node_label(action.node_label());
            *new_target->mutable_machine() = action.tree_node().machine();
        }
    }
}

bool PollResourceServicePendingRevision(ResourceServiceId const &resource_service_id,
                                        ClusterRevisionConductorInterface *revision_conductor,
                                        ClusterMapRevision *pending_revision,
                                        ClusterMapRevision *all_revision) {
    ClusterRevisionCommand poll_pending_revision_command;
    poll_pending_revision_command.set_resource_service_id(resource_service_id);
    *poll_pending_revision_command.mutable_poll_revision() = PollPendingClusterRevisionCommand();

    ClusterRevisionResult poll_pending_result =
        revision_conductor->RunCommand(poll_pending_revision_command);
    if (!poll_pending_result.poll_result().has_pending_revision()) {
        return false;
    }

    *pending_revision = poll_pending_result.poll_result().pending_revision();
    *all_revision = poll_pending_result.poll_result().all_revision();

    return true;
}

} // namespace

PollPendingRevisionResult::PollPendingRevisionResult(ResourceServiceId const &resource_service_id)
    : resource_service_id(resource_service_id) {}

std::optional<PollPendingRevisionResult>
PollPendingRevision(ClusterRevisionConductorInterface *revision_conductor,
                    RandomSource *random_source) {
    ClusterRevisionCommand poll_resource_service_command;
    *poll_resource_service_command.mutable_poll_resource_service() =
        PollClusterResourceServiceCommand();
    ClusterRevisionResult result = revision_conductor->RunCommand(poll_resource_service_command);

    google::protobuf::RepeatedPtrField<ResourceServiceId> candidate_resource_service =
        result.poll_resource_service_result().resource_services();

    // Randomly selects from the candidate list until we find a candidate resource service that has
    // a pending revision. We don't have to update the candidate list because it's ok that we'll get
    // a fresh list the next time this function gets called.
    while (!candidate_resource_service.empty()) {
        unsigned candidate_index =
            DrawUniformInts(/*lower_bound=*/0, /*upper_bound=*/candidate_resource_service.size(),
                            /*num_instances=*/1, random_source)[0];

        PollPendingRevisionResult result(candidate_resource_service[candidate_index]);
        bool has_pending = PollResourceServicePendingRevision(
            candidate_resource_service[candidate_index], revision_conductor,
            &result.pending_revision, &result.all_revision);
        if (has_pending) {
            // Found a resource service that has pending revision to work on.
            return result;
        }

        candidate_resource_service.erase(candidate_resource_service.begin() + candidate_index);
    }

    return std::nullopt;
}

bool CreateNewRevisionWork(ResourceServiceId const &resource_service_id,
                           ClusterRevisionConductorInterface *revision_conductor,
                           ClusterMapRevision *pending_revision, ClusterMapRevision *all_revision) {
    ClusterRevisionResult create_work_result;

    do {
        ClusterRevisionCommand create_work_command;
        create_work_command.set_resource_service_id(resource_service_id);
        create_work_command.mutable_create_work()->set_from_version_epoch(
            pending_revision->from_version_epoch());
        create_work_command.mutable_create_work()->set_to_version_epoch(
            pending_revision->to_version_epoch());
        ExtractTargets(*all_revision, *pending_revision,
                       create_work_command.mutable_create_work()->mutable_targets());

        create_work_result = revision_conductor->RunCommand(create_work_command);

        if (!create_work_result.create_work_result().has_pending()) {
            return false;
        }

        if (!create_work_result.create_work_result().pending_versions_mismatch()) {
            // Fixes the pending version by polling the latest pending revision again.
            bool has_pending = PollResourceServicePendingRevision(
                resource_service_id, revision_conductor, pending_revision, all_revision);
            if (!has_pending) {
                return false;
            }
        }
    } while (!create_work_result.create_work_result().successful());

    return true;
}

SelectRevisionWorkResult::SelectRevisionWorkResult(
    ResourceServiceId const &resource_service_id, ClusterRevisionWork const &selected_work,
    std::vector<ClusterMapRevision> const &all_revisions)
    : resource_service_id(resource_service_id), selected_work(selected_work),
      all_revisions(all_revisions) {}

std::optional<SelectRevisionWorkResult>
SelectRevisionWork(ClusterRevisionConductorInterface *revision_conductor,
                   RandomSource *random_source) {
    ClusterRevisionCommand poll_resource_service_command;
    *poll_resource_service_command.mutable_poll_resource_service() =
        PollClusterResourceServiceCommand();
    ClusterRevisionResult result = revision_conductor->RunCommand(poll_resource_service_command);

    google::protobuf::RepeatedPtrField<ResourceServiceId> candidate_resource_service =
        result.poll_resource_service_result().resource_services();

    // Randomly selects from the candidate list until we find a candidate resource service that has
    // at least a work-in-progress revision. If there are multiple work-in-progress revisions, we .
    // We don't have to update the candidate list because it's ok that we'll get a fresh list the
    // next time this function gets called.
    while (!candidate_resource_service.empty()) {
        unsigned candidate_index =
            DrawUniformInts(/*lower_bound=*/0, /*upper_bound=*/candidate_resource_service.size(),
                            /*num_instances=*/1, random_source)[0];

        ClusterRevisionCommand get_all_work_command;
        get_all_work_command.set_resource_service_id(candidate_resource_service[candidate_index]);
        *get_all_work_command.mutable_get_all_work() = GetAllClusterRevisionWorkCommand();

        ClusterRevisionResult work = revision_conductor->RunCommand(get_all_work_command);

        if (work.get_all_work_result().all_work().empty()) {
            candidate_resource_service.erase(candidate_resource_service.begin() + candidate_index);
            continue;
        }

        unsigned selected_work_index = DrawUniformInts(
            /*lower_bound=*/0, /*upper_bound=*/work.get_all_work_result().all_work().size(),
            /*num_instances=*/1, random_source)[0];
        ClusterRevisionWork const &selected_work =
            work.get_all_work_result().all_work(selected_work_index);

        std::vector<ClusterMapRevision> candidate_revision_history(
            {work.get_all_work_result().all_revisions().begin(),
             work.get_all_work_result().all_revisions().end()});

        return SelectRevisionWorkResult(candidate_resource_service[candidate_index], selected_work,
                                        candidate_revision_history);
    }

    return std::nullopt;
}

bool UpdateRevisionWork(ResourceServiceId const &resource_service_id,
                        ClusterRevisionWork const &work,
                        ClusterRevisionConductorInterface *revision_conductor) {
    ClusterRevisionCommand update_work_command;
    update_work_command.set_resource_service_id(resource_service_id);
    update_work_command.mutable_update_work()->set_machine_version_epoch(
        work.machine_version_epoch());
    *update_work_command.mutable_update_work()->mutable_targets() = work.targets();

    ClusterRevisionResult result = revision_conductor->RunCommand(update_work_command);

    return result.update_work_result().successful();
}

bool FinishRevisionWork(ResourceServiceId const &resource_service_id,
                        ClusterRevisionWork const &work,
                        ClusterRevisionConductorInterface *revision_conductor) {
    ClusterRevisionCommand finish_work_command;
    finish_work_command.set_resource_service_id(resource_service_id);
    finish_work_command.mutable_finish_work()->set_machine_version_epoch(
        work.machine_version_epoch());

    ClusterRevisionResult result = revision_conductor->RunCommand(finish_work_command);

    return result.finish_work_result().successful();
}

} // namespace e8
