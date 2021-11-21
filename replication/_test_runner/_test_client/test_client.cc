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

#include <limits>
#include <string>

#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/replication.pb.h"
#include "replication/runner/client.h"
#include "replication/runner/integration_test/key_value_store.h"
#include "replication/runner/integration_test/local_cluster.h"

bool BasicTest() {
    e8::LocalReplicationCluster cluster(
        /*num_nodes=*/5,
        /*preferred_snapshot_frequency=*/std::numeric_limits<e8::RaftLogOffset>::max());

    e8::ReplicationClient client(cluster.ClientConfig());

    e8::ReplicationKvStoreCommand command1;
    command1.mutable_put()->set_key("a");
    command1.mutable_put()->set_value("1");

    e8::ReplicationKvStoreCommand command2;
    command2.mutable_put_append()->set_key("a");
    command2.mutable_put_append()->set_value("2");

    e8::ReplicationKvStoreCommand command3;
    command3.mutable_get()->set_key("a");

    e8::ReplicationKvStoreCommand command4;
    command4.mutable_get()->set_key("b");

    client.RunCommand(command1.SerializeAsString());
    client.RunCommand(command2.SerializeAsString());
    std::string command3_result = client.RunCommand(command3.SerializeAsString());
    std::string command4_result = client.RunCommand(command4.SerializeAsString());

    e8::ReplicationKvStoreReturnValue command3_return_value;
    e8::ReplicationKvStoreReturnValue command4_return_value;
    command3_return_value.ParseFromString(command3_result);
    command4_return_value.ParseFromString(command4_result);

    TEST_CONDITION(command3_return_value.has_key());
    TEST_CONDITION(command3_return_value.value() == "12");

    TEST_CONDITION(!command4_return_value.has_key());

    return true;
}

int main() {
    e8::BeginTestSuite("replication_client");
    e8::RunTest("BasicTest", BasicTest);
    e8::EndTestSuite();
    return 0;
}
