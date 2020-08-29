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

#include <QtTest>
#include <cassert>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "distributor/mutation_propagator/propagator.h"
#include "distributor/mutation_propagator/sync_node_state.h"
#include "distributor/store/entity.h"
#include "distributor/store/node_state_store.h"
#include "distributor/store/peer_store.h"
#include "proto_cc/node.pb.h"

class MockPeerStore : public e8::PeerStoreInterface {
  public:
    bool AddPeer(e8::NodeState const & /*node*/) {
        assert(false);
        return true;
    }

    bool DeletePeer(std::string const & /*node_name*/) {
        assert(false);
        return true;
    }

    std::map<e8::NodeName, e8::NodeState> Peers() {
        e8::NodeState node1;
        node1.set_name("node1");
        return std::map<e8::NodeName, e8::NodeState>{std::make_pair("node1", node1)};
    }
};

class MockNodeStateStore : public e8::NodeStateStoreInterface {
  public:
    bool UpdateNodeStates(e8::NodeStateRevision const & /*revision*/) {
        assert(false);
        return true;
    }

    std::map<e8::NodeName, e8::NodeState>
    Nodes(std::optional<e8::NodeFunction> const /*node_function*/,
          std::optional<e8::NodeStatus> const /*node_status*/) {
        assert(false);
        return std::map<e8::NodeName, e8::NodeState>();
    }

    e8::RevisionEpoch CurrentRevisionEpoch() { return 1; }

    std::vector<e8::NodeStateRevision> Revisions(e8::RevisionEpoch const begin,
                                                 e8::RevisionEpoch const end) {
        assert(begin == 0);
        assert(end == 1);

        e8::NodeState node1;
        node1.set_name("node1");

        e8::NodeStateRevision revision;
        revision.set_revision_epoch(1);
        (*revision.mutable_nodes())["node1"] = node1;
        (*revision.mutable_delta_operations())["node1"] = e8::DOP_ADD;
        (*revision.mutable_delta_operations())["node2"] = e8::DOP_DELETE;

        return std::vector<e8::NodeStateRevision>{revision};
    }
};

class MockPropagator : public e8::PropagatorInterface {
  public:
    std::optional<e8::RevisionEpoch> GetRevisionEpoch(e8::NodeState const &target) {
        assert(target.name() == "node1");
        return 0;
    }

    bool PropagateDelta(e8::NodeState const &target,
                        std::vector<e8::NodeStateRevision> const &delta) {
        assert(target.name() == "node1");
        assert(delta.size() == 1);
        assert(delta[0].revision_epoch() == 1);
        assert(delta[0].nodes().find("node1") != delta[0].nodes().end());
        assert(delta[0].delta_operations().find("node1") != delta[0].delta_operations().end());
        assert(delta[0].delta_operations().find("node2") != delta[0].delta_operations().end());
        return true;
    }
};

class sync_node_state_test : public QObject {
    Q_OBJECT

  public:
    sync_node_state_test();
    ~sync_node_state_test();

  private slots:
    void propagate_node_state_sync_test();
};

sync_node_state_test::sync_node_state_test() {}

sync_node_state_test::~sync_node_state_test() {}

void sync_node_state_test::propagate_node_state_sync_test() {
    MockPeerStore peers;
    MockNodeStateStore node_states;
    MockPropagator propagator;

    bool rc = e8::SyncNodeStates(&peers, &node_states, &propagator);
    QVERIFY(rc == true);
}

QTEST_APPLESS_MAIN(sync_node_state_test)

#include "tst_sync_node_state_test.moc"
