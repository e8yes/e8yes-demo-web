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

#ifndef PLACEMENT_BUCKET_H
#define PLACEMENT_BUCKET_H

#include <list>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cluster/placement/common_types.h"
#include "cluster/placement/score.h"
#include "proto_cc/bucket.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

/**
 * @brief The BucketInterface class A logical grouping element. It allows the client of the class to
 * quickly place/locate a resource in a group of child nodes.
 */
class BucketInterface {
  public:
    /**
     * @brief The Child struct
     */
    struct Child {
        Child(ClusterTreeNodeLabel const &label, WeightedCapabilities const &capabilities);

        ClusterTreeNodeLabel label;
        WeightedCapabilities capabilities;
    };

    BucketInterface();
    virtual ~BucketInterface();

    /**
     * @brief Select Selects a tree node from the bucket for the placement or location of the
     * specified resource of particular rank (replica). The selection process distributes the
     * resource pseudo-probabilistically based on the distribution of capability scores over the
     * children (see CapabilityScoreInterface for how the scores is calculated). Changing the
     * parameter num_failures may or may not make the function return a different child. The
     * funciton returns a std::nullopt only when the bucket is empty or it couldn't find any
     * suitable children for the resource.
     */
    virtual std::optional<ClusterTreeNodeLabel>
    Select(ResourceDescriptor const &resource, unsigned rank, unsigned num_failures) const = 0;

    /**
     * @brief AddChild Adds a child node with zero capabilities to the bucket.
     *
     * @param child_label Label of the child to be added.
     * @return True if the child node has not already existed.
     */
    virtual bool AddChild(ClusterTreeNodeLabel const &child_label) = 0;

    /**
     * @brief RemoveChild Removes a node from the bucket.
     *
     * @param child_label The label of the child node to be removed.
     * @return True if the child node does exist in the bucket.
     */
    virtual bool RemoveChild(ClusterTreeNodeLabel const &child_label) = 0;

    /**
     * @brief Children Returns a list of children the bucket currently has.
     */
    virtual std::vector<Child> Children() const = 0;

    /**
     * @brief AddCapabilitiesFor Adds the capability deltas to one of its children. The function
     * will fail if the specified child doesn't exist.
     *
     * @param child_label Label of the child to be added delta to.
     * @param delta The delta capability values to be added to the child.
     */
    virtual void AddCapabilitiesFor(ClusterTreeNodeLabel const &child_label,
                                    WeightedCapabilities const &delta) = 0;

    /**
     * @brief ToProto Exports the bucket data structure as a Bucket proto.
     */
    virtual Bucket ToProto() const = 0;
};

/**
 * @brief The UniformBucket class It distributes resources uniformly across the children without
 * considering their hardware capabilities. It's capable of achieving O(1) time select. However,
 * adding or removing any children will result in a complete reshuffling of the resources that live
 * under this bucket. This class isn't thread safe.
 */
class UniformBucket : public BucketInterface {
  public:
    UniformBucket(UniformBucketData const &data);
    ~UniformBucket() override;

    std::optional<ClusterTreeNodeLabel> Select(ResourceDescriptor const &resource, unsigned rank,
                                               unsigned num_failures) const override;

    bool AddChild(ClusterTreeNodeLabel const &child_label) override;

    bool RemoveChild(ClusterTreeNodeLabel const &child_label) override;

    std::vector<Child> Children() const override;

    void AddCapabilitiesFor(ClusterTreeNodeLabel const &child_label,
                            WeightedCapabilities const &delta) override;

    Bucket ToProto() const override;

  private:
    std::vector<Child> children_;
    unsigned prime_;
};

/**
 * @brief The ListBucket class A list bucket organizes child elements into a linked list and scans
 * through the list to place a resource. So, it's capable of distributing resources according to
 * children's hardware capabilities. It takes O(n) time to select a child item with n children.
 * Adding children is optimally in terms of resource movement. However, removing children in the
 * middle of the list results in large amount of shuffling. This class isn't thread safe.
 */
class ListBucket : public BucketInterface {
  public:
    /**
     * @brief ListBucket Constructs a list bucket from the ListBucketData proto and a custom scorer.
     * The scorer helps convert children's capabilities with respect to a particular resource to a
     * distribution which the Select() function places a resource pseudo-randomly with.
     */
    ListBucket(ListBucketData const &data, std::unique_ptr<CapabilityScoreInterface> &&scorer);
    ~ListBucket() override;

    std::optional<ClusterTreeNodeLabel> Select(ResourceDescriptor const &resource, unsigned rank,
                                               unsigned num_failures) const override;

    bool AddChild(ClusterTreeNodeLabel const &child_label) override;

    bool RemoveChild(ClusterTreeNodeLabel const &child_label) override;

    std::vector<Child> Children() const override;

    void AddCapabilitiesFor(ClusterTreeNodeLabel const &child_label,
                            WeightedCapabilities const &delta) override;

    Bucket ToProto() const override;

  private:
    std::vector<ClusterTreeNodeLabel> child_labels_;
    std::vector<WeightedCapabilities> children_capabilities_;
    std::unique_ptr<CapabilityScoreInterface> scorer_;
};

/**
 * @brief The TreeBucket class A tree bucket organizes child elements in a cluster map into a
 * balanced binary tree. This avoids the need to scan through a list of child elements while still
 * be able to distribute resources fairly based on children's hardware capabilities. The selection
 * process takes O(log(n)) time where n is the number of child elements in the bucket. Adding and
 * removing child elements doesn't lead to catastrophic amount of shuffling.
 */
class TreeBucket : public BucketInterface {
  public:
    /**
     * @brief TreeBucket Constructs a tree bucket from the TreeBucketData proto and a custom
     * scorer. The scorer helps convert children's capabilities with respect to a particular
     * resource to a distribution which the Select() function places a resource pseudo-randomly
     * with.
     */
    TreeBucket(TreeBucketData const &data, std::unique_ptr<CapabilityScoreInterface> &&scorer);
    ~TreeBucket();

    std::optional<ClusterTreeNodeLabel> Select(ResourceDescriptor const &resource, unsigned rank,
                                               unsigned num_failures) const override;

    bool AddChild(ClusterTreeNodeLabel const &child_label) override;

    bool RemoveChild(ClusterTreeNodeLabel const &child_label) override;

    std::vector<Child> Children() const override;

    void AddCapabilitiesFor(ClusterTreeNodeLabel const &child_label,
                            WeightedCapabilities const &delta) override;

    Bucket ToProto() const override;

  private:
    class Node {
      public:
        /**
         * @brief Node Constructs the intial root node. This can only be called once for each tree.
         */
        Node();

        /**
         * @brief Node Constructs a new root on top of the old root and points the old root's parent
         * to this node.
         */
        Node(Node *old_root);

        /**
         * @brief Node Constructs a non-root node under the specified parent.
         */
        Node(Node *parent, unsigned depth, bool left_child);

        /**
         * @brief Id Unique (over the entire tree) identifier of this node.
         */
        std::string Id() const;

        /**
         * @brief Internal Checks if this node is an internal node.
         */
        bool Internal() const;

        /**
         * @brief External Checks if this node is an external node.
         */
        bool External() const;

        /**
         * @brief SetChildren Sets the children of this node and turns it into an internal node.
         * For an internal node, neither children coud be nullptr. The node must be an external
         * node, or it will fail.
         */
        void SetChildren(std::unique_ptr<Node> &&left, std::unique_ptr<Node> &&right);

        /**
         * @brief Empty Checks if this node was marked as empty. The node must be an external node,
         * or it will fail.
         */
        bool Empty() const;

        /**
         * @brief SetEmpty Clears the node's label to mark it as empty. Deducts capabilities and
         * external node count from still-valid ancestors. The tree structure is left intact and
         * this deleted node object serves as a placeholder so it could be re-used by the next
         * SetUsed() call. The node must be an external node, or it will fail.
         */
        void Clear();

        /**
         * @brief Use Marks the tree as non-empty and labels it with the specified label value, then
         * adds external node count to all its ancestors. The node must be an external node, or it
         * will fail.
         */
        void Use(ClusterTreeNodeLabel const &label);

        /**
         * @brief AddCapabilities Adds the capability deltas to the node and all its ancestors. The
         * node must be an external node, or it will fail.
         */
        void AddCapabilities(WeightedCapabilities const &delta);

        /**
         * @brief ExternalNodeCount Returns the number of non-empty external nodes in the subtree
         * rooted by this node.
         */
        unsigned ExternalNodeCount() const;

        /**
         * @brief Label Returns the label of this node. This function will fail if the node is
         * empty.
         */
        ClusterTreeNodeLabel Label() const;

        /**
         * @brief Capabilities The hardware capabilities of the subtree with this node being the
         * root.
         */
        WeightedCapabilities Capabilities() const;

        /**
         * @brief LeftChild Pointer to the left child.
         */
        Node *LeftChild();

        /**
         * @brief RightChild Pointer to the right child.
         */
        Node *RightChild();

        /**
         * @brief LeftChild Pointer to the left child.
         */
        Node const *LeftChild() const;

        /**
         * @brief RightChild Pointer to the right child.
         */
        Node const *RightChild() const;

      private:
        std::string id;
        std::optional<ClusterTreeNodeLabel> label;
        WeightedCapabilities capabilities;

        unsigned num_external_nodes;

        Node *parent;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    void CollectBucketChildren(Node const *node, std::vector<Child> *children) const;

    Node *Find(Node *subtree_root, ClusterTreeNodeLabel const &node_label);

    unsigned TreeCapacity() const;

    void BuildSubtree(Node *parent_node, unsigned parent_depth, unsigned target_depth);

    void ExpandDepth();

    Node *EmptyNodeFrom(Node *subtree_root);

    std::unique_ptr<CapabilityScoreInterface> scorer_;
    std::unique_ptr<Node> root_;
    unsigned depth_;
};

/**
 * @brief CreateBucket Creates a bucket with the specified proto and scorer. The bucket
 * implementation is chosen based on the proto.
 */
std::unique_ptr<BucketInterface> CreateBucket(Bucket const &proto,
                                              std::unique_ptr<CapabilityScoreInterface> &&scorer);

} // namespace e8

#endif // PLACEMENT_BUCKET_H
