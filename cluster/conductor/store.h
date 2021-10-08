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

#ifndef CLUSTER_CONDUCTOR_STORE_H
#define CLUSTER_CONDUCTOR_STORE_H

#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "cluster/placement/cluster_map.h"
#include "common/random/random_source.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/cluster_revision_command.pb.h"

namespace e8 {

// A unique identifier referencing a specific resource service (e.g. "job_service", "file_system"
// or "message_queue"). Each resource service uses its own scope of cluster map.
using ResourceServiceId = std::string;

/**
 * @brief The ClusterRevisionStore class Stores a history of cluster and manages each
 * revision's status as well its transition for each resource service. By going through the revision
 * history, each resource service is able to recover the most up-to-date cluster map. In this sense,
 * this class is at least a replica of the source-of-truths cluster map.
 *
 * Each cluster revision is put into one of the three categories: applied, work-in-progress and
 * pending. This allows a worker class to poll from this class to apply the work-ini-progress
 * revision to the cluster while other clients are able to put new revisions to the pending category
 * without blocking. When a work-in-progress revision is applied, the pending revisions are merged
 * into one single revision and becomes work-in-progress.
 *
 * This class isn't thread safe.
 */
class ClusterRevisionStore {
  public:
    struct RevisionSpecs {
        RevisionSpecs(ResourceServiceId const &resource_service_id, ClusterMap const &cluster_map,
                      ClusterMapRevision const &revision);
        ~RevisionSpecs();

        // The resource service this revision is going to apply to.
        ResourceServiceId resource_service_id;

        // The cluster map the revision is based on.
        ClusterMap cluster_map;

        // The revision to apply to the cluster map.
        ClusterMapRevision revision;
    };

    ClusterRevisionStore();
    ~ClusterRevisionStore();

    /**
     * @brief Run Runs a cluster revision command and returns the result of the command. See the
     * ClusterRevisionCommand proto for what each command does.
     */
    ClusterRevisionResult Run(ClusterRevisionCommand const &command);

    /**
     * @brief WorkInProgress Fetches one of the currently work-in-progress revisions if there is
     * any. When more than one resource service has its work-in-progress revision, only one revision
     * is selected, randomly. So in that situation, this function isn't idempotent.
     */
    std::optional<RevisionSpecs> WorkInProgress() const;

  private:
    /**
     * @brief The ResourceServiceClusterState class Stores each resource service's cluster revision
     * states.
     */
    class ResourceServiceClusterState {
      public:
        ResourceServiceClusterState();
        ~ResourceServiceClusterState();

        /**
         * @brief Enqueue Adds/merges the specified revision to the pending slot. The revision must
         * not be a composite, meaning the the revision must only span 1 version of change. The
         * version of cluster map which it's going to revise from must be that with all the pending
         * & work-in-progress revisions applied. Otherwise, it returns with the field "successful"
         * set to false.
         */
        EnqueueClusterRevisionResult Enqueue(ClusterMapRevision const &revision);

        /**
         * @brief Apply Marks the specified revision as applied so that the pending revision
         * transitions to work-in-progress.
         */
        ApplyClusterRevisionResult Apply(ClusterMapRevision const &revision);

        /**
         * @brief Get Gets the revisions, if there is any, starting from the specified version
         * epoch. The epoch can not be newer than the newest revision ever enqueued. The scope
         * parameter indicates whether to include work-in-progress revision and pending revisions
         * that has not yet been worked on. The returned revisions are merged into one
         * redundancy-free revision object.
         */
        GetClusterRevisionResult Get(ClusterMapVersionEpoch starting_from_epoch,
                                     GetClusterRevisionCommand::Scope scope);

        /**
         * @brief HasWorkInProgress Checks if this resource service has any work-in-progress
         * revision.
         */
        bool HasWorkInProgress() const;

        /**
         * @brief WorkInProgress Gets the resource service's currently work-in-progress revision if
         * there is any. Otherwise, it return's the resource service's latest cluster map with an
         * std::nullopt revision.
         */
        std::pair<ClusterMap, std::optional<ClusterMapRevision>> WorkInProgress() const;

      private:
        ClusterMap cluster_map_;
        std::vector<ClusterMapRevision> all_revisions_;
        std::optional<ClusterMapRevision> wip_revision_;
        std::optional<ClusterMapRevision> pending_revision_;
    };

    std::unordered_map<ResourceServiceId, ResourceServiceClusterState> services_;
    std::unique_ptr<RandomSource> random_source_;
    std::unique_ptr<std::mutex> mu_;
};

} // namespace e8

#endif // CLUSTER_CONDUCTOR_STORE_H
