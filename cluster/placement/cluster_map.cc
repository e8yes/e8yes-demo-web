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
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <utility>

#include "cluster/placement/bucket.h"
#include "cluster/placement/capability.h"
#include "cluster/placement/cluster_map.h"
#include "cluster/placement/common_types.h"
#include "cluster/placement/hierarchy.h"
#include "cluster/placement/score.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/machine.pb.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {
namespace {

unsigned const kMaxFailureFactor = 2;
unsigned const kMaxBucketCollisions = 3;

struct UuidState {
    UuidState() { uuid4_seed(&uuid_state); }
    UUID4_STATE_T uuid_state;
};

UuidState gUuidState;

class SharedLockGuard {
  public:
    SharedLockGuard(std::shared_mutex &mu);
    ~SharedLockGuard();

  private:
    std::shared_mutex &mu_;
};

SharedLockGuard::SharedLockGuard(std::shared_mutex &mu) : mu_(mu) { mu_.lock_shared(); }

SharedLockGuard::~SharedLockGuard() { mu_.unlock_shared(); }

ClusterHierarchy::BucketOrMachine const *CreateNode(ClusterTreeNodeLabel const &node_label,
                                                    ClusterTreeNodeLabel const &parent_label,
                                                    ClusterTreeNode const &node_proto,
                                                    ClusterHierarchy *hierarchy) {
    if (node_proto.has_bucket()) {
        auto bucket =
            CreateBucket(node_proto.bucket(), std::make_unique<MostDemandingCapabilityScore>());

        if (node_label == kClusterHierarchyRootLabel) {
            hierarchy->AddRoot(std::move(bucket));
        } else {
            hierarchy->AddBucket(parent_label, node_label, node_proto.hierarchy(),
                                 std::move(bucket));
        }
    } else {
        assert(node_proto.has_machine());

        hierarchy->AddMachine(parent_label, node_label, node_proto.machine());
    }

    return hierarchy->Node(node_label);
}

void ImportNode(ClusterTreeNodeLabel const &node_label, ClusterTreeNodeLabel const &parent_label,
                google::protobuf::Map<ClusterTreeNodeLabel, ClusterTreeNode> const &tree_proto,
                ClusterHierarchy *hierarchy) {
    auto it = tree_proto.find(node_label);
    assert(it != tree_proto.end());
    auto const &[_, node_proto] = *it;

    ClusterHierarchy::BucketOrMachine const *imported_node =
        CreateNode(node_label, parent_label, node_proto, hierarchy);

    if (imported_node->machine.has_value()) {
        return;
    }

    assert(imported_node->bucket != nullptr);

    for (auto const &child : imported_node->bucket->Children()) {
        ImportNode(child.label, node_label, tree_proto, hierarchy);
    }
}

void ExportNode(ClusterTreeNodeLabel const &node_label,
                ClusterHierarchy::BucketOrMachine const &node, ClusterHierarchy const &hierarchy,
                google::protobuf::Map<ClusterTreeNodeLabel, ClusterTreeNode> *proto) {
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
            ClusterHierarchy::BucketOrMachine const *child_node = hierarchy.Node(child.label);
            ExportNode(child.label, *child_node, hierarchy, proto);
        }
    }
}

void AddNode(ClusterMapRevision::Action const &action, ClusterHierarchy *hierarchy) {
    assert(action.operation() == ClusterMapRevision::ADD_TREE_NODE);

    if (action.tree_node().has_bucket()) {
        if (action.node_label() == kClusterHierarchyRootLabel) {
            std::unique_ptr<BucketInterface> bucket = CreateBucket(
                action.tree_node().bucket(), std::make_unique<MostDemandingCapabilityScore>());
            bool added = hierarchy->AddRoot(std::move(bucket));
            assert(added == true);
        } else {
            std::unique_ptr<BucketInterface> bucket = CreateBucket(
                action.tree_node().bucket(), std::make_unique<MostDemandingCapabilityScore>());
            bool added = hierarchy->AddBucket(action.parent_label(), action.node_label(),
                                              action.tree_node().hierarchy(), std::move(bucket));
            assert(added == true);
        }
    } else {
        assert(action.tree_node().has_machine());
        hierarchy->AddMachine(action.parent_label(), action.node_label(),
                              action.tree_node().machine());
    }
}

} // namespace

ClusterTreeNodeLabel AllocateClusterTreeNodeLabel() {
    UUID4_T uuid;
    uuid4_gen(&gUuidState.uuid_state, &uuid);

    char uuid_string[UUID4_STR_BUFFER_SIZE];
    uuid4_to_s(uuid, uuid_string, sizeof(uuid_string));

    return ClusterTreeNodeLabel(uuid_string);
}

ClusterMap::ClusterMap()
    : version_(ClusterMapVersionEpoch()), hierarchy_(std::make_unique<ClusterHierarchy>()),
      mu_(std::make_unique<std::shared_mutex>()) {}

ClusterMap::ClusterMap(ClusterMapData const &cluster_map_data)
    : version_(cluster_map_data.version_epoch()), hierarchy_(std::make_unique<ClusterHierarchy>()),
      mu_(std::make_unique<std::shared_mutex>()) {
    if (cluster_map_data.tree_nodes().empty()) {
        return;
    }

    ImportNode(kClusterHierarchyRootLabel, kClusterHierarchyRootLabel,
               cluster_map_data.tree_nodes(), hierarchy_.get());
}

ClusterMap::ClusterMap(ClusterMap const &other) : ClusterMap(other.ToProto()) {}

ClusterMap::~ClusterMap() {}

ClusterMap::Placement::Placement(ResourceDescriptor const &resource, ClusterHierarchy *hierarchy,
                                 std::shared_mutex *mu)
    : resource_(resource), hierarchy_(hierarchy), mu_(mu), sink_({hierarchy->Root()}),
      error_(hierarchy->Root() == nullptr) {}

ClusterMap::Placement::~Placement() {}

bool ClusterMap::Placement::DescentFrom(
    BucketInterface const &bucket, ClusterTreeNode::Hierarchy hierarchy,
    ResourceDescriptor const &resource, ReplicationRank rank,
    std::vector<ClusterHierarchy::BucketOrMachine const *> *sink) {
    BucketInterface const *current_bucket = &bucket;
    unsigned num_failures = 0;
    unsigned num_bucket_failures = 0;

    while (num_failures < kMaxFailureFactor * hierarchy_->NodeCount()) {
        std::optional<ClusterTreeNodeLabel> child_label =
            current_bucket->Select(resource, rank, num_failures);
        if (!child_label.has_value()) {
            // The bucket can't house the requested resource. Retry descent.
            ++num_failures;
            num_bucket_failures = 0;
            current_bucket = &bucket;
            continue;
        }

        ClusterHierarchy::BucketOrMachine const *child_node = hierarchy_->Node(*child_label);

        if (child_node->hierarchy < hierarchy) {
            // The current hierarchy is too coarse. Continue descent.
            assert(child_node->bucket != nullptr);
            current_bucket = child_node->bucket.get();
            continue;
        }

        if (child_node->hierarchy > hierarchy) {
            // The requested hierarchy doesn't exist. Retry descent.
            ++num_failures;
            num_bucket_failures = 0;
            current_bucket = &bucket;
            continue;
        }

        if (std::find(sink_.begin(), sink_.end(), child_node) == sink_.end()) {
            // Found a desired node to place the resource.
            sink->emplace_back(child_node);
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

    for (auto const &node : source_) {
        // The requested hierarchy must be more granular than the source.
        assert(hierarchy > node->hierarchy);
        assert(node->bucket != nullptr);

        for (unsigned i = 0; i < num_items; ++i) {
            if (!this->DescentFrom(*node->bucket, hierarchy, resource_, i, &sink_)) {
                error_ = true;
                return *this;
            }
        }
    }

    return *this;
}

std::vector<Machine> ClusterMap::Placement::Emit() const {
    mu_->unlock_shared(); // Locked by the TakeRootFor() call.

    if (error_) {
        return std::vector<Machine>();
    }

    std::vector<Machine> result(sink_.size());

    std::transform(sink_.begin(), sink_.end(), result.begin(),
                   [](ClusterHierarchy::BucketOrMachine const *node) {
                       assert(node->machine.has_value());
                       return *node->machine;
                   });

    return result;
}

ClusterMapVersionEpoch ClusterMap::Version() const { return version_; }

bool ClusterMap::Revise(ClusterMapRevision const &revision) {
    std::lock_guard guard(*mu_);

    if (revision.from_version_epoch() != version_) {
        // Version mistmatch.
        return false;
    }

    assert(revision.to_version_epoch() >= revision.from_version_epoch());

    for (auto const &action : revision.actions()) {
        switch (action.operation()) {
        case ClusterMapRevision::ADD_TREE_NODE: {
            AddNode(action, hierarchy_.get());
            break;
        }
        case ClusterMapRevision::DELETE_TREE_NODE: {
            bool removed = hierarchy_->Remove(action.node_label());
            assert(removed == true);
            break;
        }
        case ClusterMapRevision::INVALID_OPERATION:
        default: {
            assert(false);
        }
        }
    }

    version_ = revision.to_version_epoch();
    return true;
}

ClusterMap::Placement ClusterMap::TakeRootFor(ResourceDescriptor const &resource) const {
    mu_->lock_shared(); // Unlocks when the Emit() function is called.

    return Placement(resource, hierarchy_.get(), mu_.get());
}

ClusterMapData ClusterMap::ToProto() const {
    SharedLockGuard guard(*mu_);

    ClusterMapData cluster_map_proto;
    cluster_map_proto.set_version_epoch(version_);

    if (hierarchy_->Root() != nullptr) {
        ExportNode(kClusterHierarchyRootLabel, *hierarchy_->Root(), *hierarchy_,
                   cluster_map_proto.mutable_tree_nodes());
    }

    return cluster_map_proto;
}

} // namespace e8
