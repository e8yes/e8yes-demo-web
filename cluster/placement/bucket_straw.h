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

#ifndef PLACEMENT_BUCKET_STRAW_H
#define PLACEMENT_BUCKET_STRAW_H

#include <memory>
#include <optional>
#include <vector>

#include "cluster/placement/bucket.h"
#include "cluster/placement/common_types.h"
#include "cluster/placement/score.h"
#include "proto_cc/bucket.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

/**
 * @brief The StrawBucket class A straw bucket views each child element as a straw with length
 * proportional to its hardware capabilities with respect to a resource. The straw's actual length
 * is perturbed by a deterministic pseudo-random variable. The longest straw gets picked up. The
 * selection performance is low because it always has to scan through all the straw to compute the
 * straw lengths to figure out which one is the longest. However, adding and removing child element
 * results in minimal resource movement.
 */
class StrawBucket : public BucketInterface {
  public:
    /**
     * @brief StrawBucket Constructs a straw bucket from the StrawBucketData proto and a custom
     * scorer. The scorer helps convert children's capabilities with respect to a particular
     * resource to a distribution which the Select() function places a resource pseudo-randomly
     * with.
     */
    StrawBucket(StrawBucketData const &data, std::unique_ptr<CapabilityScoreInterface> &&scorer);
    ~StrawBucket();

    std::optional<ClusterTreeNodeLabel> Select(ResourceDescriptor const &resource, unsigned rank,
                                               unsigned num_failures) const override;

    bool AddChild(ClusterTreeNodeLabel const &child_label) override;

    bool RemoveChild(ClusterTreeNodeLabel const &child_label) override;

    std::vector<Child> Children() const override;

    void AddCapabilitiesFor(ClusterTreeNodeLabel const &child_label,
                            WeightedCapabilities const &delta) override;

    Bucket ToProto() const override;

  private:
    std::unique_ptr<CapabilityScoreInterface> scorer_;
    StrawBucketData::WeightFunction weight_function_;
    std::vector<ClusterTreeNodeLabel> child_labels_;
    std::vector<WeightedCapabilities> children_capabilities_;
};

} // namespace e8

#endif // PLACEMENT_BUCKET_STRAW_H
