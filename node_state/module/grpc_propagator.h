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

#ifndef GRPC_PROPAGATOR_H
#define GRPC_PROPAGATOR_H

#include <optional>
#include <vector>

#include "distributor/mutation_propagator/propagator.h"
#include "distributor/store/entity.h"
#include "proto_cc/node.pb.h"

namespace e8 {

/**
 * @brief The GrpcPropagator class Propagate the delta through gRPC calls.
 */
class GrpcPropagator : public PropagatorInterface {
  public:
    GrpcPropagator() = default;
    ~GrpcPropagator() override = default;

    std::optional<RevisionEpoch> GetRevisionEpoch(NodeState const &target) override;

    bool PropagateDelta(NodeState const &target,
                        std::vector<NodeStateRevision> const &revisions) override;
};

} // namespace e8

#endif // GRPC_PROPAGATOR_H
