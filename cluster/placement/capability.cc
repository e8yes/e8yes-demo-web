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
#include <unordered_map>
#include <utility>

#include "cluster/placement/capability.h"
#include "cluster/placement/common_types.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

float GetCapabilityByType(WeightedCapabilities::Type type,
                          WeightedCapabilities const &capabilities) {
    switch (type) {
    case WeightedCapabilities::CPU:
        return capabilities.cpu();
    case WeightedCapabilities::RAM:
        return capabilities.ram();
    case WeightedCapabilities::STORAGE:
        return capabilities.storage();
    case WeightedCapabilities::CORAL:
        return capabilities.coral();
    default:
        assert(false);
    }
}

void SetCapabilityByType(WeightedCapabilities::Type type, float value,
                         WeightedCapabilities *capabilities) {
    switch (type) {
    case WeightedCapabilities::CPU:
        capabilities->set_cpu(value);
        break;
    case WeightedCapabilities::RAM:
        capabilities->set_ram(value);
        break;
    case WeightedCapabilities::STORAGE:
        capabilities->set_storage(value);
        break;
    case WeightedCapabilities::CORAL:
        capabilities->set_coral(value);
        break;
    default:
        assert(false);
    }
}

std::vector<WeightedCapabilities::Type> WeightedCapabilityTypes() {
    return std::vector<WeightedCapabilities::Type>{
        WeightedCapabilities::CPU, WeightedCapabilities::RAM, WeightedCapabilities::STORAGE,
        WeightedCapabilities::CORAL};
}

ClusterCapability::Node::Node(ClusterTreeNodeLabel const &parent, bool external)
    : parent(parent), external(external) {}

ClusterCapability::ClusterCapability(ClusterTreeNodeLabel const &root) : root_(root) {
    tree_.insert(std::make_pair(root, Node(root, /*external=*/false)));
}

ClusterCapability::~ClusterCapability() {}

void ClusterCapability::UpdateAllParent(Node node, WeightedCapabilities const &delta, bool add) {
    Node *current_node = &node;
    ClusterTreeNodeLabel current_node_label; // It doesn't need initialization because it will be
                                             // initialized to the label of the parent node sson.

    do {
        current_node_label = current_node->parent;

        auto parent_it = tree_.find(current_node->parent);
        assert(parent_it != tree_.end());
        assert(parent_it->second.external == false);
        auto &[_, parent_node] = *parent_it;

        current_node = &parent_node;

        for (auto type : WeightedCapabilityTypes()) {
            float old_value = GetCapabilityByType(type, current_node->capabilities);
            float delta_value = GetCapabilityByType(type, delta);

            if (add) {
                SetCapabilityByType(type, old_value + delta_value, &current_node->capabilities);
            } else {
                SetCapabilityByType(type, old_value - delta_value, &current_node->capabilities);
            }
        }
    } while (current_node->parent != current_node_label);
}

bool ClusterCapability::AddBucket(ClusterTreeNodeLabel const &parent_label,
                                  ClusterTreeNodeLabel const &node_label) {
    assert(node_label != parent_label);

    auto parent_it = tree_.find(parent_label);
    assert(parent_it != tree_.end());
    assert(parent_it->second.external == false);

    auto [_, successful] =
        tree_.insert(std::make_pair(node_label, Node(parent_label, /*external=*/false)));
    return successful;
}

bool ClusterCapability::AddMachine(ClusterTreeNodeLabel const &parent_label,
                                   ClusterTreeNodeLabel const &node_label,
                                   WeightedCapabilities const &capabilities) {
    assert(node_label != parent_label);

    Node node(parent_label, /*external=*/true);
    node.capabilities = capabilities;

    auto [_, successful] = tree_.insert(std::make_pair(node_label, node));
    if (!successful) {
        return false;
    }

    this->UpdateAllParent(node, capabilities, /*add=*/true);

    return true;
}

bool ClusterCapability::Remove(ClusterTreeNodeLabel const &node_label) {
    auto it = tree_.find(node_label);
    if (it == tree_.end()) {
        return false;
    }

    auto const &[_, node] = *it;
    this->UpdateAllParent(node, node.capabilities, /*add=*/false);

    tree_.erase(it);

    return true;
}

WeightedCapabilities ClusterCapability::Capability(ClusterTreeNodeLabel const &node_label) const {
    auto it = tree_.find(node_label);
    assert(it != tree_.end());

    return it->second.capabilities;
}

} // namespace e8
