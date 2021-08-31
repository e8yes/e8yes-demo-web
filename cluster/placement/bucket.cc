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
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>

#include "cluster/placement/bucket.h"
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

} // namespace

BucketInterface::BucketInterface() {}

BucketInterface::~BucketInterface() {}

UniformBucket::UniformBucket(UniformBucketData const &data)
    : data_(data), mu_(std::make_unique<std::shared_mutex>()) {}

UniformBucket::~UniformBucket() {}

std::optional<ClusterTreeNodeLabel> UniformBucket::Select(ResourceDescriptor const &resource,
                                                          unsigned num_failures) const {
    SharedLockGuard guard(*mu_);

    if (data_.child_labels().empty()) {
        return std::nullopt;
    }

    unsigned jumps = resource.rank + num_failures;
    size_t final_hash = std::hash<std::string>()(resource.key) + jumps * data_.prime();
    unsigned location = final_hash % data_.child_labels().size();

    return data_.child_labels(location);
}

bool UniformBucket::AddChild(ClusterTreeNodeLabel const &label,
                             WeightedCapabilities const & /*child_weight*/) {
    std::lock_guard guard(*mu_);

    auto it = std::find(data_.child_labels().begin(), data_.child_labels().end(), label);
    if (it != data_.child_labels().end()) {
        return false;
    }

    *data_.mutable_child_labels()->Add() = label;
    return true;
}

bool UniformBucket::RemoveChild(ClusterTreeNodeLabel const &label) {
    std::lock_guard guard(*mu_);

    auto it = std::find(data_.child_labels().begin(), data_.child_labels().end(), label);
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

} // namespace e8
