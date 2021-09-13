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

#include <algorithm>
#include <cassert>
#include <google/protobuf/map.h>
#include <memory>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <utility>

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

unsigned const kMaxFailureFactor = 2;
unsigned const kMaxBucketCollisions = 3;

struct UuidState {
    UuidState() { uuid4_seed(&uuid_state); }
    UUID4_STATE_T uuid_state;
};

UuidState gUuidState;

} // namespace

namespace cluster_map_internal {

BucketOrMachine::BucketOrMachine(ClusterTreeNode::Hierarchy hierarchy,
                                 std::unique_ptr<BucketInterface> &&bucket)
    : hierarchy(hierarchy), bucket(std::move(bucket)) {
    assert(hierarchy != e8::ClusterTreeNode::INVALID_HIERARCHY);
}

BucketOrMachine::BucketOrMachine(ClusterTreeNode::Hierarchy hierarchy, Machine const &machine)
    : hierarchy(hierarchy), machine(machine) {
    assert(hierarchy != e8::ClusterTreeNode::INVALID_HIERARCHY);
}

class ClusterMapImpl {
  public:
    ClusterMapImpl(ClusterMapVersionEpoch version);
    ~ClusterMapImpl();

    TreeIterator ImportNode(ClusterTreeNodeLabel const &parent_label,
                            ClusterTreeNodeLabel const &node_label,
                            ClusterTreeNode const &node_proto);

    void
    ImportChildren(ClusterTreeNodeLabel const &parent_label, BucketOrMachine const &parent_node,
                   google::protobuf::Map<ClusterTreeNodeLabel, ClusterTreeNode> const &tree_proto);

    void ExportNode(ClusterTreeNodeLabel const &node_label,
                    google::protobuf::Map<ClusterTreeNodeLabel, ClusterTreeNode> *proto) const;

    ClusterCapability const &Capabilities() const;

    TreeIterator GetNode(ClusterTreeNodeLabel const &label) const;

    TreeIterator RootIterator() const;

    unsigned NodeCount() const;

    ClusterMapVersionEpoch Version() const;

  public:
    std::shared_mutex mu;

  private:
    ClusterMapVersionEpoch version_;
    ClusterCapability capabilities_;
    std::unordered_map<ClusterTreeNodeLabel, BucketOrMachine> tree_;
};

ClusterMapImpl::ClusterMapImpl(ClusterMapVersionEpoch version)
    : version_(version), capabilities_(kRootLabel) {}

ClusterMapImpl::~ClusterMapImpl() {}

TreeIterator ClusterMapImpl::ImportNode(ClusterTreeNodeLabel const &parent_label,
                                        ClusterTreeNodeLabel const &node_label,
                                        ClusterTreeNode const &node_proto) {
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

void ClusterMapImpl::ImportChildren(
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

void ClusterMapImpl::ExportNode(
    ClusterTreeNodeLabel const &node_label,
    google::protobuf::Map<ClusterTreeNodeLabel, ClusterTreeNode> *proto) const {
    TreeIterator it = tree_.find(node_label);
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

ClusterCapability const &ClusterMapImpl::Capabilities() const { return capabilities_; }

TreeIterator ClusterMapImpl::GetNode(ClusterTreeNodeLabel const &label) const {
    auto it = tree_.find(label);
    assert(it != tree_.end());
    return it;
}

TreeIterator ClusterMapImpl::RootIterator() const {
    auto it = tree_.find(kRootLabel);
    assert(it != tree_.end());
    return it;
}

unsigned ClusterMapImpl::NodeCount() const { return tree_.size(); }

ClusterMapVersionEpoch ClusterMapImpl::Version() const { return version_; }

} // namespace cluster_map_internal

ClusterTreeNodeLabel AllocateClusterTreeNodeLabel() {
    UUID4_T uuid;
    uuid4_gen(&gUuidState.uuid_state, &uuid);

    char uuid_string[UUID4_STR_BUFFER_SIZE];
    uuid4_to_s(uuid, uuid_string, sizeof(uuid_string));

    return ClusterTreeNodeLabel(uuid_string);
}

ClusterMap::ClusterMap()
    : pimpl_(std::make_unique<cluster_map_internal::ClusterMapImpl>(ClusterMapVersionEpoch())) {}

ClusterMap::ClusterMap(ClusterMapData const &cluster_map_data)
    : pimpl_(std::make_unique<cluster_map_internal::ClusterMapImpl>(
          cluster_map_data.version_epoch())) {
    if (cluster_map_data.tree_nodes().empty()) {
        return;
    }

    auto proto_it = cluster_map_data.tree_nodes().find(kRootLabel);
    assert(proto_it != cluster_map_data.tree_nodes().end());
    assert(proto_it->second.has_bucket());

    auto const &[_, root_node_proto] = *proto_it;
    auto const &[__, root_node] = *pimpl_->ImportNode(kRootLabel, kRootLabel, root_node_proto);
    pimpl_->ImportChildren(kRootLabel, root_node, cluster_map_data.tree_nodes());
}

ClusterMap::~ClusterMap() {}

ClusterMap::Placement::Placement(ResourceDescriptor const &resource,
                                 cluster_map_internal::TreeIterator const &root_it,
                                 cluster_map_internal::ClusterMapImpl *pimpl)
    : resource_(resource), pimpl_(pimpl), sink_({root_it}), error_(false) {}

ClusterMap::Placement::~Placement() {}

bool ClusterMap::Placement::DescentFrom(BucketInterface const &bucket,
                                        ClusterTreeNode::Hierarchy hierarchy,
                                        ResourceDescriptor const &resource, ReplicationRank rank,
                                        std::vector<cluster_map_internal::TreeIterator> *sink) {
    BucketInterface const *current_bucket = &bucket;
    unsigned num_failures = 0;
    unsigned num_bucket_failures = 0;

    while (num_failures < kMaxFailureFactor * pimpl_->NodeCount()) {
        std::optional<ClusterTreeNodeLabel> child =
            current_bucket->Select(resource, rank, num_failures, pimpl_->Capabilities());
        if (!child.has_value()) {
            // The bucket can't house the requested resource. Retry descent.
            ++num_failures;
            num_bucket_failures = 0;
            current_bucket = &bucket;
            continue;
        }

        cluster_map_internal::TreeIterator child_node_it = pimpl_->GetNode(*child);
        auto const &[_, child_node] = *child_node_it;

        if (child_node.hierarchy < hierarchy) {
            // The current hierarchy is too coarse. Continue descent.
            assert(child_node.bucket != nullptr);
            current_bucket = child_node.bucket.get();
            continue;
        }

        if (child_node.hierarchy > hierarchy) {
            // The requested hierarchy doesn't exist. Retry descent.
            ++num_failures;
            num_bucket_failures = 0;
            current_bucket = &bucket;
            continue;
        }

        auto sink_it = std::find_if(
            sink_.begin(), sink_.end(),
            [&child](cluster_map_internal::TreeIterator const &it) { return it->first == *child; });
        if (sink_it == sink_.end()) {
            // Found a desired node to place the resource.
            sink->push_back(child_node_it);
            return true;
        }

        if (num_bucket_failures < kMaxBucketCollisions) {
            // Encounters collision. Retry bucket.
            ++num_failures;
            ++num_bucket_failures;
        } else {
            // Too many collision failures. Retry descent.
            ++num_failures;
            num_bucket_failures = 0;
            current_bucket = &bucket;
        }
    }

    return false;
}

ClusterMap::Placement &ClusterMap::Placement::Select(ClusterTreeNode::Hierarchy hierarchy,
                                                     unsigned num_items) {
    if (error_) {
        return *this;
    }

    // The sink values of the last select becomes our source.
    source_ = sink_;
    sink_.clear();

    for (auto const &input : source_) {
        // The requested hierarchy must be more granular than the source.
        assert(hierarchy > input->second.hierarchy);
        assert(input->second.bucket != nullptr);

        for (unsigned i = 0; i < num_items; ++i) {
            if (!this->DescentFrom(*input->second.bucket, hierarchy, resource_, i, &sink_)) {
                error_ = true;
                return *this;
            }
        }
    }

    return *this;
}

std::vector<Machine> ClusterMap::Placement::Emit() const {
    if (error_) {
        return std::vector<Machine>();
    }

    std::vector<Machine> result(sink_.size());

    std::transform(sink_.begin(), sink_.end(), result.begin(),
                   [](cluster_map_internal::TreeIterator const &machine_it) {
                       auto const &[label, node] = *machine_it;
                       assert(node.machine.has_value());
                       return *node.machine;
                   });

    return result;
}

ClusterMap::Placement ClusterMap::TakeRootFor(ResourceDescriptor const &resource) const {
    cluster_map_internal::TreeIterator root_it = pimpl_->RootIterator();
    return Placement(resource, root_it, pimpl_.get());
}

ClusterMapData ClusterMap::ToProto() const {
    ClusterMapData cluster_map_proto;
    cluster_map_proto.set_version_epoch(pimpl_->Version());
    pimpl_->ExportNode(kRootLabel, cluster_map_proto.mutable_tree_nodes());

    return cluster_map_proto;
}

} // namespace e8
