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

#include <google/protobuf/map.h>
#include <memory>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <utility>

#include "cluster/placement/bucket.h"
#include "cluster/placement/capability.h"
#include "cluster/placement/common_types.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

/**
 * @brief AllocateClusterTreeNodeLabel Allocates a unique tree node label for addition of the node
 * to the cluster map.
 */
ClusterTreeNodeLabel AllocateClusterTreeNodeLabel();

/**
 * @brief The ClusterMap class Supports a hierarchical (tree) description of a cluster. It divides
 * and groups machines by logical location elements (See ClusterTreeNode::Hierarchy for what
 * location elements it supports). It allows, for example, a flexible  placement rule to find 3
 * replicas located in machines that are placed in the same room but in 3 different rows.
 */
class ClusterMap {
  public:
    /**
     * @brief ClusterMap Constructs an empty cluster.
     */
    ClusterMap();

    /**
     * @brief ClusterMap Recovers the cluster description from the specified proto.
     */
    ClusterMap(ClusterMapData const &cluster_map_data);

    ~ClusterMap();

    /**
     * @brief ToProto Exports the current cluster description to a proto.
     */
    ClusterMapData ToProto() const;

  private:
    struct BucketOrMachine {
        BucketOrMachine(ClusterTreeNode::Hierarchy hierarchy,
                        std::unique_ptr<BucketInterface> &&bucket);
        BucketOrMachine(ClusterTreeNode::Hierarchy hierarchy, Machine const &machine);

        ClusterTreeNode::Hierarchy hierarchy;
        std::unique_ptr<BucketInterface> bucket;
        std::optional<Machine> machine;
    };

    std::unordered_map<ClusterTreeNodeLabel, BucketOrMachine>::iterator
    ImportNode(ClusterTreeNodeLabel const &parent_label, ClusterTreeNodeLabel const &node_label,
               ClusterTreeNode const &node_proto);

    void
    ImportChildren(ClusterTreeNodeLabel const &parent_label, BucketOrMachine const &parent_node,
                   google::protobuf::Map<ClusterTreeNodeLabel, ClusterTreeNode> const &tree_proto);

    void ExportNode(ClusterTreeNodeLabel const &node_label,
                    google::protobuf::Map<ClusterTreeNodeLabel, ClusterTreeNode> *proto) const;

    ClusterMapVersionEpoch version_;
    ClusterCapability capabilities_;
    std::unordered_map<ClusterTreeNodeLabel, BucketOrMachine> tree_;
    std::unique_ptr<std::shared_mutex> mu_;
};

} // namespace e8

#endif // PLACEMENT_CLUSTER_MAP_H
