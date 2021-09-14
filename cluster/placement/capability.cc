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
#include <unordered_map>
#include <utility>

#include "cluster/placement/capability.h"
#include "cluster/placement/common_types.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

float GetCapabilityByType(WeightedCapabilities::Type type,
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

void SetCapabilityByType(WeightedCapabilities::Type type, float value,
                         WeightedCapabilities *capabilities) {
    switch (type) {
    case WeightedCapabilities::CPU:
        capabilities->set_cpu(value);
        break;
    case WeightedCapabilities::RAM:
        capabilities->set_ram(value);
        break;
    case WeightedCapabilities::STORAGE:
        capabilities->set_storage(value);
        break;
    case WeightedCapabilities::CORAL:
        capabilities->set_coral(value);
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

} // namespace e8
