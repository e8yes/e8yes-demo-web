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

#include <cassert>
#include <functional>
#include <iterator>
#include <map>
#include <optional>
#include <string>

#include "distributor/distributor/distribute.h"
#include "distributor/store/node_state_store.h"
#include "proto_cc/node.pb.h"

namespace e8 {

std::optional<NodeState> HashDistributor::Distribute(std::string const &key,
                                                     std::optional<NodeFunction> const function,
                                                     NodeStateStoreInterface *store) {
    std::map<NodeName, NodeState> nodes = store->Nodes(function, NodeStatus::NDS_READY);
    if (nodes.empty()) {
        return std::nullopt;
    }
    size_t selected = std::hash<std::string>()(key) % nodes.size();

    auto it = nodes.begin();
    std::advance(it, selected);
    assert(it != nodes.end());

    return it->second;
}

} // namespace e8
