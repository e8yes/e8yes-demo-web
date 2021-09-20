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
#include <functional>
#include <google/protobuf/repeated_field.h>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cluster/placement/bucket.h"
#include "cluster/placement/bucket_tree.h"
#include "cluster/placement/capability.h"
#include "cluster/placement/common_types.h"
#include "cluster/placement/score.h"
#include "proto_cc/bucket.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {
namespace {

bool EnterWithProbability(size_t hash, float p) { return hash % 16384 < p * 16384; }

} // namespace

TreeBucket::Node::Node() : id("1"), num_external_nodes(0), parent(nullptr) {}

TreeBucket::Node::Node(Node *old_root)
    : id(old_root->id + "0"), capabilities(old_root->capabilities),
      num_external_nodes(old_root->num_external_nodes), parent(nullptr) {
    assert(old_root != nullptr);

    old_root->parent = this;
}

TreeBucket::Node::Node(Node *parent, unsigned depth, bool left_child)
    : num_external_nodes(0), parent(parent) {
    assert(parent != nullptr);

    id = parent->id;
    assert(id.length() > depth);

    if (left_child) {
        id[depth] = '0';
    } else {
        id[depth] = '1';
    }
}

std::string TreeBucket::Node::Id() const { return id; }

bool TreeBucket::Node::Internal() const { return left != nullptr && right != nullptr; }

bool TreeBucket::Node::External() const { return left == nullptr && right == nullptr; }

void TreeBucket::Node::SetChildren(std::unique_ptr<Node> &&left, std::unique_ptr<Node> &&right) {
    assert(this->External());

    assert(left != nullptr);
    assert(right != nullptr);

    this->left = std::move(left);
    this->right = std::move(right);
}

bool TreeBucket::Node::Empty() const {
    assert(this->External());
    return !label.has_value();
}

void TreeBucket::Node::Clear() {
    assert(this->External());

    label.reset();

    WeightedCapabilities deduction = capabilities;

    Node *current_ancestor = this;
    do {
        --current_ancestor->num_external_nodes;
        current_ancestor->capabilities -= deduction;

        current_ancestor = current_ancestor->parent;
    } while (current_ancestor != nullptr);
}

void TreeBucket::Node::Use(ClusterTreeNodeLabel const &label) {
    assert(this->External());
    this->label = label;

    Node *current_ancestor = this;
    do {
        ++current_ancestor->num_external_nodes;
        current_ancestor = current_ancestor->parent;
    } while (current_ancestor != nullptr);
}

void TreeBucket::Node::AddCapabilities(WeightedCapabilities const &delta) {
    assert(this->External());

    Node *current_ancestor = this;
    do {
        current_ancestor->capabilities += delta;
        current_ancestor = current_ancestor->parent;
    } while (current_ancestor != nullptr);
}

unsigned TreeBucket::Node::ExternalNodeCount() const { return num_external_nodes; }

ClusterTreeNodeLabel TreeBucket::Node::Label() const {
    assert(!this->Empty());
    return *label;
}

WeightedCapabilities TreeBucket::Node::Capabilities() const { return capabilities; }

TreeBucket::Node *TreeBucket::Node::LeftChild() { return left.get(); }

TreeBucket::Node *TreeBucket::Node::RightChild() { return right.get(); }

TreeBucket::Node const *TreeBucket::Node::LeftChild() const { return left.get(); }

TreeBucket::Node const *TreeBucket::Node::RightChild() const { return right.get(); }

TreeBucket::TreeBucket(
    TreeBucketData const &data,
    google::protobuf::RepeatedPtrField<ClusterTreeNodeNamespace> const &supported_namespaces,
    std::unique_ptr<CapabilityScoreInterface> &&scorer)
    : BucketInterface(supported_namespaces), scorer_(std::move(scorer)),
      root_(std::make_unique<Node>()), depth_(0) {

    for (auto const &child_label : data.child_labels()) {
        bool added = this->AddChild(child_label);
        assert(added);
    }
}

TreeBucket::~TreeBucket() {}

void TreeBucket::CollectBucketChildren(Node const *node, std::vector<Child> *children) const {
    if (node->External() && !node->Empty()) {
        children->push_back(Child(node->Label(), node->Capabilities()));
        return;
    }

    assert(node->Internal());

    // Internal nodes must have two children.
    this->CollectBucketChildren(node->LeftChild(), children);
    this->CollectBucketChildren(node->RightChild(), children);
}

TreeBucket::Node *TreeBucket::Find(Node *subtree_root, ClusterTreeNodeLabel const &node_label) {
    assert(subtree_root != nullptr);

    if (subtree_root->External()) {
        if (!subtree_root->Empty() && subtree_root->Label() == node_label) {
            return subtree_root;
        } else {
            return nullptr;
        }
    }

    assert(subtree_root->Internal());

    Node *result = this->Find(subtree_root->LeftChild(), node_label);
    if (result != nullptr) {
        return result;
    }

    return this->Find(subtree_root->RightChild(), node_label);
}

unsigned TreeBucket::TreeCapacity() const { return 1 << depth_; }

void TreeBucket::BuildSubtree(Node *parent_node, unsigned parent_depth, unsigned target_depth) {
    if (parent_depth == target_depth) {
        return;
    }

    parent_node->SetChildren(
        std::make_unique<Node>(parent_node, parent_depth + 1, /*left_child=*/true),
        std::make_unique<Node>(parent_node, parent_depth + 1, /*left_child=*/false));

    this->BuildSubtree(parent_node->LeftChild(), parent_depth + 1, target_depth);
    this->BuildSubtree(parent_node->RightChild(), parent_depth + 1, target_depth);
}

void TreeBucket::ExpandDepth() {
    auto old_root = std::move(root_);
    auto new_root = std::make_unique<Node>(old_root.get());

    // The old tree becomes the left subtree.
    std::unique_ptr<Node> left_subtree = std::move(old_root);

    // The right subtree is going to build entirely with empty nodes.
    std::unique_ptr<Node> right_subtree =
        std::make_unique<Node>(new_root.get(), /*depth=*/1, /*left_child=*/false);

    this->BuildSubtree(right_subtree.get(), /*parent_depth=*/1, depth_ + 1);

    // Combines subtrees and the new root is complete.
    new_root->SetChildren(std::move(left_subtree), std::move(right_subtree));
    root_ = std::move(new_root);

    ++depth_;
}

TreeBucket::Node *TreeBucket::EmptyNodeFrom(Node *subtree_root) {
    if (subtree_root->External()) {
        if (subtree_root->Empty()) {
            return subtree_root;
        } else {
            return nullptr;
        }
    }

    assert(subtree_root->Internal());

    Node *result = this->EmptyNodeFrom(subtree_root->LeftChild());
    if (result != nullptr) {
        return result;
    }

    return this->EmptyNodeFrom(subtree_root->RightChild());
}

std::optional<ClusterTreeNodeLabel> TreeBucket::Select(ResourceDescriptor const &resource,
                                                       unsigned rank, unsigned num_failures) const {
    if (!this->SupportNameSpace(resource.name_space)) {
        return std::nullopt;
    }

    if (root_->ExternalNodeCount() == 0) {
        return std::nullopt;
    }

    // Randomly descent through the tree until we reach an external node.
    Node *current_node = root_.get();

    std::string const jumps = std::to_string(rank + num_failures);
    std::string const key_and_jumps = resource.key + jumps;

    while (current_node->Internal()) {
        // We randomly selects a branch to descent based on each branch's hardware capability
        // score with respect to the resource. It should distribute the resource fairly into the
        // appropiate child. All internal nodes must have two child branches, though one of the
        // branches may not have any external node. In this case, we should deterministically
        // skip that branch.
        if (current_node->LeftChild()->ExternalNodeCount() > 0 &&
            current_node->RightChild()->ExternalNodeCount() > 0) {
            std::vector<float> scores = scorer_->Score({current_node->LeftChild()->Capabilities(),
                                                        current_node->RightChild()->Capabilities()},
                                                       resource);
            if (scores.empty()) {
                // None of the children is able to satisfy with the resource's requirement.
                return std::nullopt;
            }

            assert(scores.size() == 2);

            std::string hash_key = key_and_jumps + current_node->Id();
            size_t random_variable = std::hash<std::string>()(hash_key);

            if (EnterWithProbability(random_variable, scores[0])) {
                current_node = current_node->LeftChild();
            } else {
                current_node = current_node->RightChild();
            }
        } else if (current_node->LeftChild()->ExternalNodeCount() > 0) {
            if (scorer_->Score({current_node->LeftChild()->Capabilities()}, resource).empty()) {
                return std::nullopt;
            }

            current_node = current_node->LeftChild();
        } else if (current_node->RightChild()->ExternalNodeCount() > 0) {
            if (scorer_->Score({current_node->RightChild()->Capabilities()}, resource).empty()) {
                return std::nullopt;
            }

            current_node = current_node->RightChild();
        } else {
            // If the parent has non-zero external node count, one of the children must also has
            // non-zero external node count.
            assert(false);
        }
    }

    assert(current_node->External());
    assert(!current_node->Empty());
    return current_node->Label();
}

bool TreeBucket::AddChild(ClusterTreeNodeLabel const &child_label) {
    Node *old_node = this->Find(root_.get(), child_label);
    if (old_node != nullptr) {
        return false;
    }

    if (root_->ExternalNodeCount() == this->TreeCapacity()) {
        // We've reached the tree's maximum capacity.
        this->ExpandDepth();
        assert(this->TreeCapacity() > root_->ExternalNodeCount());
    }

    Node *new_node = this->EmptyNodeFrom(root_.get());
    assert(new_node != nullptr);
    new_node->Use(child_label);

    return true;
}

bool TreeBucket::RemoveChild(ClusterTreeNodeLabel const &child_label) {
    Node *to_be_removed = this->Find(root_.get(), child_label);
    if (to_be_removed == nullptr) {
        return false;
    }

    // Only clears the node and not touch the tree topology.
    to_be_removed->Clear();

    return true;
}

std::vector<BucketInterface::Child> TreeBucket::Children() const {
    std::vector<BucketInterface::Child> children;
    if (root_ == nullptr) {
        return children;
    }

    this->CollectBucketChildren(root_.get(), &children);
    return children;
}

void TreeBucket::AddCapabilitiesFor(ClusterTreeNodeLabel const &child_label,
                                    WeightedCapabilities const &delta) {
    // Takes out the desired external node and updates the capability values for it as well as all
    // its ancestors.
    Node *node = this->Find(root_.get(), child_label);
    assert(node != nullptr);

    node->AddCapabilities(delta);
}

Bucket TreeBucket::ToProto() const {
    Bucket bucket;

    for (auto const &child : this->Children()) {
        bucket.mutable_tree_bucket()->add_child_labels(child.label);
    }

    return bucket;
}

} // namespace e8
