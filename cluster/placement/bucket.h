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

#ifndef PLACEMENT_BUCKET_H
#define PLACEMENT_BUCKET_H

#include <memory>
#include <optional>

#include "cluster/placement/capability.h"
#include "cluster/placement/common_types.h"
#include "cluster/placement/score.h"
#include "proto_cc/bucket.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

/**
 * @brief The BucketInterface class A logical grouping element. It allows the client of the class to
 * quickly place/locate a resource in a group of child nodes.
 */
class BucketInterface {
  public:
    BucketInterface();
    virtual ~BucketInterface();

    /**
     * @brief Select Selects a tree node from the bucket for the placement or location of the
     * specified resource of particular rank (replica). The selection process distributes the
     * resource pseudo-probabilistically based on the its most outstanding required capability to
     * one of the children. Changing the parameter num_failures may or may not make the function
     * return a different child. The funciton returns a std::nullopt only when the bucket is empty
     * or it couldn't find any suitable children for the resource.
     */
    virtual std::optional<ClusterTreeNodeLabel>
    Select(ResourceDescriptor const &resource, unsigned rank, unsigned num_failures,
           ClusterCapability const &cluster_capabilities) const = 0;

    /**
     * @brief AddChild Adds a child node to the bucket.
     *
     * @param child_label The label of the child node to be added.
     * @param child_weight The capability weights the child node has.
     * @return True if the child node has not already existed.
     */
    virtual bool AddChild(ClusterTreeNodeLabel const &child_label) = 0;

    /**
     * @brief RemoveChild Removes a node from the bucket.
     *
     * @param child_label The label of the child node to be removed.
     * @return True if the child node does exist in the bucket.
     */
    virtual bool RemoveChild(ClusterTreeNodeLabel const &child_label) = 0;

    /**
     * @brief Children Returns a list of children the bucket currently has.
     */
    virtual std::vector<ClusterTreeNodeLabel> Children() const = 0;

    /**
     * @brief ToProto Exports the bucket data structure as a Bucket proto.
     */
    virtual Bucket ToProto() const = 0;
};

/**
 * @brief The UniformBucket class Distributes resources uniformly across the children without
 * consideration of their hardware capabilities. It's capable of achieving O(1) time select.
 * However, adding or removing any children will result in a complete reshuffling of the resources
 * that live under this bucket.
 */
class UniformBucket : public BucketInterface {
  public:
    UniformBucket(UniformBucketData const &data);
    ~UniformBucket() override;

    std::optional<ClusterTreeNodeLabel>
    Select(ResourceDescriptor const &resource, unsigned rank, unsigned num_failures,
           ClusterCapability const &cluster_capabilities) const override;

    bool AddChild(ClusterTreeNodeLabel const &child_label) override;

    bool RemoveChild(ClusterTreeNodeLabel const &child_label) override;

    std::vector<ClusterTreeNodeLabel> Children() const override;

    Bucket ToProto() const override;

  private:
    UniformBucketData data_;
};

/**
 * @brief The ListBucket class Distributes resources according to the distribution of the children's
 * hardware capabilities. It takes O(n) time to select a tree node for a bucket with n children.
 * Adding children requires optimal resource movement. However, removing children in the middle of
 * the list results in large amount of shuffling.
 */
class ListBucket : public BucketInterface {
  public:
    /**
     * @brief ListBucket Constructs a list bucket from the ListBucketData proto and a custom scorer.
     * The scorer helps convert children's capabilities to a distribution which the Select()
     * function places a resource pseudo-randomly with.
     */
    ListBucket(ListBucketData const &data, std::unique_ptr<CapabilityScoreInterface> &&scorer);
    ~ListBucket() override;

    std::optional<ClusterTreeNodeLabel>
    Select(ResourceDescriptor const &resource, unsigned rank, unsigned num_failures,
           ClusterCapability const &cluster_capabilities) const override;

    bool AddChild(ClusterTreeNodeLabel const &child_label) override;

    bool RemoveChild(ClusterTreeNodeLabel const &child_label) override;

    std::vector<ClusterTreeNodeLabel> Children() const override;

    Bucket ToProto() const override;

  private:
    ListBucketData data_;
    std::unique_ptr<CapabilityScoreInterface> scorer_;
};

/**
 * @brief CreateBucket Creates a bucket with the specified proto and scorer. The bucket
 * implementation is chosen based on the proto.
 */
std::unique_ptr<BucketInterface> CreateBucket(Bucket const &proto,
                                              std::unique_ptr<CapabilityScoreInterface> &&scorer);

} // namespace e8

#endif // PLACEMENT_BUCKET_H
