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

#ifndef PLACEMENT_CAPABILITY_H
#define PLACEMENT_CAPABILITY_H

#include <unordered_map>

#include "cluster/placement/common_types.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

/**
 * @brief The ClusterCapability class Manages capability weights of both the logical bucket elements
 * and physical machines in a cluster. Bucket serves as a logical grouping mechanism to structure a
 * cluster in a hierarhical way. Therefore, changing any part of a hierarchy can potentially affects
 * the capability weights of other elements. This class propagates the change automatically for the
 * client. This class isn't thread-safe.
 */
class ClusterCapability {
  public:
    /**
     * @brief ClusterCapability A hierarchical cluster must have a root bucket. This class requires
     * the label of the root bucket on construction. In the subsequent operations, the client won't
     * be allowed set any root bucket to this class, so the root bucket is final by the end of the
     * construction.
     */
    explicit ClusterCapability(ClusterTreeNodeLabel const &root);
    ~ClusterCapability();

    /**
     * @brief AddBucket Adds an internal bucket node that logically groups child capabilities. The
     * capability of this node is the sum of the child capabilities. If the specified node has been
     * added in the class, it will rejects the addition.
     *
     * @param parent_label The parent label of the the bucket node to be added. The parent must
     * exist prior the function call, and it can't be an external machine node.
     * @param node_label The label of the bucket node to be added.
     * @return True if the node doesn't exist in the class.
     */
    bool AddBucket(ClusterTreeNodeLabel const &parent_label,
                   ClusterTreeNodeLabel const &node_label);

    /**
     * @brief AddMachine Adds an external machine node that describes the capabilities of a
     * specific machine. The capabilities will be added to all the parent buckets. If the specified
     * node has been added in the class, it will rejects the addition.
     *
     * @param parent_label The parent label of the machine node to be added. The parent must exist
     * prior to the function call, and it can't be an external machine node.
     * @param node_label The label of the machine node to be added.
     * @param capabilities The capability of the machine.
     * @return True if the node doesn't exist in the class.
     */
    bool AddMachine(ClusterTreeNodeLabel const &parent_label,
                    ClusterTreeNodeLabel const &node_label,
                    WeightedCapabilities const &capabilities);

    /**
     * @brief Remove Removes a node and its entire subtree from the class. It will propagate the
     * deduction in capabilities to all the parent buckets.
     *
     * @param node The node to be removed.
     * @return True if the node exists in the class.
     */
    bool Remove(ClusterTreeNodeLabel const &node);

    /**
     * @brief Capability Queries the capabilities of the specified node. The node must exist in the
     * class prior to the call.
     *
     * @param node_label Label of the node to be queried.
     * @return The capabilities of the node in question.
     */
    WeightedCapabilities Capability(ClusterTreeNodeLabel const &node_label);

  private:
    struct Node {
        Node(ClusterTreeNodeLabel const &parent, bool external);

        ClusterTreeNodeLabel parent;
        WeightedCapabilities capabilities;
        bool external;
    };

    void UpdateAllParent(Node node, WeightedCapabilities const &delta, bool add);

    ClusterTreeNodeLabel const root_;
    std::unordered_map<ClusterTreeNodeLabel, Node> tree_;
};

} // namespace e8

#endif // PLACEMENT_CAPABILITY_H
