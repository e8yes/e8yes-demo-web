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
#include <optional>
#include <unordered_map>

#include "cluster/placement/bucket.h"
#include "cluster/placement/capability.h"
#include "cluster/placement/common_types.h"
#include "cluster/placement/hierarchy.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {
namespace {

constexpr char const *kRootLabel = "root";

WeightedCapabilities NegateCapabilities(WeightedCapabilities const &capabilities) {
    WeightedCapabilities negated;

    for (auto type : WeightedCapabilityTypes()) {
        float value = GetCapabilityByType(type, capabilities);
        SetCapabilityByType(type, -value, &negated);
    }

    return negated;
}

} // namespace

ClusterHierarchy::BucketOrMachine::BucketOrMachine(ClusterTreeNodeLabel const &parent_label,
                                                   ClusterTreeNode::Hierarchy hierarchy,
                                                   std::unique_ptr<BucketInterface> &&bucket)
    : parent_label(parent_label), hierarchy(hierarchy), bucket(std::move(bucket)) {}

ClusterHierarchy::BucketOrMachine::BucketOrMachine(ClusterTreeNodeLabel const &parent_label,
                                                   Machine const &machine)
    : parent_label(parent_label), hierarchy(ClusterTreeNode::MACHINE), machine(machine) {}

ClusterHierarchy::ClusterHierarchy() {}

ClusterHierarchy::~ClusterHierarchy() {}

void ClusterHierarchy::UpdateAllAncestors(ClusterTreeNodeLabel const &node_label,
                                          BucketOrMachine const &node,
                                          WeightedCapabilities const &delta) {
    BucketOrMachine const *child_node = &node;
    ClusterTreeNodeLabel child_node_label = node_label;

    do {
        auto parent_it = tree_.find(child_node->parent_label);
        assert(parent_it != tree_.end());
        assert(parent_it->second->bucket != nullptr);
        auto &[_, parent_node] = *parent_it;

        parent_node->bucket->AddCapabilitiesFor(child_node_label, delta);

        child_node_label = child_node->parent_label;
        child_node = parent_node.get();
    } while (child_node_label != child_node->parent_label);
}

bool ClusterHierarchy::AddRoot(std::unique_ptr<BucketInterface> &&bucket) {
    auto root_it = tree_.find(kRootLabel);
    if (root_it != tree_.end()) {
        return false;
    }

    auto node =
        std::make_unique<BucketOrMachine>(kRootLabel, ClusterTreeNode::ROOT, std::move(bucket));
    tree_.insert(std::make_pair(kRootLabel, std::move(node)));

    return true;
}

bool ClusterHierarchy::AddBucket(ClusterTreeNodeLabel const &parent_label,
                                 ClusterTreeNodeLabel const &node_label,
                                 ClusterTreeNode::Hierarchy hierarchy,
                                 std::unique_ptr<BucketInterface> &&bucket) {
    assert(node_label != parent_label);
    assert(node_label != kRootLabel);

    auto parent_it = tree_.find(parent_label);
    assert(parent_it != tree_.end());
    assert(hierarchy > parent_it->second->hierarchy);
    assert(!parent_it->second->machine.has_value());

    auto node_it = tree_.find(node_label);
    if (node_it != tree_.end()) {
        return false;
    }

    auto node = std::make_unique<BucketOrMachine>(parent_label, hierarchy, std::move(bucket));
    tree_.insert(std::make_pair(node_label, std::move(node)));

    return true;
}

bool ClusterHierarchy::AddMachine(ClusterTreeNodeLabel const &parent_label,
                                  ClusterTreeNodeLabel const &node_label, Machine const &machine) {
    assert(node_label != parent_label);
    assert(node_label != kRootLabel);

    auto node = std::make_unique<BucketOrMachine>(parent_label, machine);

    auto [it, successful] = tree_.insert(std::make_pair(node_label, std::move(node)));
    if (!successful) {
        return false;
    }

    this->UpdateAllAncestors(node_label, *it->second, machine.capabilities());

    return true;
}

void ClusterHierarchy::RemoveSubtree(ClusterTreeNodeLabel const &node_label) {
    auto it = tree_.find(node_label);
    assert(it != tree_.end());

    if (it->second->machine.has_value()) {
        WeightedCapabilities delta = NegateCapabilities(it->second->machine->capabilities());
        this->UpdateAllAncestors(node_label, *it->second, delta);

        tree_.erase(it);

        return;
    }

    assert(it->second->bucket != nullptr);
    for (auto const &child : it->second->bucket->Children()) {
        this->RemoveSubtree(child.label);
    }

    tree_.erase(it);
}

bool ClusterHierarchy::Remove(ClusterTreeNodeLabel const &node_label) {
    auto it = tree_.find(node_label);
    if (it == tree_.end()) {
        return false;
    }

    ClusterTreeNodeLabel parent = it->second->parent_label;

    this->RemoveSubtree(node_label);

    if (node_label == kRootLabel) {
        return true;
    }

    auto parent_it = tree_.find(parent);
    assert(parent_it != tree_.end());
    assert(parent_it->second->bucket != nullptr);
    parent_it->second->bucket->RemoveChild(node_label);

    return true;
}

ClusterHierarchy::BucketOrMachine const *ClusterHierarchy::Root() const {
    auto root_it = tree_.find(kRootLabel);
    if (root_it == tree_.end()) {
        return nullptr;
    }

    return root_it->second.get();
}

ClusterHierarchy::BucketOrMachine const *
ClusterHierarchy::Node(ClusterTreeNodeLabel const &node_label) const {
    auto node_it = tree_.find(node_label);
    assert(node_it != tree_.end());
    return node_it->second.get();
}

unsigned ClusterHierarchy::NodeCount() const { return tree_.size(); }

} // namespace e8
