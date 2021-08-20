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

#include <chrono>
#include <optional>
#include <thread>

#include "common/unit_test_util/unit_test_util.h"
#include "replication/raft/common_types.h"
#include "replication/raft/integration_test/local_cluster.h"
#include "replication/raft/integration_test/role_monitor.h"

bool InitialElectionTest() {
    e8::LocalRaftCluster cluster;
    cluster.AddNode(/*commit_listener=*/nullptr)
        .AddNode(/*commit_listener=*/nullptr)
        .AddNode(/*commit_listener=*/nullptr)
        .AddNode(/*commit_listener=*/nullptr)
        .AddNode(/*commit_listener=*/nullptr)
        .Start(/*quorum_size=*/3);

    e8::RaftRoleMonitor role_monitor(&cluster);

    // Normal network. Should find one leader soon.
    std::optional<e8::RaftMachineAddress> leader1 = role_monitor.FindOneRaftLeaeder();
    TEST_CONDITION(leader1.has_value());

    // Keeps the cluster running for a while to see if the leader is stable.
    std::this_thread::sleep_for(std::chrono::milliseconds(2 * cluster.ElectionTimeout()));

    std::optional<e8::RaftMachineAddress> leader2 = role_monitor.FindOneRaftLeaeder();
    TEST_CONDITION(leader2.has_value());
    TEST_CONDITION(*leader1 == *leader2);

    return true;
}

bool ReElectionTest() {
    e8::LocalRaftCluster cluster;
    cluster.AddNode(/*commit_listener=*/nullptr)
        .AddNode(/*commit_listener=*/nullptr)
        .AddNode(/*commit_listener=*/nullptr)
        .AddNode(/*commit_listener=*/nullptr)
        .AddNode(/*commit_listener=*/nullptr)
        .Start(/*quorum_size=*/3);

    e8::RaftRoleMonitor role_monitor(&cluster);

    std::optional<e8::RaftMachineAddress> leader1 = role_monitor.FindOneRaftLeaeder();
    TEST_CONDITION(leader1.has_value());

    // Kills the leader. The cluster should quickly re-elect a new leader.
    cluster.Shutdown(*leader1);
    std::this_thread::sleep_for(std::chrono::milliseconds(2 * cluster.ElectionTimeout()));

    std::optional<e8::RaftMachineAddress> leader2 = role_monitor.FindOneRaftLeaeder();
    TEST_CONDITION(leader2.has_value());
    TEST_CONDITION(*leader2 != *leader1);

    // Bring the obsolete leader back. It should not disturb the current leader.
    cluster.Recover(*leader1);
    std::this_thread::sleep_for(std::chrono::milliseconds(2 * cluster.ElectionTimeout()));

    std::optional<e8::RaftMachineAddress> leader3 = role_monitor.FindOneRaftLeaeder();
    TEST_CONDITION(leader3.has_value());
    TEST_CONDITION(*leader3 == *leader2);

    return true;
}

bool MissingQuorumTest() {
    e8::LocalRaftCluster cluster;
    cluster.AddNode(/*commit_listener=*/nullptr)
        .AddNode(/*commit_listener=*/nullptr)
        .AddNode(/*commit_listener=*/nullptr)
        .AddNode(/*commit_listener=*/nullptr)
        .AddNode(/*commit_listener=*/nullptr)
        .Start(/*quorum_size=*/4);

    e8::RaftRoleMonitor role_monitor(&cluster);

    // Kills 2 leader nodes, then no leader should be ever elected because 3 nodes can't form a
    // quorum of 4.
    std::optional<e8::RaftMachineAddress> leader1 = role_monitor.FindOneRaftLeaeder();
    TEST_CONDITION(leader1.has_value());
    cluster.Shutdown(*leader1);
    std::this_thread::sleep_for(std::chrono::milliseconds(2 * cluster.ElectionTimeout()));

    std::optional<e8::RaftMachineAddress> leader2 = role_monitor.FindOneRaftLeaeder();
    TEST_CONDITION(leader2.has_value());
    cluster.Shutdown(*leader2);
    std::this_thread::sleep_for(std::chrono::milliseconds(2 * cluster.ElectionTimeout()));

    std::optional<e8::RaftMachineAddress> leader3 = role_monitor.FindOneRaftLeaeder();
    TEST_CONDITION(!leader3.has_value());

    // Brings all the nodes back, then we will find a unique leader.
    cluster.Recover(*leader1);
    cluster.Recover(*leader2);
    std::this_thread::sleep_for(std::chrono::milliseconds(2 * cluster.ElectionTimeout()));

    std::optional<e8::RaftMachineAddress> leader4 = role_monitor.FindOneRaftLeaeder();
    TEST_CONDITION(leader4.has_value());

    return true;
}

int main() {
    e8::BeginTestSuite("raft_election");
    e8::RunTest("InitialElectionTest", InitialElectionTest);
    e8::RunTest("ReElectionTest", ReElectionTest);
    e8::RunTest("MissingQuorumTest", MissingQuorumTest);
    e8::EndTestSuite();
    return 0;
}
