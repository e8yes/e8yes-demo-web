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
WeightedCapabilities::FixedPoint GetCapabilityByType(WeightedCapabilities::Type type,
                                                     WeightedCapabilities const &capabilities);

/**
 * @brief SetCapabilityByType Sets the value of a specific capability by type.
 */
void SetCapabilityByType(WeightedCapabilities::Type type,
                         WeightedCapabilities::FixedPoint const &value,
                         WeightedCapabilities *capabilities);

/**
 * @brief WeightedCapabilityTypes Returns a list of all capability types.
 */
std::vector<WeightedCapabilities::Type> WeightedCapabilityTypes();

/**
 * @brief operator + Calulates the sum of two capabilities by adding each type's values
 * individually.
 */
WeightedCapabilities operator+(WeightedCapabilities const &a, WeightedCapabilities const &b);

/**
 * @brief operator += Addition assignment.
 */
WeightedCapabilities &operator+=(WeightedCapabilities &a, WeightedCapabilities const &b);

/**
 * @brief operator - Calulates the difference of two capabilities by adding each type's values
 * individually.
 */
WeightedCapabilities operator-(WeightedCapabilities const &a, WeightedCapabilities const &b);

/**
 * @brief operator -= Subtraction assignment.
 */
WeightedCapabilities &operator-=(WeightedCapabilities &a, WeightedCapabilities const &b);

/**
 * @brief operator - Negates the capability values individually.
 */
WeightedCapabilities operator-(WeightedCapabilities const &a);

/**
 * @brief CapabilityFixedPointFromFloat Creates a fixed point representation from a floating point
 * value.
 */
WeightedCapabilities::FixedPoint CapabilityFixedPointFromFloat(float value);

/**
 * @brief ToFloat Converts a fixed point representation to a floating point value.
 */
float ToFloat(WeightedCapabilities::FixedPoint const &value);

/**
 * @brief operator + Adds two fixed point values.
 */
WeightedCapabilities::FixedPoint operator+(WeightedCapabilities::FixedPoint const &a,
                                           WeightedCapabilities::FixedPoint const &b);

/**
 * @brief operator - Subtracts two fixed point values.
 */
WeightedCapabilities::FixedPoint operator-(WeightedCapabilities::FixedPoint const &a,
                                           WeightedCapabilities::FixedPoint const &b);

/**
 * @brief operator - Negates a fixed point value.
 */
WeightedCapabilities::FixedPoint operator-(WeightedCapabilities::FixedPoint const &a);

/**
 * @brief operator < Compares if a is less than b in fixed point.
 */
bool operator<(WeightedCapabilities::FixedPoint const &a,
               WeightedCapabilities::FixedPoint const &b);

/**
 * @brief operator <= Compares if a is less than or equals to b in fixed point.
 */
bool operator<=(WeightedCapabilities::FixedPoint const &a,
                WeightedCapabilities::FixedPoint const &b);

/**
 * @brief operator == Compares if a is equal to b in fixed point.
 */
bool operator==(WeightedCapabilities::FixedPoint const &a,
                WeightedCapabilities::FixedPoint const &b);

/**
 * @brief operator != Compares if a is different from b in fixed point.
 */
bool operator!=(WeightedCapabilities::FixedPoint const &a,
                WeightedCapabilities::FixedPoint const &b);

/**
 * @brief operator > Compares if a is greater than b in fixed point.
 */
bool operator>(WeightedCapabilities::FixedPoint const &a,
               WeightedCapabilities::FixedPoint const &b);

/**
 * @brief operator >= Compares if a is greater than or equals to b in fixed point.
 */
bool operator>=(WeightedCapabilities::FixedPoint const &a,
                WeightedCapabilities::FixedPoint const &b);

} // namespace e8

#endif // PLACEMENT_CAPABILITY_H
