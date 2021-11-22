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
#include <cstdio>
#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cluster/conductor/revision_work_pool.h"
#include "cluster/placement/cluster_map.h"
#include "cluster/placement/common_types.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/cluster_revision.pb.h"
#include "proto_cc/cluster_revision_command.pb.h"
#include "proto_cc/cluster_revision_work_pool.pb.h"

namespace e8 {

std::optional<ClusterMapRevision>
MergeClusterMapRevisions(std::vector<ClusterMapRevision> const &revisions, unsigned start,
                         unsigned end) {
    assert(end <= revisions.size());
    assert(start <= end);

    if (start == end) {
        return std::nullopt;
    }

    // TODO: We could eleminate redundant operations.
    ClusterMapRevision merged;

    merged.set_from_version_epoch(revisions[start].from_version_epoch());
    merged.set_to_version_epoch(revisions[end - 1].to_version_epoch());

    for (unsigned i = start; i < end; ++i) {
        for (auto const &action : revisions[i].actions()) {
            *merged.mutable_actions()->Add() = action;
        }
    }

    return merged;
}

ClusterRevisionWorkPool::ResourceServiceClusterState::ResourceServiceClusterState() {}

ClusterRevisionWorkPool::ResourceServiceClusterState::~ResourceServiceClusterState() {}

EnqueueClusterRevisionResult
ClusterRevisionWorkPool::ResourceServiceClusterState::EnqueuePendingRevision(
    EnqueueClusterRevisionCommand const &command) {
    assert(command.revision().from_version_epoch() ==
           command.revision().to_version_epoch() - 1); // New revision can not be a composite.

    EnqueueClusterRevisionResult result;

    ClusterMapVersionEpoch last_epoch;
    if (all_revisions_.empty()) {
        assert(work_pool_.empty());
        assert(!pending_revision_.has_value());

        last_epoch = ClusterMapVersionEpoch();
    } else {
        ClusterMapRevision const &last_revision = *all_revisions_.rbegin();
        last_epoch = last_revision.to_version_epoch();
    }

    if (command.revision().from_version_epoch() != last_epoch) {
        // New revision purposal doesn't continue from the last enqueued revision.
        result.set_successful(false);
        return result;
    }

    all_revisions_.push_back(command.revision());

    if (!pending_revision_.has_value()) {
        // There is revision to be worked on, but there hasn't been any pending revision yet.
        // Schedules this revision to sit in the pending position.
        pending_revision_ = command.revision();

        result.set_successful(true);
        return result;
    }

    // There has been a pending revision already. Merges with the existing pending revision.
    std::optional<ClusterMapRevision> merged_pending_revision = MergeClusterMapRevisions(
        std::vector<ClusterMapRevision>{*pending_revision_, command.revision()}, /*start=*/0,
        /*end=*/2);
    assert(merged_pending_revision.has_value());
    pending_revision_ = *merged_pending_revision;

    result.set_successful(true);
    return result;
}

PollPendingClusterRevisionResult
ClusterRevisionWorkPool::ResourceServiceClusterState::PollPendingRevision(
    PollPendingClusterRevisionCommand const & /*command*/) {
    PollPendingClusterRevisionResult result;

    if (!pending_revision_.has_value()) {
        std::optional<ClusterMapRevision> applied_and_wip =
            MergeClusterMapRevisions(all_revisions_, /*start=*/0, all_revisions_.size());
        if (applied_and_wip.has_value()) {
            *result.mutable_all_revision() = *applied_and_wip;
        }

        return result;
    }

    *result.mutable_pending_revision() = *pending_revision_;

    std::optional<ClusterMapRevision> applied_and_wip = MergeClusterMapRevisions(
        all_revisions_, /*start=*/0, pending_revision_->from_version_epoch());
    if (applied_and_wip.has_value()) {
        *result.mutable_all_revision() = *applied_and_wip;
    }

    return result;
}

CreateClusterRevisionWorkResult ClusterRevisionWorkPool::ResourceServiceClusterState::CreateWork(
    CreateClusterRevisionWorkCommand const &command) {
    CreateClusterRevisionWorkResult result;

    if (!pending_revision_.has_value()) {
        result.set_successful(false);
        result.set_has_pending(false);
        return result;
    }

    result.set_has_pending(true);

    if (command.from_version_epoch() != pending_revision_->from_version_epoch() ||
        command.to_version_epoch() != pending_revision_->to_version_epoch()) {
        result.set_successful(false);
        result.set_pending_versions_mismatch(true);
        return result;
    }

    ClusterRevisionWork new_work;
    new_work.set_machine_version_epoch(pending_revision_->from_version_epoch());
    *new_work.mutable_targets() = command.targets();

    bool added_to_pool =
        work_pool_.insert(std::make_pair(new_work.machine_version_epoch(), new_work)).second;
    assert(added_to_pool); // The work must not have existed in the pool, or else the command's
                           // version epochs could not have matched with the pending revision's.

    pending_revision_.reset();

    result.set_successful(true);
    return result;
}

GetAllClusterRevisionWorkResult ClusterRevisionWorkPool::ResourceServiceClusterState::GetAllWork(
    GetAllClusterRevisionWorkCommand const & /*command*/) {
    GetAllClusterRevisionWorkResult result;
    for (auto const &revision : all_revisions_) {
        *result.add_all_revisions() = revision;
    }

    for (auto const &[_, work] : work_pool_) {
        *result.add_all_work() = work;
    }

    return result;
}

UpdateClusterRevisionWorkResult ClusterRevisionWorkPool::ResourceServiceClusterState::UpdateWork(
    UpdateClusterRevisionWorkCommand const &command) {
    assert(!command.targets().empty()); // If there is no target machine, the client should
                                        // have used the FinishClusterRevisionWorkCommand.

    UpdateClusterRevisionWorkResult result;

    auto work_it = work_pool_.find(command.machine_version_epoch());
    if (work_it == work_pool_.end()) {
        result.set_successful(false);
        return result;
    }

    auto &[_, work] = *work_it;
    *work.mutable_targets() = command.targets();

    result.set_successful(true);
    return result;
}

FinishClusterRevisionWorkResult ClusterRevisionWorkPool::ResourceServiceClusterState::FinishWork(
    FinishClusterRevisionWorkCommand const &command) {
    unsigned num_removed = work_pool_.erase(command.machine_version_epoch());

    FinishClusterRevisionWorkResult result;
    result.set_successful(num_removed == 1);

    return result;
}

ListRevisionHistoryResult ClusterRevisionWorkPool::ResourceServiceClusterState::ListHistory(
    ListRevisionHistoryCommand const & /*command*/) {
    ListRevisionHistoryResult result;

    // Figures out the index ranges where work-in-progress and pending revivions span across the
    // revision history. The rest of the entries are then applied revisions.
    int pending_range_begin;
    int pending_range_end;
    if (pending_revision_.has_value()) {
        pending_range_begin = pending_revision_->from_version_epoch();
        pending_range_end = pending_revision_->to_version_epoch() - 1;
    } else {
        pending_range_begin = all_revisions_.size();
        pending_range_end = all_revisions_.size();
    }

    int wip_range_begin;
    int wip_range_end;
    if (!work_pool_.empty()) {
        wip_range_begin = work_pool_.begin()->first;
        wip_range_end = pending_range_begin - 1;
    } else {
        wip_range_begin = -1;
        wip_range_end = -1;
    }

    assert(wip_range_begin <= wip_range_end);
    assert(pending_range_begin <= pending_range_end);
    assert(wip_range_end < pending_range_begin);

    // Annotates each revision entry's status according the above ranges.
    for (int i = 0; i < static_cast<int>(all_revisions_.size()); ++i) {
        ListRevisionHistoryResult::Revision *revision = result.add_history();

        *revision->mutable_revision() = all_revisions_[i];

        if (wip_range_begin <= i && i <= wip_range_end) {
            revision->set_status(ListRevisionHistoryResult::Revision::WORK_IN_PROGRESS);
        } else if (pending_range_begin <= i && i <= pending_range_end) {
            revision->set_status(ListRevisionHistoryResult::Revision::PENDING);
        } else {
            revision->set_status(ListRevisionHistoryResult::Revision::APPLIED);
        }
    }

    return result;
}

ResourceServiceClusterStateData
ClusterRevisionWorkPool::ResourceServiceClusterState::ToProto() const {
    ResourceServiceClusterStateData proto;

    for (auto const &revision : all_revisions_) {
        *proto.add_all_revisions() = revision;
    }
    for (auto const &[revision_epoch, work] : work_pool_) {
        (*proto.mutable_work_pool())[revision_epoch] = work;
    }
    if (pending_revision_.has_value()) {
        *proto.mutable_pending_revision() = *pending_revision_;
    }

    return proto;
}

void ClusterRevisionWorkPool::ResourceServiceClusterState::FromProto(
    ResourceServiceClusterStateData const &proto) {
    all_revisions_.clear();
    work_pool_.clear();
    pending_revision_.reset();

    for (auto const &revision : proto.all_revisions()) {
        all_revisions_.push_back(revision);
    }
    for (auto const &[revision_epoch, work] : proto.work_pool()) {
        work_pool_[revision_epoch] = work;
    }
    if (proto.has_pending_revision()) {
        pending_revision_ = proto.pending_revision();
    }
}

ClusterRevisionWorkPool::ClusterRevisionWorkPool(std::string const &snapshot_file)
    : snapshot_file_(snapshot_file), state_changed_(false) {}

ClusterRevisionWorkPool::~ClusterRevisionWorkPool() {}

ClusterRevisionResult ClusterRevisionWorkPool::Run(ClusterRevisionCommand const &command) {
    if (command.has_poll_resource_service()) {
        ClusterRevisionResult result;
        *result.mutable_poll_resource_service_result() = PollClusterResourceServiceResult();

        for (auto const &[resource_service_id, _] : services_) {
            result.mutable_poll_resource_service_result()->add_resource_services(
                resource_service_id);
        }

        return result;
    }

    auto service_it = services_.find(command.resource_service_id());
    if (service_it == services_.end()) {
        service_it = services_
                         .insert(std::make_pair(command.resource_service_id(),
                                                ResourceServiceClusterState()))
                         .first;
    }

    auto &[_, service_states] = *service_it;

    ClusterRevisionResult result;
    switch (command.command_case()) {
    case ClusterRevisionCommand::CommandCase::kEnqueueRevision: {
        *result.mutable_enqueue_result() =
            service_states.EnqueuePendingRevision(command.enqueue_revision());
        state_changed_ |= result.enqueue_result().successful();
        break;
    }
    case ClusterRevisionCommand::CommandCase::kPollRevision: {
        *result.mutable_poll_result() = service_states.PollPendingRevision(command.poll_revision());
        break;
    }
    case ClusterRevisionCommand::CommandCase::kCreateWork: {
        *result.mutable_create_work_result() = service_states.CreateWork(command.create_work());
        state_changed_ |= result.create_work_result().successful();
        break;
    }
    case ClusterRevisionCommand::CommandCase::kGetAllWork: {
        *result.mutable_get_all_work_result() = service_states.GetAllWork(command.get_all_work());
        break;
    }
    case ClusterRevisionCommand::CommandCase::kUpdateWork: {
        *result.mutable_update_work_result() = service_states.UpdateWork(command.update_work());
        state_changed_ |= result.update_work_result().successful();
        break;
    }
    case ClusterRevisionCommand::CommandCase::kFinishWork: {
        *result.mutable_finish_work_result() = service_states.FinishWork(command.finish_work());
        state_changed_ |= result.finish_work_result().successful();
        break;
    }
    case ClusterRevisionCommand::CommandCase::kListRevisionHistory: {
        *result.mutable_list_result() = service_states.ListHistory(command.list_revision_history());
        break;
    }
    default: {
        assert(false);
    }
    }

    return result;
}

void ClusterRevisionWorkPool::Save() {
    if (!state_changed_) {
        return;
    }

    ClusterRevisionWorkPoolData proto;
    for (auto const &[service_id, service_state] : services_) {
        (*proto.mutable_services())[service_id] = service_state.ToProto();
    }

    std::fstream f;
    f.open(snapshot_file_, std::ios::out | std::ios::binary | std::ios::trunc);
    assert(f.is_open());

    proto.SerializeToOstream(&f);

    f.close();
}

void ClusterRevisionWorkPool::Restore() {
    ClusterRevisionWorkPoolData data;

    std::fstream f;
    f.open(snapshot_file_, std::ios::in | std::ios::binary);
    assert(f.is_open());

    data.ParseFromIstream(&f);

    f.close();

    for (auto const &[service_id, service_state_proto] : data.services()) {
        ResourceServiceClusterState service_state;
        service_state.FromProto(service_state_proto);
        services_.insert(std::make_pair(service_id, service_state));
    }
}

} // namespace e8
