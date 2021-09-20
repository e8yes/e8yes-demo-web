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

#ifndef PLACEMENT_BUCKET_UNIFORM_H
#define PLACEMENT_BUCKET_UNIFORM_H

#include <google/protobuf/repeated_field.h>
#include <optional>
#include <vector>

#include "cluster/placement/bucket.h"
#include "cluster/placement/common_types.h"
#include "proto_cc/bucket.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

/**
 * @brief The UniformBucket class It distributes resources uniformly across the children without
 * considering their hardware capabilities. It's capable of achieving O(1) time select. However,
 * adding or removing any children will result in a complete reshuffling of the resources that live
 * under this bucket. This class isn't thread safe.
 */
class UniformBucket : public BucketInterface {
  public:
    UniformBucket(
        UniformBucketData const &data,
        google::protobuf::RepeatedPtrField<ClusterTreeNodeNamespace> const &supported_namespaces);
    ~UniformBucket() override;

    std::optional<ClusterTreeNodeLabel> Select(ResourceDescriptor const &resource, unsigned rank,
                                               unsigned num_failures) const override;

    bool AddChild(ClusterTreeNodeLabel const &child_label) override;

    bool RemoveChild(ClusterTreeNodeLabel const &child_label) override;

    std::vector<Child> Children() const override;

    void AddCapabilitiesFor(ClusterTreeNodeLabel const &child_label,
                            WeightedCapabilities const &delta) override;

    Bucket ToProto() const override;

  private:
    std::vector<Child> children_;
    unsigned prime_;
};

} // namespace e8

#endif // PLACEMENT_BUCKET_UNIFORM_H
