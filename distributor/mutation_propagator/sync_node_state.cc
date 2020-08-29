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
#include <map>
#include <optional>
#include <vector>

#include "distributor/mutation_propagator/propagator.h"
#include "distributor/mutation_propagator/sync_node_state.h"
#include "distributor/store/entity.h"
#include "distributor/store/node_state_store.h"
#include "proto_cc/node.pb.h"

namespace e8 {

bool SyncNodeStates(PeerStoreInterface *peers, NodeStateStoreInterface *node_states,
                    PropagatorInterface *propagator) {
    RevisionEpoch current_epoch = node_states->CurrentRevisionEpoch();

    // Propogate updates to the peers.
    std::map<NodeName, NodeState> peer_nodes = peers->Peers();
    for (auto const &[_, peer_node] : peer_nodes) {
        std::optional<RevisionEpoch> dst_epoch = propagator->GetRevisionEpoch(peer_node);
        assert(dst_epoch.has_value());

        std::vector<NodeStateRevision> delta = node_states->Revisions(*dst_epoch, current_epoch);
        if (delta.empty()) {
            continue;
        }

        bool rc = propagator->PropagateDelta(peer_node, delta);
        assert(rc);
    }

    return true;
}

} // namespace e8
