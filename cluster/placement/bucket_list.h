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

#ifndef PLACEMENT_BUCKET_LIST_H
#define PLACEMENT_BUCKET_LIST_H

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
 * @brief The ListBucket class A list bucket organizes child elements into a linked list and scans
 * through the list (in the worst case) to place a resource. So, it's capable of distributing
 * resources according to children's hardware capabilities. It takes O(n) time to select a child
 * item with n children. Adding children is optimally in terms of resource movement. However,
 * removing children in the middle of the list results in large amount of shuffling. This class
 * isn't thread safe.
 */
class ListBucket : public BucketInterface {
  public:
    /**
     * @brief ListBucket Constructs a list bucket from the ListBucketData proto and a custom scorer.
     * The scorer helps convert children's capabilities with respect to a particular resource to a
     * distribution which the Select() function places a resource pseudo-randomly with.
     */
    ListBucket(ListBucketData const &data, std::unique_ptr<CapabilityScoreInterface> &&scorer);
    ~ListBucket() override;

    std::optional<ClusterTreeNodeLabel> Select(ResourceDescriptor const &resource, unsigned rank,
                                               unsigned num_failures) const override;

    bool AddChild(ClusterTreeNodeLabel const &child_label) override;

    bool RemoveChild(ClusterTreeNodeLabel const &child_label) override;

    std::vector<Child> Children() const override;

    void AddCapabilitiesFor(ClusterTreeNodeLabel const &child_label,
                            WeightedCapabilities const &delta) override;

    Bucket ToProto() const override;

  private:
    std::vector<ClusterTreeNodeLabel> child_labels_;
    std::vector<WeightedCapabilities> children_capabilities_;
    std::unique_ptr<CapabilityScoreInterface> scorer_;
};

} // namespace e8

#endif // PLACEMENT_BUCKET_LIST_H
