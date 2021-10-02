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

#ifndef PLACEMENT_CLUSTER_MAP_H
#define PLACEMENT_CLUSTER_MAP_H

#include <vector>

#include "cluster/placement/bucket.h"
#include "cluster/placement/common_types.h"
#include "cluster/placement/hierarchy.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

/**
 * @brief AllocateClusterTreeNodeLabel Allocates a unique tree node label for addition of the node
 * to the cluster map.
 */
ClusterTreeNodeLabel AllocateClusterTreeNodeLabel();

/**
 * @brief The ClusterMap class Manages cluster map revision, versioning, resource placement and
 * proto import/export. This class isn't thread safe.
 */
class ClusterMap {
  public:
    /**
     * @brief The Placement class Accepts custom placement rules and determines resource placements.
     */
    class Placement {
      public:
        /**
         * @brief Placement The object should be constructed by calling the
         * ClusterMap::TakeRootFor() function.
         */
        Placement(ResourceDescriptor const &resource, ClusterHierarchy const &hierarchy);
        ~Placement();

        /**
         * @brief Select Selects child items from the buckets produced from the previous Select()
         * call, if there is any. Otherwise, it begins the selection from the root. Each hierarchy
         * can only be selected once and the last selection must end at the machine hierarchy. By
         * using a sequence of Select() calls, the client is able to specify a flexible placement
         * rule. For example, one is able to place the resource with 3 replicas located in machines
         * that are placed in the same room but in 3 different rows.
         *
         * @param hierarchy The hierarchy to select at.
         * @param num_items The number of items to select at the current hierarchy.
         */
        Placement &Select(ClusterTreeNode::Hierarchy hierarchy, unsigned num_items);

        /**
         * @brief Emit Emits all the selected machines in descreasing rank if the selection didn't
         * result in error. Otherwise, it returns an empty list. The number of machines in the list
         * is the product of item counts in all previous Select() calls. Notes, the last Select()
         * call must select at the machine hierarchy, or this function will fail.
         */
        std::vector<Machine> Emit() const;

      private:
        bool DescentFrom(BucketInterface const &bucket, ClusterTreeNode::Hierarchy hierarchy,
                         ResourceDescriptor const &resource, ReplicationRank rank,
                         std::vector<ClusterHierarchy::BucketOrMachine const *> *sink);

        ResourceDescriptor const resource_;
        ClusterHierarchy const &hierarchy_;

        std::vector<ClusterHierarchy::BucketOrMachine const *> source_;
        std::vector<ClusterHierarchy::BucketOrMachine const *> sink_;

        bool error_;
    };

    /**
     * @brief ClusterMap Constructs an empty cluster.
     */
    ClusterMap();

    /**
     * @brief ClusterMap Recovers the cluster description from the specified proto.
     */
    ClusterMap(ClusterMapData const &cluster_map_data);

    /**
     * @brief ClusterMap Copy constructor.
     */
    ClusterMap(ClusterMap const &other);

    ~ClusterMap();

    /**
     * @brief Version The current cluster map version.
     */
    ClusterMapVersionEpoch Version() const;

    /**
     * @brief Revise Applies revision to the cluster map. If the revision isn't based on the current
     * version of the cluster map, it returns false. Otherwise, it applies actions from the revision
     * object to upgrade the cluster map.
     */
    bool Revise(ClusterMapRevision const &revision);

    /**
     * @brief Machines Returns all the machines there are in the cluster map.
     */
    std::vector<Machine> Machines() const;

    /**
     * @brief TakeRoot Constructs a placement object for the specified resource given the current
     * cluster map. The cluster map object must live longer than the returned placement object.
     */
    Placement TakeRootFor(ResourceDescriptor const &resource) const;

    /**
     * @brief ToProto Exports the current cluster description to a proto.
     */
    ClusterMapData ToProto() const;

  private:
    ClusterMapVersionEpoch version_;
    ClusterHierarchy hierarchy_;
};

} // namespace e8

#endif // PLACEMENT_CLUSTER_MAP_H
