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

#ifndef DISTRIBUTE_H
#define DISTRIBUTE_H

#include <optional>
#include <string>

#include "distributor/store/node_state_store.h"
#include "proto_cc/node.pb.h"

namespace e8 {

/**
 * @brief The DistributorInterface class Provides means to distribute a keyed object to an active
 * node in the cluster.
 */
class DistributorInterface {
  public:
    DistributorInterface() = default;
    virtual ~DistributorInterface() = default;

    /**
     * @brief Distribute Distribute a keyed object to an active node in the cluster. It's
     * guaranteed that the object can be later retrieved using this function as long as the key is
     * the same.
     *
     * @param function Distribute only to the subset of the nodes that has the specified node
     * function.
     * @param store A node state store that has the information of the entire cluster.
     * @return The node the object should distribute as long as there is one active node that
     * satisfies the constraint. Otherwise, it returns empty.
     */
    virtual std::optional<NodeState> Distribute(std::string const &key,
                                                std::optional<NodeFunction> const function,
                                                NodeStateStoreInterface *store) = 0;
};

/**
 * @brief The HashDistributor class Distribute the object by hashing the key into different buckets
 * (nodes).
 */
class HashDistributor : public DistributorInterface {
  public:
    HashDistributor() = default;
    ~HashDistributor() override = default;

    std::optional<NodeState> Distribute(std::string const &key,
                                        std::optional<NodeFunction> const function,
                                        NodeStateStoreInterface *store) override;
};

} // namespace e8

#endif // DISTRIBUTE_H
