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

#include <cassert>
#include <list>
#include <memory>
#include <optional>
#include <vector>

#include "cluster/placement/bucket.h"
#include "cluster/placement/bucket_list.h"
#include "cluster/placement/bucket_straw.h"
#include "cluster/placement/bucket_tree.h"
#include "cluster/placement/bucket_uniform.h"
#include "cluster/placement/common_types.h"
#include "cluster/placement/score.h"
#include "proto_cc/bucket.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

BucketInterface::Child::Child(ClusterTreeNodeLabel const &label,
                              WeightedCapabilities const &capabilities)
    : label(label), capabilities(capabilities) {}

BucketInterface::BucketInterface() {}

BucketInterface::~BucketInterface() {}

std::unique_ptr<BucketInterface> CreateBucket(Bucket const &proto,
                                              std::unique_ptr<CapabilityScoreInterface> &&scorer) {
    switch (proto.children_case()) {
    case Bucket::ChildrenCase::kUniformBucket: {
        return std::make_unique<UniformBucket>(proto.uniform_bucket());
    }
    case Bucket::ChildrenCase::kListBucket: {
        return std::make_unique<ListBucket>(proto.list_bucket(), std::move(scorer));
    }
    case Bucket::ChildrenCase::kTreeBucket: {
        return std::make_unique<TreeBucket>(proto.tree_bucket(), std::move(scorer));
    }
    case Bucket::ChildrenCase::kStrawBucket: {
        return std::make_unique<StrawBucket>(proto.straw_bucket(), std::move(scorer));
    }
    case Bucket::ChildrenCase::CHILDREN_NOT_SET:
    default: {
        assert(false);
    }
    }
}

} // namespace e8
