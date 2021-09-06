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
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <vector>

#include "cluster/placement/bucket.h"
#include "cluster/placement/capability.h"
#include "cluster/placement/common_types.h"
#include "proto_cc/bucket.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {
namespace {

class SharedLockGuard {
  public:
    SharedLockGuard(std::shared_mutex &mu);
    ~SharedLockGuard();

  private:
    std::shared_mutex &mu_;
};

SharedLockGuard::SharedLockGuard(std::shared_mutex &mu) : mu_(mu) { mu_.lock_shared(); }

SharedLockGuard::~SharedLockGuard() { mu_.unlock_shared(); }

bool EnterWithProbability(size_t hash, float p) { return hash % 16384 < p * 16384; }

} // namespace

BucketInterface::BucketInterface() {}

BucketInterface::~BucketInterface() {}

UniformBucket::UniformBucket(UniformBucketData const &data)
    : data_(data), mu_(std::make_unique<std::shared_mutex>()) {}

UniformBucket::~UniformBucket() {}

std::optional<ClusterTreeNodeLabel>
UniformBucket::Select(ResourceDescriptor const &resource, unsigned num_failures,
                      ClusterCapability const & /*cluster_capabilities*/) const {
    SharedLockGuard guard(*mu_);

    if (data_.child_labels().empty()) {
        return std::nullopt;
    }

    unsigned jumps = resource.rank + num_failures;
    size_t final_hash = std::hash<std::string>()(resource.key) + jumps * data_.prime();
    unsigned location = final_hash % data_.child_labels().size();

    return data_.child_labels(location);
}

bool UniformBucket::AddChild(ClusterTreeNodeLabel const &child_label) {
    std::lock_guard guard(*mu_);

    auto it = std::find(data_.child_labels().begin(), data_.child_labels().end(), child_label);
    if (it != data_.child_labels().end()) {
        return false;
    }

    *data_.mutable_child_labels()->Add() = child_label;
    return true;
}

bool UniformBucket::RemoveChild(ClusterTreeNodeLabel const &child_label) {
    std::lock_guard guard(*mu_);

    auto it = std::find(data_.child_labels().begin(), data_.child_labels().end(), child_label);
    if (it == data_.child_labels().end()) {
        return false;
    }

    data_.mutable_child_labels()->erase(it);
    return true;
}

Bucket UniformBucket::ToProto() const {
    SharedLockGuard guard(*mu_);

    Bucket bucket;
    *bucket.mutable_uniform_bucket() = data_;
    return bucket;
}

ListBucket::ListBucket(ListBucketData const &data,
                       std::unique_ptr<CapabilityScoreInterface> &&scorer)
    : data_(data), scorer_(std::move(scorer)), mu_(std::make_unique<std::shared_mutex>()) {}

ListBucket::~ListBucket() {}

std::optional<ClusterTreeNodeLabel>
ListBucket::Select(ResourceDescriptor const &resource, unsigned num_failures,
                   ClusterCapability const &cluster_capabilities) const {
    SharedLockGuard guard(*mu_);

    if (data_.child_labels().empty()) {
        return std::nullopt;
    }

    unsigned jumps = resource.rank + num_failures;

    std::vector<WeightedCapabilities> children_capabilities(data_.child_labels_size());
    std::transform(
        data_.child_labels().begin(), data_.child_labels().end(), children_capabilities.begin(),
        [&cluster_capabilities](ClusterTreeNodeLabel const &child_label) -> WeightedCapabilities {
            return cluster_capabilities.Capability(child_label);
        });

    std::vector<float> scores = scorer_->Score(children_capabilities, resource);
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

    for (int i = 0; i < data_.child_labels_size(); ++i) {
        std::string hash_key = resource.key + std::to_string(jumps) + data_.child_labels(i);
        size_t random_variable = std::hash<std::string>()(hash_key);

        float p;
        if (sum_p < 1.0f) {
            p = scores[i] / (1.0f - sum_p);
        } else {
            p = 0.0f;
        }

        if (EnterWithProbability(random_variable, p)) {
            return data_.child_labels(i);
        }

        sum_p += p;
    }

    return *data_.child_labels().rbegin();
}

bool ListBucket::AddChild(ClusterTreeNodeLabel const &child_label) {
    std::lock_guard guard(*mu_);

    auto it = std::find(data_.child_labels().begin(), data_.child_labels().end(), child_label);
    if (it != data_.child_labels().end()) {
        return false;
    }

    data_.mutable_child_labels()->Add();

    for (int i = data_.child_labels_size() - 1; i >= 1; --i) {
        *data_.mutable_child_labels(i) = data_.child_labels(i - 1);
    }

    *data_.mutable_child_labels(0) = child_label;

    return true;
}

bool ListBucket::RemoveChild(ClusterTreeNodeLabel const &child_label) {
    std::lock_guard guard(*mu_);

    auto it = std::find(data_.child_labels().begin(), data_.child_labels().end(), child_label);
    if (it == data_.child_labels().end()) {
        return false;
    }

    data_.mutable_child_labels()->erase(it);
    return true;
}

Bucket ListBucket::ToProto() const {
    SharedLockGuard guard(*mu_);

    Bucket bucket;
    *bucket.mutable_list_bucket() = data_;
    return bucket;
}

} // namespace e8
