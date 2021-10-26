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

#ifndef CLUSTER_CONDUCTOR_REVISION_WORK_POOL_H
#define CLUSTER_CONDUCTOR_REVISION_WORK_POOL_H

#include <google/protobuf/repeated_field.h>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

#include "cluster/placement/cluster_map.h"
#include "cluster/placement/common_types.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/cluster_revision.pb.h"
#include "proto_cc/cluster_revision_command.pb.h"

namespace e8 {

// A unique identifier referencing a specific resource service (e.g. "job_service", "file_system"
// or "message_queue"). Each resource service uses its own scope of cluster map.
using ResourceServiceId = std::string;

/**
 * @brief The ClusterRevisionWorkPool class Manages the state transition of the cluster map
 * revisions. It responds to commands that modify and query the states of cluster map revision work.
 * This lays the foundation for a set of conductor nodes to apply revision to the cluster in a
 * fault-tolerant manner (as a command runner).
 *
 * The lifecyle of each cluster map revision goes as follow:
 *
 *             ┌────────────────► client prepares revision
 *             │                              │
 *             │                              │
 * based on outdated cluster map           enqueue
 *             │                              │
 *             │                     ┌────────▼───────┐
 *             │                     │                │
 *             └─────────────────────┤    pending     │
 *                                   │                │
 *                                   └────────┬───────┘
 *                                            │
 *                                          poll
 *                                            │
 *                                            ▼
 *                              conductor assembles work data◄─────────machine targets
 *                                            ┼
 *                                       create work
 *                                            │
 *                                 ┌──────────▼───────────┐
 *                                 │                      │
 *                                 │   work in progress   ◄─────────────┐
 *                                 │                      │             │
 *                                 └──────────┬───────────┘             │
 *                                            │            update work on machine failures
 *                                        get work                      │
 *                                            │                         │
 *                                            ▼                         │
 *                                   conductor does work────────────────┘
 *                                            ┼
 *                                       finish work
 *                                            │
 *                                      ┌─────▼─────┐
 *                                      │           │
 *                                      │  Applied  │
 *                                      │           │
 *                                      └───────────┘
 */
class ClusterRevisionWorkPool {
  public:
    ClusterRevisionWorkPool();
    ~ClusterRevisionWorkPool();

    /**
     * @brief Run Runs a cluster revision command and returns the result of the command. See the
     * ClusterRevisionCommand proto for what each command does. Note, states are resource service
     * independent.
     */
    ClusterRevisionResult Run(ClusterRevisionCommand const &command);

  private:
    /**
     * @brief The ResourceServiceClusterState class Stores each resource service's cluster
     * revision states.
     */
    class ResourceServiceClusterState {
      public:
        ResourceServiceClusterState();
        ~ResourceServiceClusterState();

        /**
         * @brief Enqueue Adds the specified revision to the pending queue. The revision must not be
         * a composite, meaning the the revision must only span 1 version of change. The version of
         * cluster map which it's going to revise from must be that with all the pending &
         * work-in-progress revisions applied. Otherwise, the command will fail.
         */
        EnqueueClusterRevisionResult
        EnqueuePendingRevision(EnqueueClusterRevisionCommand const &command);

        /**
         * @brief PollPendingRevision Polls all the currently pending revisions.
         */
        PollPendingClusterRevisionResult
        PollPendingRevision(PollPendingClusterRevisionCommand const &command);

        /**
         * @brief CreateWork Creates new work to apply all the pending revisions to the specified
         * target machines. Then, empties the pending queue if the work could be successfully
         * created.
         */
        CreateClusterRevisionWorkResult CreateWork(CreateClusterRevisionWorkCommand const &command);

        /**
         * @brief GetAllWork Gets all the currently ongoing revision work.
         */
        GetAllClusterRevisionWorkResult GetAllWork(GetAllClusterRevisionWorkCommand const &command);

        /**
         * @brief UpdateWork Updates a piece of revision work with a new set of target machines.
         */
        UpdateClusterRevisionWorkResult UpdateWork(UpdateClusterRevisionWorkCommand const &command);

        /**
         * @brief FinishWork Marks a piece of revision work as finished and removes it from the
         * ongoing work pool.
         */
        FinishClusterRevisionWorkResult FinishWork(FinishClusterRevisionWorkCommand const &command);

        /**
         * @brief ListHistory Returns a history list of all revisions.
         */
        ListRevisionHistoryResult ListHistory(ListRevisionHistoryCommand const &command);

      private:
        google::protobuf::RepeatedPtrField<ClusterMapRevision> all_revisions_;
        std::map<ClusterMapVersionEpoch, ClusterRevisionWork> work_pool_;
        std::optional<ClusterMapRevision> pending_revision_;
    };

    std::unordered_map<ResourceServiceId, ResourceServiceClusterState> services_;
};

} // namespace e8

#endif // CLUSTER_CONDUCTOR_REVISION_WORK_POOL_H
