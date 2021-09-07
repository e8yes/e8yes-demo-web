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
#include <shared_mutex>
#include <unordered_map>

#include "cluster/placement/bucket.h"
#include "cluster/placement/capability.h"
#include "cluster/placement/cluster_map.h"
#include "cluster/placement/common_types.h"
#include "cluster/placement/score.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/machine.pb.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {
namespace {

constexpr char const *kRootLabel = "root";

struct UuidState {
    UuidState() { uuid4_seed(&uuid_state); }
    UUID4_STATE_T uuid_state;
};

UuidState gUuidState;

} // namespace

ClusterTreeNodeLabel AllocateClusterTreeNodeLabel() {
    UUID4_T uuid;
    uuid4_gen(&gUuidState.uuid_state, &uuid);

    char uuid_string[UUID4_STR_BUFFER_SIZE];
    uuid4_to_s(uuid, uuid_string, sizeof(uuid_string));

    return ClusterTreeNodeLabel(uuid_string);
}

ClusterMap::BucketOrMachine::BucketOrMachine(ClusterTreeNode::Hierarchy hierarchy,
                                             std::unique_ptr<BucketInterface> &&bucket)
    : hierarchy(hierarchy), bucket(std::move(bucket)) {}

ClusterMap::BucketOrMachine::BucketOrMachine(ClusterTreeNode::Hierarchy hierarchy,
                                             Machine const &machine)
    : hierarchy(hierarchy), machine(machine) {}

std::unordered_map<ClusterTreeNodeLabel, ClusterMap::BucketOrMachine>::iterator
ClusterMap::ImportNode(ClusterTreeNodeLabel const &parent_label,
                       ClusterTreeNodeLabel const &node_label, ClusterTreeNode const &node_proto) {
    if (node_proto.has_bucket()) {
        if (node_label != parent_label) {
            // Not a root node.
            capabilities_.AddBucket(parent_label, node_label);
        }

        auto bucket =
            CreateBucket(node_proto.bucket(), std::make_unique<MostDemandingCapabilityScore>());
        return tree_
            .insert(std::make_pair(node_label,
                                   BucketOrMachine(node_proto.hierarchy(), std::move(bucket))))
            .first;
    } else {
        assert(node_proto.has_machine());

        capabilities_.AddMachine(parent_label, node_label, node_proto.machine().capabilities());

        return tree_
            .insert(std::make_pair(node_label,
                                   BucketOrMachine(node_proto.hierarchy(), node_proto.machine())))
            .first;
    }
}

void ClusterMap::ImportChildren(
    ClusterTreeNodeLabel const &parent_label, BucketOrMachine const &parent_node,
    google::protobuf::Map<ClusterTreeNodeLabel, ClusterTreeNode> const &tree_proto) {
    if (parent_node.machine.has_value()) {
        return;
    }

    for (auto const &child : parent_node.bucket->Children()) {
        auto child_proto_it = tree_proto.find(child);
        assert(child_proto_it != tree_proto.end());

        auto const &[_, child_node_proto] = *child_proto_it;

        auto const &[child_label, child_node] =
            *this->ImportNode(parent_label, child, child_node_proto);

        this->ImportChildren(child_label, child_node, tree_proto);
    }
}

void ClusterMap::ExportNode(
    ClusterTreeNodeLabel const &node_label,
    google::protobuf::Map<ClusterTreeNodeLabel, ClusterTreeNode> *proto) const {
    auto it = tree_.find(node_label);
    assert(it != tree_.end());
    auto const &[_, node] = *it;

    ClusterTreeNode tree_node_proto;
    tree_node_proto.set_hierarchy(node.hierarchy);
    if (node.machine.has_value()) {
        *tree_node_proto.mutable_machine() = *node.machine;
    } else {
        assert(node.bucket != nullptr);
        *tree_node_proto.mutable_bucket() = node.bucket->ToProto();
    }

    (*proto)[node_label] = tree_node_proto;

    if (node.bucket != nullptr) {
        for (auto const &child : node.bucket->Children()) {
            this->ExportNode(child, proto);
        }
    }
}

ClusterMap::ClusterMap()
    : version_(ClusterMapVersionEpoch()), capabilities_(kRootLabel),
      mu_(std::make_unique<std::shared_mutex>()) {}

ClusterMap::ClusterMap(ClusterMapData const &cluster_map_data)
    : version_(cluster_map_data.version_epoch()), capabilities_(kRootLabel) {
    if (cluster_map_data.tree_nodes().empty()) {
        return;
    }

    auto proto_it = cluster_map_data.tree_nodes().find(kRootLabel);
    assert(proto_it != cluster_map_data.tree_nodes().end());
    assert(proto_it->second.has_bucket());

    auto const &[_, root_node_proto] = *proto_it;
    auto const &[__, root_node] = *this->ImportNode(kRootLabel, kRootLabel, root_node_proto);
    this->ImportChildren(kRootLabel, root_node, cluster_map_data.tree_nodes());
}

ClusterMap::~ClusterMap() {}

ClusterMapData ClusterMap::ToProto() const {
    ClusterMapData cluster_map_proto;
    cluster_map_proto.set_version_epoch(version_);
    this->ExportNode(kRootLabel, cluster_map_proto.mutable_tree_nodes());

    return cluster_map_proto;
}

} // namespace e8
