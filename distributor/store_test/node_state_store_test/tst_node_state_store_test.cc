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

#include <QtTest>
#include <cstdio>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "distributor/store/node_state_store.h"
#include "proto_cc/delta.pb.h"
#include "proto_cc/node.pb.h"

class node_state_store_test : public QObject {
    Q_OBJECT

  public:
    node_state_store_test();
    ~node_state_store_test();

  private slots:
    void add_swap_delete_test();
    void arbitrary_update_order_test();
    void query_filter_test();
    void revision_history_test();
};

inline unsigned char constexpr operator"" _uchar(unsigned long long arg) noexcept {
    return static_cast<unsigned char>(arg);
}

inline char constexpr operator"" _char(unsigned long long arg) noexcept {
    return static_cast<char>(arg);
}

node_state_store_test::node_state_store_test() {}

node_state_store_test::~node_state_store_test() {}

std::map<e8::NodeName, e8::NodeState> PrepareNodeStates() {
    e8::NodeState node1;
    node1.set_name("node1");
    (*node1.mutable_ip_address()) += 192_uchar;
    (*node1.mutable_ip_address()) += 168_uchar;
    (*node1.mutable_ip_address()) += 1_uchar;
    (*node1.mutable_ip_address()) += 1_uchar;
    node1.set_status(e8::NodeStatus::NDS_READY);
    node1.mutable_functions()->Add(e8::NDF_FILE_STORE);

    e8::NodeState node2;
    node2.set_name("node2");
    (*node2.mutable_ip_address()) += 192_uchar;
    (*node2.mutable_ip_address()) += 168_uchar;
    (*node2.mutable_ip_address()) += 1_uchar;
    (*node2.mutable_ip_address()) += 2_uchar;
    node2.set_status(e8::NodeStatus::NDS_READY);
    node2.mutable_functions()->Add(e8::NDF_TASK_EXECUTOR);
    node2.mutable_functions()->Add(e8::NDF_MESSAGE_QUEUE);

    return std::map<e8::NodeName, e8::NodeState>{std::make_pair(node2.name(), node2),
                                                 std::make_pair(node1.name(), node1)};
}

void node_state_store_test::add_swap_delete_test() {
    std::remove("test.sqlite");

    e8::NodeStateStore store(/*file_path=*/"test.sqlite");
    e8::RevisionEpoch epoch = store.CurrentRevisionEpoch();
    QVERIFY(epoch == 0);

    std::map<e8::NodeName, e8::NodeState> nodes = PrepareNodeStates();

    // Add nodes.
    e8::NodeStateRevision revision1;
    revision1.set_revision_epoch(1);
    *revision1.mutable_nodes() = {nodes.begin(), nodes.end()};
    (*revision1.mutable_delta_operations())["node1"] = e8::DOP_ADD;
    (*revision1.mutable_delta_operations())["node2"] = e8::DOP_ADD;

    bool updated = store.UpdateNodeStates(revision1);
    QVERIFY(updated == true);

    epoch = store.CurrentRevisionEpoch();
    QVERIFY(epoch == 1);

    std::map<e8::NodeName, e8::NodeState> retrieved =
        store.Nodes(/*node_function=*/std::nullopt, /*node_status=*/std::nullopt);
    QVERIFY(retrieved.size() == 2);
    QVERIFY(retrieved.find("node1") != retrieved.end());
    QVERIFY(retrieved["node1"].name() == "node1");
    QVERIFY(retrieved["node1"].ip_address()[0] == 192_char);
    QVERIFY(retrieved["node1"].ip_address()[1] == 168_char);
    QVERIFY(retrieved["node1"].ip_address()[2] == 1_char);
    QVERIFY(retrieved["node1"].ip_address()[3] == 1_char);
    QVERIFY(retrieved["node1"].status() == e8::NDS_READY);
    QVERIFY(retrieved["node1"].functions_size() == 1);
    QVERIFY(retrieved["node1"].functions()[0] == e8::NDF_FILE_STORE);

    QVERIFY(retrieved.find("node2") != retrieved.end());
    QVERIFY(retrieved["node2"].name() == "node2");
    QVERIFY(retrieved["node2"].ip_address()[0] == 192_char);
    QVERIFY(retrieved["node2"].ip_address()[1] == 168_char);
    QVERIFY(retrieved["node2"].ip_address()[2] == 1_char);
    QVERIFY(retrieved["node2"].ip_address()[3] == 2_char);
    QVERIFY(retrieved["node2"].status() == e8::NDS_READY);
    QVERIFY(retrieved["node2"].functions_size() == 2);
    QVERIFY(retrieved["node2"].functions()[0] == e8::NDF_TASK_EXECUTOR);
    QVERIFY(retrieved["node2"].functions()[1] == e8::NDF_MESSAGE_QUEUE);

    updated = store.UpdateNodeStates(revision1);
    QVERIFY(updated == false);

    // Swap node1.
    nodes["node1"].set_status(e8::NDS_UNAVALIABLE);

    e8::NodeStateRevision revision2;
    revision2.set_revision_epoch(2);
    (*revision2.mutable_nodes())["node1"] = nodes["node1"];
    (*revision2.mutable_delta_operations())["node1"] = e8::DOP_SWAP;

    updated = store.UpdateNodeStates(revision2);
    QVERIFY(updated == true);

    epoch = store.CurrentRevisionEpoch();
    QVERIFY(epoch == 2);

    retrieved = store.Nodes(/*node_function=*/std::nullopt, /*node_status=*/std::nullopt);
    QVERIFY(retrieved.size() == 2);
    QVERIFY(retrieved["node1"].status() == e8::NDS_UNAVALIABLE);
    QVERIFY(retrieved["node2"].status() == e8::NDS_READY);

    // Remove node2.
    e8::NodeStateRevision revision3;
    revision3.set_revision_epoch(3);
    (*revision3.mutable_delta_operations())["node2"] = e8::DOP_DELETE;

    updated = store.UpdateNodeStates(revision3);
    QVERIFY(updated == true);

    epoch = store.CurrentRevisionEpoch();
    QVERIFY(epoch == 3);

    retrieved = store.Nodes(/*node_function=*/std::nullopt, /*node_status=*/std::nullopt);
    QVERIFY(retrieved.size() == 1);
    QVERIFY(retrieved["node1"].status() == e8::NDS_UNAVALIABLE);

    std::remove("test.sqlite");
}

void node_state_store_test::arbitrary_update_order_test() {
    std::map<e8::NodeName, e8::NodeState> nodes = PrepareNodeStates();

    // Add nodes.
    e8::NodeStateRevision revision1;
    revision1.set_revision_epoch(1);
    *revision1.mutable_nodes() = {nodes.begin(), nodes.end()};
    (*revision1.mutable_delta_operations())["node1"] = e8::DOP_ADD;
    (*revision1.mutable_delta_operations())["node2"] = e8::DOP_ADD;

    // Swap node1.
    nodes["node1"].set_status(e8::NDS_UNAVALIABLE);

    e8::NodeStateRevision revision2;
    revision2.set_revision_epoch(2);
    (*revision2.mutable_nodes())["node1"] = nodes["node1"];
    (*revision2.mutable_delta_operations())["node1"] = e8::DOP_SWAP;

    // Remove node2.
    e8::NodeStateRevision revision3;
    revision3.set_revision_epoch(3);
    (*revision3.mutable_delta_operations())["node2"] = e8::DOP_DELETE;

    std::remove("test.sqlite");

    e8::NodeStateStore store(/*file_path=*/"test.sqlite");
    store.UpdateNodeStates(revision3);
    e8::RevisionEpoch epoch = store.CurrentRevisionEpoch();
    QVERIFY(epoch == 0);

    store.UpdateNodeStates(revision1);
    epoch = store.CurrentRevisionEpoch();
    QVERIFY(epoch == 1);

    store.UpdateNodeStates(revision2);
    epoch = store.CurrentRevisionEpoch();
    QVERIFY(epoch == 3);

    std::map<e8::NodeName, e8::NodeState> retrieved =
        store.Nodes(/*node_function=*/std::nullopt, /*node_status=*/std::nullopt);
    QVERIFY(retrieved.size() == 1);
    QVERIFY(retrieved["node1"].status() == e8::NDS_UNAVALIABLE);

    std::remove("test.sqlite");
}

void node_state_store_test::query_filter_test() {
    std::map<e8::NodeName, e8::NodeState> nodes = PrepareNodeStates();

    // Add nodes.
    e8::NodeStateRevision revision;
    revision.set_revision_epoch(1);
    *revision.mutable_nodes() = {nodes.begin(), nodes.end()};
    (*revision.mutable_delta_operations())["node1"] = e8::DOP_ADD;
    (*revision.mutable_delta_operations())["node2"] = e8::DOP_ADD;

    std::remove("test.sqlite");

    e8::NodeStateStore store(/*file_path=*/"test.sqlite");
    store.UpdateNodeStates(revision);

    std::map<e8::NodeName, e8::NodeState> message_nodes =
        store.Nodes(/*node_function=*/e8::NDF_MESSAGE_QUEUE, /*node_status=*/std::nullopt);
    QVERIFY(message_nodes.size() == 1);
    QVERIFY(message_nodes["node2"].status() == e8::NDS_READY);

    std::map<e8::NodeName, e8::NodeState> initializing_nodes =
        store.Nodes(/*node_function=*/std::nullopt, /*node_status=*/e8::NDS_INITIALIZING);
    QVERIFY(initializing_nodes.empty());

    std::remove("test.sqlite");
}

void node_state_store_test::revision_history_test() {
    std::map<e8::NodeName, e8::NodeState> nodes = PrepareNodeStates();

    // Add nodes.
    e8::NodeStateRevision revision1;
    revision1.set_revision_epoch(1);
    *revision1.mutable_nodes() = {nodes.begin(), nodes.end()};
    (*revision1.mutable_delta_operations())["node1"] = e8::DOP_ADD;
    (*revision1.mutable_delta_operations())["node2"] = e8::DOP_ADD;

    // Swap node1.
    nodes["node1"].set_status(e8::NDS_UNAVALIABLE);

    e8::NodeStateRevision revision2;
    revision2.set_revision_epoch(2);
    (*revision2.mutable_nodes())["node1"] = nodes["node1"];
    (*revision2.mutable_delta_operations())["node1"] = e8::DOP_SWAP;

    // Remove node2.
    e8::NodeStateRevision revision3;
    revision3.set_revision_epoch(3);
    (*revision3.mutable_delta_operations())["node2"] = e8::DOP_DELETE;

    std::remove("test.sqlite");

    e8::NodeStateStore store(/*file_path=*/"test.sqlite");

    store.UpdateNodeStates(revision3);
    store.UpdateNodeStates(revision1);
    store.UpdateNodeStates(revision2);

    std::vector<e8::NodeStateRevision> revisions1_2 = store.Revisions(/*begin=*/1, /*end=*/2);
    QVERIFY(revisions1_2.size() == 2);

    std::remove("test.sqlite");
}

QTEST_APPLESS_MAIN(node_state_store_test)

#include "tst_node_state_store_test.moc"
