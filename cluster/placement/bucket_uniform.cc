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
#include <google/protobuf/repeated_field.h>
#include <optional>
#include <string>
#include <vector>

#include "cluster/placement/bucket.h"
#include "cluster/placement/bucket_uniform.h"
#include "cluster/placement/capability.h"
#include "cluster/placement/common_types.h"
#include "proto_cc/bucket.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

UniformBucket::UniformBucket(
    UniformBucketData const &data,
    google::protobuf::RepeatedPtrField<ClusterTreeNodeNamespace> const &supported_namespaces)
    : BucketInterface(supported_namespaces), prime_(data.prime()) {
    for (auto const &child_label : data.child_labels()) {
        children_.push_back(Child(child_label, WeightedCapabilities()));
    }
}

UniformBucket::~UniformBucket() {}

std::optional<ClusterTreeNodeLabel> UniformBucket::Select(ResourceDescriptor const &resource,
                                                          unsigned rank,
                                                          unsigned num_failures) const {
    if (!this->SupportNameSpace(resource.name_space)) {
        return std::nullopt;
    }

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

    it->capabilities += delta;
}

Bucket UniformBucket::ToProto() const {
    Bucket bucket;
    bucket.mutable_uniform_bucket()->set_prime(prime_);

    for (auto const &child : children_) {
        *bucket.mutable_uniform_bucket()->add_child_labels() = child.label;
    }

    return bucket;
}

} // namespace e8
