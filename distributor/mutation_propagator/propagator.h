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

#ifndef PROPAGATOR_H
#define PROPAGATOR_H

#include <optional>
#include <vector>

#include "distributor/store/entity.h"
#include "proto_cc/node.pb.h"

namespace e8 {

/**
 * @brief The PropagatorInterface class Serves as a mean to synchronize differences among nodes in
 * a cluster.
 */
class PropagatorInterface {
  public:
    PropagatorInterface() = default;
    virtual ~PropagatorInterface() = default;

    /**
     * @brief GetRevisionEpoch Get the current revision epoch at the target node.
     * @return If no error occurs, then it should return the revision epoch retrieved.
     */
    virtual std::optional<RevisionEpoch> GetRevisionEpoch(NodeState const &target) = 0;

    /**
     * @brief PropagateDelta Apply and propagate the delta to the target node.
     * @return True if no error occurred, otherwise false.
     */
    virtual bool PropagateDelta(NodeState const &target,
                                std::vector<NodeStateRevision> const &delta) = 0;
};

} // namespace e8

#endif // PROPAGATOR_H
