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

#include <optional>

#include "cluster/placement/common_types.h"
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
     * @brief Select Selects a tree node from the bucket for the allocation or location of the
     * specified resource. The selection process distributes the resource pseudo-probabilistically
     * based on the its most outstanding required capability to one of the children. Changing
     * the parameter num_failures may or may not make the function return a different child. The
     * funciton returns a std::nullopt only when the bucket is empty.
     */
    virtual std::optional<ClusterTreeNodeLabel> Select(ResourceDescriptor const &resource,
                                                       unsigned num_failures) = 0;

    /**
     * @brief AddChild Adds a child node to the bucket.
     *
     * @param label The label of the child node to be added.
     * @param child_weight The capability weights the child node has.
     * @return True if the child node has not already existed.
     */
    virtual bool AddChild(ClusterTreeNodeLabel const &label,
                          WeightedCapabilities const &child_weight) = 0;

    /**
     * @brief RemoveChild Removes a node from the bucket.
     *
     * @param label The label of the child node to be removed.
     * @return True if the child node does exist in the bucket.
     */
    virtual bool RemoveChild(ClusterTreeNodeLabel const &label) = 0;

    /**
     * @brief ToProto Exports the bucket data structure as a Bucket proto.
     */
    virtual Bucket const &ToProto() const = 0;
};

} // namespace e8

#endif // PLACEMENT_BUCKET_H
