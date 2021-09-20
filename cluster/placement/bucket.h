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

#include <google/protobuf/repeated_field.h>
#include <list>
#include <memory>
#include <optional>
#include <unordered_set>
#include <vector>

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
    /**
     * @brief The Child struct
     */
    struct Child {
        Child(ClusterTreeNodeLabel const &label, WeightedCapabilities const &capabilities);

        ClusterTreeNodeLabel label;
        WeightedCapabilities capabilities;
    };

    /**
     * @brief BucketInterface
     * @param supported_namespaces
     */
    BucketInterface(
        google::protobuf::RepeatedPtrField<ClusterTreeNodeNamespace> const &supported_namespaces);
    virtual ~BucketInterface();

    /**
     * @brief Select Selects a tree node from the bucket for the placement or location of the
     * specified resource of particular rank (replica). The selection process distributes the
     * resource pseudo-probabilistically based on the distribution of capability scores over the
     * children (see CapabilityScoreInterface for how the scores is calculated). Changing the
     * parameter num_failures may or may not make the function return a different child. The
     * funciton returns a std::nullopt only when the bucket is empty or it couldn't find any
     * suitable children for the resource.
     */
    virtual std::optional<ClusterTreeNodeLabel>
    Select(ResourceDescriptor const &resource, unsigned rank, unsigned num_failures) const = 0;

    /**
     * @brief AddChild Adds a child node with zero capabilities to the bucket.
     *
     * @param child_label Label of the child to be added.
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
    virtual std::vector<Child> Children() const = 0;

    /**
     * @brief AddCapabilitiesFor Adds the capability deltas to one of its children. The function
     * will fail if the specified child doesn't exist.
     *
     * @param child_label Label of the child to be added delta to.
     * @param delta The delta capability values to be added to the child.
     */
    virtual void AddCapabilitiesFor(ClusterTreeNodeLabel const &child_label,
                                    WeightedCapabilities const &delta) = 0;

    /**
     * @brief ToProto Exports the bucket data structure as a Bucket proto.
     */
    virtual Bucket ToProto() const = 0;

  protected:
    /**
     * @brief SupportNamespace
     */
    bool SupportNameSpace(std::optional<ClusterTreeNodeNamespace> const &name_space) const;

  private:
    std::unordered_set<ClusterTreeNodeNamespace> supported_names_paces_;
};

/**
 * @brief CreateBucket Creates a bucket with the specified proto and scorer. The bucket
 * implementation is chosen based on the proto.
 */
std::unique_ptr<BucketInterface> CreateBucket(Bucket const &proto,
                                              std::unique_ptr<CapabilityScoreInterface> &&scorer);

} // namespace e8

#endif // PLACEMENT_BUCKET_H
