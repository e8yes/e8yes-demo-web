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
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "cluster/conductor/revision_store.h"
#include "cluster/placement/cluster_map.h"
#include "cluster/placement/common_types.h"
#include "common/random/random_source.h"
#include "common/random/sample.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/cluster_revision_command.pb.h"

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

ClusterRevisionStore::ClusterRevisionStore()
    : random_source_(std::make_unique<RandomSource>()), mu_(std::make_unique<std::mutex>()) {}

ClusterRevisionStore::~ClusterRevisionStore() {}

ClusterRevisionStore::RevisionSpecs::RevisionSpecs(ResourceServiceId const &resource_service_id,
                                                   ClusterMap const &cluster_map,
                                                   std::vector<ClusterMapRevision> const &revisions,
                                                   ClusterMapVersionEpoch wip_starting_version)
    : resource_service_id(resource_service_id), cluster_map(cluster_map), revisions(revisions),
      wip_from_version_epoch(wip_starting_version) {}

ClusterRevisionStore::RevisionSpecs::~RevisionSpecs() {}

ClusterMapRevision ClusterRevisionStore::RevisionSpecs::WipRevision() const {
    std::optional<ClusterMapRevision> merged =
        MergeClusterMapRevisions(revisions, wip_from_version_epoch, revisions.size());
    assert(merged.has_value());

    return *merged;
}

ClusterRevisionStore::ResourceServiceClusterState::ResourceServiceClusterState(
    ResourceServiceId const &resource_service_id)
    : resource_service_id_(resource_service_id) {}

ClusterRevisionStore::ResourceServiceClusterState::~ResourceServiceClusterState() {}

EnqueueClusterRevisionResult
ClusterRevisionStore::ResourceServiceClusterState::Enqueue(ClusterMapRevision const &revision) {
    assert(revision.from_version_epoch() ==
           revision.to_version_epoch() - 1); // New revision can not be a composite.

    EnqueueClusterRevisionResult result;

    ClusterMapVersionEpoch last_epoch;
    if (all_revisions_.empty()) {
        assert(!wip_revision_.has_value());
        assert(!pending_revision_.has_value());

        last_epoch = ClusterMapVersionEpoch();
    } else {
        ClusterMapRevision const &last_revision = *all_revisions_.rbegin();
        last_epoch = last_revision.to_version_epoch();
    }

    if (revision.from_version_epoch() != last_epoch) {
        // New revision purposal doesn't continue from the last enqueued revision.
        result.set_successful(false);
        return result;
    }

    all_revisions_.push_back(revision);

    if (!wip_revision_.has_value()) {
        // Nothing is currently work-in-progress. Schedules this revision to be worked on
        // (boardcasted to the cluster).
        wip_revision_ = revision;

        result.set_successful(true);
        return result;
    }

    if (!pending_revision_.has_value()) {
        // There is revision to be worked on, but there hasn't been any pending revision yet.
        // Schedules this revision to sit in the pending position.
        pending_revision_ = revision;

        result.set_successful(true);
        return result;
    }

    // There has been a pending revision already. Merges with the existing pending revision.
    std::optional<ClusterMapRevision> merged_pending_revision = MergeClusterMapRevisions(
        std::vector<ClusterMapRevision>{*pending_revision_, revision}, /*start=*/0, /*end=*/2);
    assert(merged_pending_revision.has_value());
    pending_revision_ = *merged_pending_revision;

    result.set_successful(true);
    return result;
}

ApplyClusterRevisionResult
ClusterRevisionStore::ResourceServiceClusterState::Apply(ClusterMapRevision const &revision) {
    assert(revision.from_version_epoch() <= revision.to_version_epoch());

    ApplyClusterRevisionResult result;

    if (!wip_revision_.has_value()) {
        result.set_successful(false);
        return result;
    }

    if (revision.from_version_epoch() != wip_revision_->from_version_epoch() ||
        revision.to_version_epoch() != wip_revision_->to_version_epoch()) {
        assert(wip_revision_->from_version_epoch() >= revision.to_version_epoch());
        result.set_successful(false);
        return result;
    }

    bool successful = cluster_map_.Revise(revision);
    assert(successful);

    wip_revision_ = pending_revision_;
    pending_revision_.reset();

    result.set_successful(true);
    return result;
}

GetClusterRevisionResult
ClusterRevisionStore::ResourceServiceClusterState::Get(ClusterMapVersionEpoch starting_from_epoch,
                                                       GetClusterRevisionCommand::Scope scope) {
    assert(starting_from_epoch >= 1);
    assert(static_cast<unsigned>(starting_from_epoch) - 1 <= all_revisions_.size());

    GetClusterRevisionResult result;

    unsigned end;
    switch (scope) {
    case GetClusterRevisionCommand::APPLIED: {
        if (wip_revision_.has_value()) {
            end = wip_revision_->from_version_epoch();
        } else {
            end = all_revisions_.size();
        }
        break;
    }
    case GetClusterRevisionCommand::APPLIED_WIP: {
        if (wip_revision_.has_value()) {
            end = wip_revision_->to_version_epoch();
        } else {
            end = all_revisions_.size();
        }
        break;
    }
    case GetClusterRevisionCommand::APPLIED_WIP_PENDING: {
        end = all_revisions_.size();
        break;
    }
    default: {
        assert(false);
    }
    }

    std::optional<ClusterMapRevision> merged =
        MergeClusterMapRevisions(all_revisions_, starting_from_epoch - 1, end);
    if (merged.has_value()) {
        *result.mutable_revision() = *merged;
    }

    return result;
}

bool ClusterRevisionStore::ResourceServiceClusterState::HasWorkInProgress() const {
    return wip_revision_.has_value();
}

ClusterRevisionStore::RevisionSpecs
ClusterRevisionStore::ResourceServiceClusterState::WorkInProgress() const {
    assert(wip_revision_.has_value());

    std::vector<ClusterMapRevision> revision_of_interest(
        all_revisions_.begin(), all_revisions_.begin() + wip_revision_->to_version_epoch());

    return RevisionSpecs(resource_service_id_, cluster_map_, revision_of_interest,
                         wip_revision_->from_version_epoch());
}

ClusterRevisionResult ClusterRevisionStore::Run(ClusterRevisionCommand const &command) {
    std::lock_guard<std::mutex> guard(*mu_);

    auto service_it = services_.find(command.resource_service_id());
    if (service_it == services_.end()) {
        service_it =
            services_
                .insert(std::make_pair(command.resource_service_id(),
                                       ResourceServiceClusterState(command.resource_service_id())))
                .first;
    }

    auto &[_, service_states] = *service_it;

    ClusterRevisionResult result;
    switch (command.command_case()) {
    case ClusterRevisionCommand::CommandCase::kEnqueueRevision: {
        *result.mutable_enqueue_result() =
            service_states.Enqueue(command.enqueue_revision().revision());
        break;
    }
    case ClusterRevisionCommand::CommandCase::kApplyRevision: {
        *result.mutable_apply_result() = service_states.Apply(command.apply_revision().revision());
        break;
    }
    case ClusterRevisionCommand::CommandCase::kGetRevision: {
        *result.mutable_get_result() = service_states.Get(
            command.get_revision().starting_from_epoch(), command.get_revision().scope());
        break;
    }
    default: {
        assert(false);
    }
    }

    return result;
}

std::optional<ClusterRevisionStore::RevisionSpecs> ClusterRevisionStore::WorkInProgress() const {
    std::lock_guard<std::mutex> guard(*mu_);

    std::unordered_map<ResourceServiceId, float> weights;
    for (auto const &[resource_service_id, states] : services_) {
        if (states.HasWorkInProgress()) {
            weights.insert(std::make_pair(resource_service_id, 0.0f));
        }
    }

    if (weights.empty()) {
        // None of the services.
        return std::nullopt;
    }

    for (auto &[_, weight] : weights) {
        weight = 1.0f / weights.size();
    }

    ResourceServiceId selected_service = SampleFrom(weights, random_source_.get());
    return services_.at(selected_service).WorkInProgress();
}

} // namespace e8
