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
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cluster/placement/bucket.h"
#include "cluster/placement/capability.h"
#include "cluster/placement/common_types.h"
#include "proto_cc/bucket.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {
namespace {

bool EnterWithProbability(size_t hash, float p) { return hash % 16384 < p * 16384; }

void AddCapabilities(WeightedCapabilities const &delta, WeightedCapabilities *target) {
    for (auto type : WeightedCapabilityTypes()) {
        float value = GetCapabilityByType(type, *target);
        float delta_value = GetCapabilityByType(type, delta);

        SetCapabilityByType(type, value + delta_value, target);
    }
}

} // namespace

BucketInterface::Child::Child(ClusterTreeNodeLabel const &label,
                              WeightedCapabilities const &capabilities)
    : label(label), capabilities(capabilities) {}

BucketInterface::BucketInterface() {}

BucketInterface::~BucketInterface() {}

UniformBucket::UniformBucket(UniformBucketData const &data) : prime_(data.prime()) {
    for (auto const &child_label : data.child_labels()) {
        children_.push_back(Child(child_label, WeightedCapabilities()));
    }
}

UniformBucket::~UniformBucket() {}

std::optional<ClusterTreeNodeLabel> UniformBucket::Select(ResourceDescriptor const &resource,
                                                          unsigned rank,
                                                          unsigned num_failures) const {
    if (children_.empty()) {
        return std::nullopt;
    }

    unsigned jumps = rank + num_failures;
    size_t final_hash = std::hash<std::string>()(resource.key) + jumps * prime_;
    unsigned location = final_hash % children_.size();

    return children_[location].label;
}

bool UniformBucket::AddChild(ClusterTreeNodeLabel const &child_label) {
    auto it =
        std::find_if(children_.begin(), children_.end(), [child_label](Child const &child) -> bool {
            return child.label == child_label;
        });
    if (it != children_.end()) {
        return false;
    }

    children_.push_back(Child(child_label, WeightedCapabilities()));
    return true;
}

bool UniformBucket::RemoveChild(ClusterTreeNodeLabel const &child_label) {
    auto it =
        std::find_if(children_.begin(), children_.end(), [child_label](Child const &child) -> bool {
            return child.label == child_label;
        });
    if (it == children_.end()) {
        return false;
    }

    children_.erase(it);
    return true;
}

std::vector<BucketInterface::Child> UniformBucket::Children() const { return children_; }

void UniformBucket::AddCapabilitiesFor(ClusterTreeNodeLabel const &child_label,
                                       WeightedCapabilities const &delta) {
    auto it =
        std::find_if(children_.begin(), children_.end(), [child_label](Child const &child) -> bool {
            return child.label == child_label;
        });
    assert(it != children_.end());

    AddCapabilities(delta, &it->capabilities);
}

Bucket UniformBucket::ToProto() const {
    Bucket bucket;
    bucket.mutable_uniform_bucket()->set_prime(prime_);

    for (auto const &child : children_) {
        *bucket.mutable_uniform_bucket()->add_child_labels() = child.label;
    }

    return bucket;
}

ListBucket::ListBucket(ListBucketData const &data,
                       std::unique_ptr<CapabilityScoreInterface> &&scorer)
    : child_labels_(data.child_labels_size()), children_capabilities_(data.child_labels_size()),
      scorer_(std::move(scorer)) {

    for (int i = 0; i < data.child_labels_size(); ++i) {
        child_labels_[i] = data.child_labels()[i];
    }
}

ListBucket::~ListBucket() {}

std::optional<ClusterTreeNodeLabel> ListBucket::Select(ResourceDescriptor const &resource,
                                                       unsigned rank, unsigned num_failures) const {
    if (child_labels_.empty()) {
        return std::nullopt;
    }

    unsigned jumps = rank + num_failures;

    std::vector<float> scores = scorer_->Score(children_capabilities_, resource);
    if (scores.empty()) {
        // None of the children is able to satisfy the resource.
        return std::nullopt;
    }

    // Assume each child is selected with the probability of its score, and we have n children in
    // the list, each with scores: p[0], p[1], ..., p[n]. We want to select one of the children
    // starting from the front of the list. We should conduct the ith selection with probaility:
    // p[i]/(1 - sum(p[0:i])).
    // To see why, we know the first item must be selected with probability p[0]. And we assume the
    // kth selection is conducted with probability p[k]/(1 - sum(p[0:k])) for 0 < k < i. Let the
    // probability of the ith selection be x, we have:
    // (1 - p[0])*(1 - p[1]/(1 - p[0])*(1 - p[2]/(1 - p[0] - p[1] - p[2]) ... *
    // (1 - p[i-1]/(1 - sum(p[0:i-1]))) * x = p[i]
    //
    // Simplifying the left hande side yields:
    // (1 - p[0] - p[1] - ... - p[i-1]) * x = p[i]
    // Therefore,
    // x = p[i]/(1 - p[0:i]).
    // At this point, we have shown the ith selection probability inductively.
    float sum_p = 0.0f;

    for (unsigned i = 0; i < child_labels_.size(); ++i) {
        std::string hash_key = resource.key + std::to_string(jumps) + child_labels_[i];
        size_t random_variable = std::hash<std::string>()(hash_key);

        float p;
        if (sum_p < 1.0f) {
            p = scores[i] / (1.0f - sum_p);
        } else {
            p = 0.0f;
        }

        if (EnterWithProbability(random_variable, p)) {
            return child_labels_[i];
        }

        sum_p += p;
    }

    return *child_labels_.rbegin();
}

bool ListBucket::AddChild(ClusterTreeNodeLabel const &child_label) {
    assert(child_labels_.size() == children_capabilities_.size());

    if (std::find(child_labels_.begin(), child_labels_.end(), child_label) != child_labels_.end()) {
        return false;
    }

    child_labels_.insert(child_labels_.begin(), child_label);
    children_capabilities_.insert(children_capabilities_.begin(), WeightedCapabilities());
    return true;
}

bool ListBucket::RemoveChild(ClusterTreeNodeLabel const &child_label) {
    assert(child_labels_.size() == children_capabilities_.size());

    auto it = std::find(child_labels_.begin(), child_labels_.end(), child_label);
    if (it == child_labels_.end()) {
        return false;
    }

    children_capabilities_.erase(children_capabilities_.begin() + (it - child_labels_.begin()));
    child_labels_.erase(it);
    return true;
}

std::vector<BucketInterface::Child> ListBucket::Children() const {
    assert(child_labels_.size() == children_capabilities_.size());

    std::vector<BucketInterface::Child> children;
    for (unsigned i = 0; i < child_labels_.size(); ++i) {
        children.push_back(Child(child_labels_[i], children_capabilities_[i]));
    }

    return children;
}

void ListBucket::AddCapabilitiesFor(ClusterTreeNodeLabel const &child_label,
                                    WeightedCapabilities const &delta) {
    auto it = std::find(child_labels_.begin(), child_labels_.end(), child_label);
    assert(it != child_labels_.end());

    AddCapabilities(delta, &children_capabilities_[it - child_labels_.begin()]);
}

Bucket ListBucket::ToProto() const {
    Bucket bucket;

    for (auto const &child_label : child_labels_) {
        *bucket.mutable_list_bucket()->add_child_labels() = child_label;
    }

    return bucket;
}

std::unique_ptr<BucketInterface> CreateBucket(Bucket const &proto,
                                              std::unique_ptr<CapabilityScoreInterface> &&scorer) {
    switch (proto.children_case()) {
    case Bucket::ChildrenCase::kUniformBucket: {
        return std::make_unique<UniformBucket>(proto.uniform_bucket());
    }
    case Bucket::ChildrenCase::kListBucket: {
        return std::make_unique<ListBucket>(proto.list_bucket(), std::move(scorer));
    }
    case Bucket::ChildrenCase::kTreeBucket: {
        // TODO: Implements tree bucket.
        assert(false);
    }
    case Bucket::ChildrenCase::kStrawBucket: {
        // TODO: Implements straw bucket.
        assert(false);
    }
    case Bucket::ChildrenCase::CHILDREN_NOT_SET:
    default: {
        assert(false);
    }
    }
}

} // namespace e8
