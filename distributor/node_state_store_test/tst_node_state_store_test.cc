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

#include "distributor/store/node_state_store.h"
#include "proto_cc/node.pb.h"

class node_state_store_test : public QObject {
    Q_OBJECT

  public:
    node_state_store_test();
    ~node_state_store_test();

  private slots:
    void update_and_load_test();
    void update_override_test();
};

inline unsigned char constexpr operator"" _uchar(unsigned long long arg) noexcept {
    return static_cast<unsigned char>(arg);
}

inline char constexpr operator"" _char(unsigned long long arg) noexcept {
    return static_cast<char>(arg);
}

node_state_store_test::node_state_store_test() {}

node_state_store_test::~node_state_store_test() {}

std::map<std::string, e8::NodeState> PrepareNodeStates() {
    e8::NodeState node1;
    node1.set_name("node1");
    (*node1.mutable_ip_address()) += 192_uchar;
    (*node1.mutable_ip_address()) += 168_uchar;
    (*node1.mutable_ip_address()) += 1_uchar;
    (*node1.mutable_ip_address()) += 1_uchar;
    node1.mutable_functions()->Add(e8::NDF_FILE_STORE);

    e8::NodeState node2;
    node2.set_name("node2");
    (*node2.mutable_ip_address()) += 192_uchar;
    (*node2.mutable_ip_address()) += 168_uchar;
    (*node2.mutable_ip_address()) += 1_uchar;
    (*node2.mutable_ip_address()) += 2_uchar;
    node2.mutable_functions()->Add(e8::NDF_TASK_EXECUTOR);
    node2.mutable_functions()->Add(e8::NDF_MESSAGE_STORE);

    return std::map<std::string, e8::NodeState>{std::make_pair(node2.name(), node2),
                                                std::make_pair(node1.name(), node1)};
}

void node_state_store_test::update_and_load_test() {
    //    e8::NodeStateReaderStore reader_store(/*file_path=*/"test.sqlite");
    //    e8::NodeStateWriterStore writer_store(/*file_path=*/"test.sqlite",
    //    /*override_data=*/true);

    //    std::map<std::string, e8::NodeState> node_states = PrepareNodeStates();

    //    writer_store.Update(node_states);
    //    std::map<std::string, e8::NodeState> loaded_node_states = reader_store.Load();

    //    QVERIFY(loaded_node_states.size() == node_states.size());

    //    QVERIFY(loaded_node_states.find("node1") != loaded_node_states.end());
    //    QVERIFY(loaded_node_states["node1"].name() == "node1");
    //    QVERIFY(loaded_node_states["node1"].clock() == 0);
    //    QVERIFY(loaded_node_states["node1"].ip_address()[0] == 192_char);
    //    QVERIFY(loaded_node_states["node1"].ip_address()[1] == 168_char);
    //    QVERIFY(loaded_node_states["node1"].ip_address()[2] == 1_char);
    //    QVERIFY(loaded_node_states["node1"].ip_address()[3] == 1_char);
    //    QVERIFY(loaded_node_states["node1"].types_size() == 1);
    //    QVERIFY(loaded_node_states["node1"].types()[0] == e8::NDT_FILE_STORE);

    //    QVERIFY(loaded_node_states.find("node2") != loaded_node_states.end());
    //    QVERIFY(loaded_node_states["node2"].name() == "node2");
    //    QVERIFY(loaded_node_states["node2"].clock() == 10);
    //    QVERIFY(loaded_node_states["node2"].ip_address()[0] == 192_char);
    //    QVERIFY(loaded_node_states["node2"].ip_address()[1] == 168_char);
    //    QVERIFY(loaded_node_states["node2"].ip_address()[2] == 1_char);
    //    QVERIFY(loaded_node_states["node2"].ip_address()[3] == 2_char);
    //    QVERIFY(loaded_node_states["node2"].types_size() == 2);
    //    QVERIFY(loaded_node_states["node2"].types()[0] == e8::NDT_TASK_EXECUTOR);
    //    QVERIFY(loaded_node_states["node2"].types()[1] == e8::NDT_MESSAGE_STORE);

    //    std::remove("test.sqlite");
}

void node_state_store_test::update_override_test() {
    //    e8::NodeStateReaderStore reader_store(/*file_path=*/"test.sqlite");
    //    e8::NodeStateWriterStore writer_store(/*file_path=*/"test.sqlite",
    //    /*override_data=*/true);

    //    std::map<std::string, e8::NodeState> node_states = PrepareNodeStates();

    //    writer_store.Update(node_states);
    //    node_states["node1"].set_clock(2);
    //    writer_store.Update(node_states);

    //    std::map<std::string, e8::NodeState> loaded_node_states = reader_store.Load();
    //    QVERIFY(loaded_node_states.size() == node_states.size());

    //    QVERIFY(loaded_node_states.find("node1") != loaded_node_states.end());
    //    QVERIFY(loaded_node_states.find("node2") != loaded_node_states.end());
    //    QVERIFY(loaded_node_states["node1"].clock() == 2);

    //    std::remove("test.sqlite");
}

QTEST_APPLESS_MAIN(node_state_store_test)

#include "tst_node_state_store_test.moc"
