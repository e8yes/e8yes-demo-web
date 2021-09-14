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

#ifndef PLACEMENT_HIERARCHY_H
#define PLACEMENT_HIERARCHY_H

#include <memory>
#include <optional>
#include <unordered_map>

#include "cluster/placement/bucket.h"
#include "cluster/placement/common_types.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

/**
 * @brief The ClusterHierarchy class Supports a hierarchical (tree) description of a cluster. It
 * divides and groups machines by logical location elements (See ClusterTreeNode::Hierarchy for what
 * location elements it supports). This class isn't thread safe.
 */
class ClusterHierarchy {
  public:
    /**
     * @brief The BucketOrMachine struct Public data associated with a tree node in the hierarchy.
     */
    struct BucketOrMachine {
        BucketOrMachine(ClusterTreeNodeLabel const &parent_label,
                        ClusterTreeNode::Hierarchy hierarchy,
                        std::unique_ptr<BucketInterface> &&bucket);
        BucketOrMachine(ClusterTreeNodeLabel const &parent_label, Machine const &machine);

        // Label of the node's parent. If it's a root node, it's the same as the node's label.
        ClusterTreeNodeLabel parent_label;

        // The hierarchy the node belongs to.
        ClusterTreeNode::Hierarchy hierarchy;

        // It can have either a non-empty bucket or machine field.
        std::unique_ptr<BucketInterface> bucket;
        std::optional<Machine> machine;
    };

    /**
     * @brief ClusterHierarchy Constructs an empty cluster.
     */
    ClusterHierarchy();
    ~ClusterHierarchy();

    /**
     * @brief AddRoot Adds an internal root bucket node to the hierarchy. If a root node has already
     * existed in the hierarhcy, it will reject this addition.
     *
     * @param bucket The root bucket to be added.
     * @return true if the root doesn't exist and the root bucket is able to add to the hierarchy.
     */
    bool AddRoot(std::unique_ptr<BucketInterface> &&bucket);

    /**
     * @brief AddBucket Adds an internal bucket node to the hierarchy. A bucket node logically
     * groups child elements. The hardware capabilities of this node is the sum of the child
     * capabilities. If the specified node has been added in the class, it will rejects the
     * addition.
     *
     * @param parent_label he parent label of the the bucket node to be added. The parent must exist
     * prior the function call. It can't be an external machine node.
     * @param node_label The label of the bucket node to be added. Note, the label value "root" is
     * reserved.
     * @param hierarchy The hierarchy the bucket belongs to. It must have a more granular hierarchy
     * than its parent, or this function will fail.
     * @param bucket The bucket to be added. This class owns the bucket if the call is successful.
     * @return True if the bucket is added to the hierarchy.
     */
    bool AddBucket(ClusterTreeNodeLabel const &parent_label, ClusterTreeNodeLabel const &node_label,
                   ClusterTreeNode::Hierarchy hierarchy, std::unique_ptr<BucketInterface> &&bucket);

    /**
     * @brief AddMachine Adds an external machine node to the hierarchy. The machine's capabilities
     * will be added to all the parent buckets. If the specified node has been added in the class,
     * it will rejects the addition.
     *
     * @param parent_label The parent label of the machine node to be added. The parent must exist
     * prior to the function call, and it can't be an external machine node.
     * @param node_label The label of the machine node to be added. Note, the label value "root" is
     * reserved.
     * @param machine The machine proto describing the properties of the node.
     * @return True if the node is added to the hierarchy.
     */
    bool AddMachine(ClusterTreeNodeLabel const &parent_label,
                    ClusterTreeNodeLabel const &node_label, Machine const &machine);

    /**
     * @brief Remove Removes a node and the entire subtree from the hierarchy. It will propagate the
     * deduction in hardware capabilities to all the parent buckets. Also, it notifies the parent
     * bucket to remove the node from its child set. If the node doesn't exist, it will do nothing.
     *
     * @param node_label Label of the node to be removed.
     * @return true if the node and its subtree are removed.
     */
    bool Remove(ClusterTreeNodeLabel const &node_label);

    /**
     * @brief Root Returns a pointer to the root node if it exists. Otherwise, it returns a nullptr.
     */
    BucketOrMachine const *Root() const;

    /**
     * @brief Node Returns a pointer to the node keyed by the specified node_label. This function
     * will fail if the node doesn't exist.
     */
    BucketOrMachine const *Node(ClusterTreeNodeLabel const &node_label) const;

    /**
     * @brief NodeCount The total number of nodes in the hierarchy.
     */
    unsigned NodeCount() const;

  private:
    void UpdateAllParents(ClusterTreeNodeLabel const &node_label, BucketOrMachine const &node,
                          WeightedCapabilities const &delta);

    void RemoveSubtree(ClusterTreeNodeLabel const &node_label);

    std::unordered_map<ClusterTreeNodeLabel, std::unique_ptr<BucketOrMachine>> tree_;
};

} // namespace e8

#endif // PLACEMENT_HIERARCHY_H
