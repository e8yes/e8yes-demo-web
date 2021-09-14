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

#ifndef PLACEMENT_CAPABILITY_H
#define PLACEMENT_CAPABILITY_H

#include <unordered_map>

#include "cluster/placement/common_types.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

/**
 * @brief GetCapabilityByType Gets the value of a specific capability by type.
 */
float GetCapabilityByType(WeightedCapabilities::Type type,
                          WeightedCapabilities const &capabilities);

/**
 * @brief SetCapabilityByType Sets the value of a specific capability by type.
 */
void SetCapabilityByType(WeightedCapabilities::Type type, float value,
                         WeightedCapabilities *capabilities);

/**
 * @brief WeightedCapabilityTypes Returns a list of all capability types.
 */
std::vector<WeightedCapabilities::Type> WeightedCapabilityTypes();

} // namespace e8

#endif // PLACEMENT_CAPABILITY_H
