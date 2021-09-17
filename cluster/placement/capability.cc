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
#include <cstdint>
#include <unordered_map>
#include <utility>

#include "cluster/placement/capability.h"
#include "cluster/placement/common_types.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

WeightedCapabilities::FixedPoint GetCapabilityByType(WeightedCapabilities::Type type,
                                                     WeightedCapabilities const &capabilities) {
    switch (type) {
    case WeightedCapabilities::CPU:
        return capabilities.cpu();
    case WeightedCapabilities::RAM:
        return capabilities.ram();
    case WeightedCapabilities::STORAGE:
        return capabilities.storage();
    case WeightedCapabilities::CORAL:
        return capabilities.coral();
    default:
        assert(false);
    }
}

void SetCapabilityByType(WeightedCapabilities::Type type,
                         WeightedCapabilities::FixedPoint const &value,
                         WeightedCapabilities *capabilities) {
    switch (type) {
    case WeightedCapabilities::CPU:
        if (!capabilities->has_cpu() && value.integer_and_fraction() == 0) {
            break;
        }

        *capabilities->mutable_cpu() = value;
        break;
    case WeightedCapabilities::RAM:
        if (!capabilities->has_ram() && value.integer_and_fraction() == 0) {
            break;
        }

        *capabilities->mutable_ram() = value;
        break;
    case WeightedCapabilities::STORAGE:
        if (!capabilities->has_storage() && value.integer_and_fraction() == 0) {
            break;
        }

        *capabilities->mutable_storage() = value;
        break;
    case WeightedCapabilities::CORAL:
        if (!capabilities->has_coral() && value.integer_and_fraction() == 0) {
            break;
        }

        *capabilities->mutable_coral() = value;
        break;
    default:
        assert(false);
    }
}

std::vector<WeightedCapabilities::Type> WeightedCapabilityTypes() {
    return std::vector<WeightedCapabilities::Type>{
        WeightedCapabilities::CPU, WeightedCapabilities::RAM, WeightedCapabilities::STORAGE,
        WeightedCapabilities::CORAL};
}

WeightedCapabilities operator+(WeightedCapabilities const &a, WeightedCapabilities const &b) {
    WeightedCapabilities result;
    for (auto type : WeightedCapabilityTypes()) {
        WeightedCapabilities::FixedPoint a_value = GetCapabilityByType(type, a);
        WeightedCapabilities::FixedPoint b_value = GetCapabilityByType(type, b);
        SetCapabilityByType(type, a_value + b_value, &result);
    }
    return result;
}

WeightedCapabilities &operator+=(WeightedCapabilities &a, WeightedCapabilities const &b) {
    a = a + b;
    return a;
}

WeightedCapabilities operator-(WeightedCapabilities const &a, WeightedCapabilities const &b) {
    WeightedCapabilities result;
    for (auto type : WeightedCapabilityTypes()) {
        WeightedCapabilities::FixedPoint a_value = GetCapabilityByType(type, a);
        WeightedCapabilities::FixedPoint b_value = GetCapabilityByType(type, b);
        SetCapabilityByType(type, a_value - b_value, &result);
    }
    return result;
}

WeightedCapabilities &operator-=(WeightedCapabilities &a, WeightedCapabilities const &b) {
    a = a - b;
    return a;
}

WeightedCapabilities operator-(WeightedCapabilities const &a) {
    WeightedCapabilities result;
    for (auto type : WeightedCapabilityTypes()) {
        WeightedCapabilities::FixedPoint a_value = GetCapabilityByType(type, a);
        SetCapabilityByType(type, -a_value, &result);
    }
    return result;
}

WeightedCapabilities::FixedPoint CapabilityFixedPointFromFloat(float value) {
    WeightedCapabilities::FixedPoint fp;
    fp.set_integer_and_fraction(static_cast<int64_t>(value * 10000.0));
    return fp;
}

float ToFloat(WeightedCapabilities::FixedPoint const &value) {
    return value.integer_and_fraction() / 10000.0;
}

WeightedCapabilities::FixedPoint operator+(WeightedCapabilities::FixedPoint const &a,
                                           WeightedCapabilities::FixedPoint const &b) {
    WeightedCapabilities::FixedPoint result;
    result.set_integer_and_fraction(a.integer_and_fraction() + b.integer_and_fraction());
    return result;
}

WeightedCapabilities::FixedPoint operator-(WeightedCapabilities::FixedPoint const &a,
                                           WeightedCapabilities::FixedPoint const &b) {
    WeightedCapabilities::FixedPoint result;
    result.set_integer_and_fraction(a.integer_and_fraction() - b.integer_and_fraction());
    return result;
}

WeightedCapabilities::FixedPoint operator-(WeightedCapabilities::FixedPoint const &a) {
    WeightedCapabilities::FixedPoint negated;
    negated.set_integer_and_fraction(-a.integer_and_fraction());
    return negated;
}

bool operator<(WeightedCapabilities::FixedPoint const &a,
               WeightedCapabilities::FixedPoint const &b) {
    return a.integer_and_fraction() < b.integer_and_fraction();
}

bool operator<=(WeightedCapabilities::FixedPoint const &a,
                WeightedCapabilities::FixedPoint const &b) {
    return a.integer_and_fraction() < b.integer_and_fraction();
}

bool operator==(WeightedCapabilities::FixedPoint const &a,
                WeightedCapabilities::FixedPoint const &b) {
    return a.integer_and_fraction() == b.integer_and_fraction();
}

bool operator>(WeightedCapabilities::FixedPoint const &a,
               WeightedCapabilities::FixedPoint const &b) {
    return a.integer_and_fraction() > b.integer_and_fraction();
}

bool operator>=(WeightedCapabilities::FixedPoint const &a,
                WeightedCapabilities::FixedPoint const &b) {
    return a.integer_and_fraction() >= b.integer_and_fraction();
}

} // namespace e8
