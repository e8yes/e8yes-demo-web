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
#include <cstdio>

#include "distributor/store/peer_store.h"
#include "proto_cc/node.pb.h"

class peer_store_test : public QObject {
    Q_OBJECT

  public:
    peer_store_test();
    ~peer_store_test();

  private slots:
    void add_delete_query_peer_test();
};

peer_store_test::peer_store_test() {}

peer_store_test::~peer_store_test() {}

void peer_store_test::add_delete_query_peer_test() {
    std::remove("test.sqlite");

    e8::NodeState node1;
    node1.set_name("node1");

    e8::NodeState node2;
    node2.set_name("node2");

    e8::PeerStore store("test.sqlite");
    std::map<e8::NodeName, e8::NodeState> peers = store.Peers();
    QVERIFY(peers.empty());

    // Add node1.
    bool successful = store.AddPeer(node1);
    QVERIFY(successful == true);

    successful = store.AddPeer(node1);
    QVERIFY(successful == false);

    peers = store.Peers();
    QVERIFY(peers.size() == 1);
    QVERIFY(peers.find("node1") != peers.end());

    // Add node2.
    successful = store.AddPeer(node2);
    QVERIFY(successful == true);

    peers = store.Peers();
    QVERIFY(peers.size() == 2);
    QVERIFY(peers.find("node1") != peers.end());
    QVERIFY(peers.find("node2") != peers.end());

    // Delete node1.
    successful = store.DeletePeer("node1");
    QVERIFY(successful == true);

    successful = store.DeletePeer("node1");
    QVERIFY(successful == false);

    peers = store.Peers();
    QVERIFY(peers.size() == 1);
    QVERIFY(peers.find("node2") != peers.end());

    std::remove("test.sqlite");
}

QTEST_APPLESS_MAIN(peer_store_test)

#include "tst_peer_store_test.moc"
