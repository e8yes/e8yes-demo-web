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
#include "cluster/placement/bucket_straw.h"
#include "cluster/placement/capability.h"
#include "cluster/placement/common_types.h"
#include "cluster/placement/score.h"
#include "proto_cc/bucket.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

StrawBucket::StrawBucket(StrawBucketData const &data,
                         std::unique_ptr<CapabilityScoreInterface> &&scorer)
    : scorer_(std::move(scorer)), weight_function_(data.weight_function()),
      child_labels_(data.child_labels_size()), children_capabilities_(data.child_labels_size()) {

    for (auto const &child_label : data.child_labels()) {
        child_labels_.push_back(child_label);
    }
}

StrawBucket::~StrawBucket() {}

std::optional<ClusterTreeNodeLabel> StrawBucket::Select(ResourceDescriptor const &resource,
                                                        unsigned rank,
                                                        unsigned num_failures) const {
    if (child_labels_.empty()) {
        return std::nullopt;
    }

    std::vector<float> scores = scorer_->Score(children_capabilities_, resource);
    if (scores.empty()) {
        // None of the children is able to satisfy with the resource's requirement.
        return std::nullopt;
    }

    std::string jumps = std::to_string(rank + num_failures);
    std::string key_and_jumps = resource.key + jumps;

    // Let the straws (random variables scaled by capability scores) compete against each other.
    // The longest straw gets selected.
    unsigned longest_straw = 0;
    float max_straw_length = 0.0f;
    for (unsigned i = 0; i < scores.size(); ++i) {
        float straw_length;

        size_t random_variable = std::hash<std::string>()(key_and_jumps + child_labels_[i]);
        switch (weight_function_) {
        case StrawBucketData::LINEAR: {
            straw_length = scores[i] * random_variable;
            break;
        }
        case StrawBucketData::INVALID_FUNCTION:
        default: {
            assert(false);
        }
        }

        if (straw_length >= max_straw_length) {
            max_straw_length = straw_length;
            longest_straw = i;
        }
    }

    return child_labels_[longest_straw];
}

bool StrawBucket::AddChild(ClusterTreeNodeLabel const &child_label) {
    assert(child_labels_.size() == children_capabilities_.size());

    if (std::find(child_labels_.begin(), child_labels_.end(), child_label) != child_labels_.end()) {
        return false;
    }

    child_labels_.insert(child_labels_.begin(), child_label);
    children_capabilities_.insert(children_capabilities_.begin(), WeightedCapabilities());
    return true;
}

bool StrawBucket::RemoveChild(ClusterTreeNodeLabel const &child_label) {
    assert(child_labels_.size() == children_capabilities_.size());

    auto it = std::find(child_labels_.begin(), child_labels_.end(), child_label);
    if (it == child_labels_.end()) {
        return false;
    }

    children_capabilities_.erase(children_capabilities_.begin() + (it - child_labels_.begin()));
    child_labels_.erase(it);
    return true;
}

std::vector<BucketInterface::Child> StrawBucket::Children() const {
    assert(child_labels_.size() == children_capabilities_.size());

    std::vector<BucketInterface::Child> children;
    for (unsigned i = 0; i < child_labels_.size(); ++i) {
        children.push_back(Child(child_labels_[i], children_capabilities_[i]));
    }

    return children;
}

void StrawBucket::AddCapabilitiesFor(ClusterTreeNodeLabel const &child_label,
                                     WeightedCapabilities const &delta) {
    auto it = std::find(child_labels_.begin(), child_labels_.end(), child_label);
    assert(it != child_labels_.end());

    children_capabilities_[it - child_labels_.begin()] += delta;
}

Bucket StrawBucket::ToProto() const {
    Bucket bucket;
    bucket.mutable_straw_bucket()->set_weight_function(weight_function_);

    for (auto const &child_label : child_labels_) {
        *bucket.mutable_straw_bucket()->add_child_labels() = child_label;
    }

    return bucket;
}

} // namespace e8
