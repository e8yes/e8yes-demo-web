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
#include <utility>
#include <vector>

#include "distributor/distributor/distribute.h"
#include "distributor/store/entity.h"
#include "distributor/store/node_state_store.h"
#include "proto_cc/node.pb.h"

class MockNodeStateStore : public e8::NodeStateStoreInterface {
  public:
    MockNodeStateStore(e8::NodeFunction const expected_node_function)
        : expected_node_function_(expected_node_function) {}
    ~MockNodeStateStore() override = default;

    bool UpdateNodeStates(e8::NodeStateRevision const & /*revision*/) override {
        assert(false);
        return false;
    }

    std::map<e8::NodeName, e8::NodeState>
    Nodes(std::optional<e8::NodeFunction> const node_function,
          std::optional<e8::NodeStatus> const node_status) override {
        assert(node_function.has_value());
        assert(node_status.has_value());

        assert(*node_function == expected_node_function_);
        assert(*node_status == e8::NDS_READY);

        e8::NodeState node1;
        node1.set_name("node1");
        node1.set_status(e8::NDS_READY);
        node1.mutable_functions()->Add(*node_function);

        e8::NodeState node2;
        node2.set_name("node2");
        node2.set_status(e8::NDS_READY);
        node1.mutable_functions()->Add(*node_function);

        return std::map<e8::NodeName, e8::NodeState>{std::make_pair("node1", node1),
                                                     std::make_pair("node2", node2)};
    }

    e8::RevisionEpoch CurrentRevisionEpoch() override {
        assert(false);
        return 0;
    }

    std::vector<e8::NodeStateRevision> Revisions(e8::RevisionEpoch const /*begin*/,
                                                 e8::RevisionEpoch const /*end*/) override {
        assert(false);
        return std::vector<e8::NodeStateRevision>();
    }

  private:
    e8::NodeFunction expected_node_function_;
};

class distributor_test : public QObject {
    Q_OBJECT

  public:
    distributor_test();
    ~distributor_test();

  private slots:
    void hash_distributor_test();
};

distributor_test::distributor_test() {}

distributor_test::~distributor_test() {}

void distributor_test::hash_distributor_test() {
    e8::HashDistributor distributor;
    MockNodeStateStore store(/*expected_node_function=*/e8::NDF_FILE_STORE);

    std::optional<e8::NodeState> node_a = distributor.Distribute("1", e8::NDF_FILE_STORE, &store);
    QVERIFY(node_a.has_value());
    QVERIFY(node_a->name() == "node2");

    // Distribute with the same key again.
    node_a = distributor.Distribute("1", e8::NDF_FILE_STORE, &store);
    QVERIFY(node_a.has_value());
    QVERIFY(node_a->name() == "node2");

    // Another key.
    std::optional<e8::NodeState> node_b = distributor.Distribute("2", e8::NDF_FILE_STORE, &store);
    QVERIFY(node_b.has_value());
    QVERIFY(node_b->name() == "node1");
}

QTEST_APPLESS_MAIN(distributor_test)

#include "tst_distributor_test.moc"
