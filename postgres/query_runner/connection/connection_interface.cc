/**
 * e8yes demo web server.
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

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/reflection/sql_primitive_interface.h"
#include "postgres/query_runner/resultset/result_set_interface.h"

namespace e8 {

void ConnectionInterface::QueryParams::SetParam(SlotId slot,
                                                std::shared_ptr<SqlPrimitiveInterface> const &val) {
    params_.insert(std::make_pair(slot, val.get()));
    value_storage_.push_back(std::move(val));
}

void ConnectionInterface::QueryParams::SetParamPtr(SlotId slot, SqlPrimitiveInterface const *val) {
    params_.insert(std::make_pair(slot, val));
}

SqlPrimitiveInterface const *ConnectionInterface::QueryParams::GetParam(SlotId slot) const {
    auto it = params_.find(slot);
    if (it != params_.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

void ConnectionInterface::QueryParams::Clear() {
    params_.clear();
    value_storage_.clear();
}

size_t ConnectionInterface::QueryParams::NumSlots() const { return params_.size(); }

ConnectionInterface::QueryParams::SlotId ConnectionInterface::QueryParams::AllocateSlot() {
    return ++next_slot_id_;
}

std::map<ConnectionInterface::QueryParams::SlotId, SqlPrimitiveInterface const *> const &
ConnectionInterface::QueryParams::Parameters() const {
    return params_;
}

} // namespace e8
