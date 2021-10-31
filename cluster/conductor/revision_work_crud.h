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

#ifndef CLUSTER_CONDUCTOR_REVISION_WORK_CRUD_H
#define CLUSTER_CONDUCTOR_REVISION_WORK_CRUD_H

#include <optional>
#include <vector>

#include "cluster/conductor/conductor.h"
#include "cluster/conductor/revision_work_pool.h"
#include "common/random/random_source.h"
#include "proto_cc/cluster_revision.pb.h"

namespace e8 {

/**
 * @brief The PollPendingRevisionResult struct Return values from the PollPendingRevision() call.
 */
struct PollPendingRevisionResult {
    /**
     * @brief PollPendingRevisionResult Do not construct this structure. Use the function
     * PollPendingRevision() to create it.
     */
    PollPendingRevisionResult(ResourceServiceId const &resource_service_id);

    // ID of the resource service which gets selected.
    ResourceServiceId resource_service_id;

    // The currently pending revision from the resource service.
    ClusterMapRevision pending_revision;

    // The merged revision history containing the applied and work-in-progress revisions.
    ClusterMapRevision all_revision;
};

/**
 * @brief PollPendingRevision Randomly selects from one of the resource services that has a pending
 * revision and polls from it. See the structure PollPendingRevisionResult for what this function
 * returns. If none of the resource services have a pending revision or the revision conductor is no
 * longer an active boardcastor, it returns a std::nullopt.
 */
std::optional<PollPendingRevisionResult>
PollPendingRevision(ClusterRevisionConductorInterface *revision_conductor,
                    RandomSource *random_source);

/**
 * @brief CreateNewRevisionWork Creates a new piece of revision work on the specified resource
 * service. It determines the revision work's machine targets from the revision history and the
 * pending revision. If these sources aren't up-to-date, it will update them and retry.
 *
 * @param resource_service_id ID of the resource service where the new work will be created.
 * @param pending_revision The client of this function should supply the initial pending revision to
 * form the work on. This function will keep it up-to-date during the work creation.
 * @param all_revision The client of this function should supply the initial revision history for
 * determining the target machines. This function will keep the history up-to-date during the work
 * creation.
 * @return It returns true when a new piece of work could be created on the resource service. It
 * returns false, when there's no pending revision to create work from or the revision conductor is
 * no longer an active boardcaster.
 */
bool CreateNewRevisionWork(ResourceServiceId const &resource_service_id,
                           ClusterRevisionConductorInterface *revision_conductor,
                           ClusterMapRevision *pending_revision, ClusterMapRevision *all_revision);

/**
 * @brief The SelectWorkResult struct Return values from the SelectRevisionWork() call.
 */
struct SelectRevisionWorkResult {
    /**
     * @brief SelectRevisionWorkResult Do not construct this structure. Use the function
     * SelectRevisionWork() to create it.
     */
    SelectRevisionWorkResult(ResourceServiceId const &resource_service_id,
                             ClusterRevisionWork const &selected_work,
                             std::vector<ClusterMapRevision> const &all_revisions);

    // ID of the resource service which gets selected.
    ResourceServiceId resource_service_id;

    // The piece of work selected from the resource serviec.
    ClusterRevisionWork selected_work;

    // The revision history containing the applied and work-in-progress revisions.
    std::vector<ClusterMapRevision> all_revisions;
};

/**
 * @brief SelectRevisionWork Randomly selects from one of the resource services that has at least
 * one piece of revision work. Then, it random selects a piece of revision work if there are
 * multiple. See the structure SelectRevisionWorkResult for what this function returns. If none of
 * the resource services have revision work or the revision conductor is no longer an active
 * boardcastor, it returns a std::nullopt.
 */
std::optional<SelectRevisionWorkResult>
SelectRevisionWork(ClusterRevisionConductorInterface *revision_conductor,
                   RandomSource *random_source);

/**
 * @brief UpdateRevisionWork Updates a piece of revision work with different machine targets.
 *
 * @param resource_service_id ID of the resource service the revision work belongs to.
 * @param work The revision work with new machine targets.
 * @return True if the revision work exists and the revision conductor is currently an active
 * boardcastor.
 */
bool UpdateRevisionWork(ResourceServiceId const &resource_service_id,
                        ClusterRevisionWork const &work,
                        ClusterRevisionConductorInterface *revision_conductor);

/**
 * @brief FinishRevisionWork Marks a piece of revision work as finish.
 *
 * @param resource_service_id ID of the resource service the revision work belongs to.
 * @param work The revision work to be marked as finish.
 * @return True if the revision work exists and the revision conductor is currently an active
 * boardcastor.
 */
bool FinishRevisionWork(ResourceServiceId const &resource_service_id,
                        ClusterRevisionWork const &work,
                        ClusterRevisionConductorInterface *revision_conductor);

} // namespace e8

#endif // CLUSTER_CONDUCTOR_REVISION_WORK_CRUD_H
