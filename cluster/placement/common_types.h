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

#ifndef PLACEMENT_COMMON_TYPES
#define PLACEMENT_COMMON_TYPES

#include <cstdint>
#include <string>
#include <vector>

#include "proto_cc/machine.pb.h"

namespace e8 {

// The unique key of a particular resource.
using ResourceKey = std::string;

// The unique label of either a bucket or machine.
using ClusterTreeNodeLabel = std::string;

// The replica number of a particular resource. The 0th replica is the original copy. Negative ranks
// are reserved for error indication.
using ReplicationRank = int32_t;

/**
 * @brief The ResourceDescriptor struct Describes information about a keyed resource.
 */
struct ResourceDescriptor {
    ResourceDescriptor(ResourceKey const &key) : key(key) {}

    // The key of the resource. It must be unique among all resource workers within the cluster
    // that manages this type of resource.
    ResourceKey const key;

    // The hardware capabilities this resource requires.
    WeightedCapabilities required_capabilities;
};

// The version of a cluster map description.
using ClusterMapVersionEpoch = int64_t;

} // namespace e8

#endif // PLACEMENT_COMMON_TYPES
