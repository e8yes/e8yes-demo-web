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

#include <iostream>
#include <limits>
#include <optional>
#include <string>

#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/command.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/integration_test/local_cluster.h"
#include "replication/raft/integration_test/replication_monitor.h"
#include "replication/raft/integration_test/role_monitor.h"

bool BasicAgreeTest() {
    e8::LocalRaftCluster cluster;
    cluster
        .AddNode(std::make_shared<e8::RaftReplicationMonitor::Listener>(
            std::numeric_limits<e8::RaftLogOffset>::max()))
        .AddNode(std::make_shared<e8::RaftReplicationMonitor::Listener>(
            std::numeric_limits<e8::RaftLogOffset>::max()))
        .AddNode(std::make_shared<e8::RaftReplicationMonitor::Listener>(
            std::numeric_limits<e8::RaftLogOffset>::max()))
        .AddNode(std::make_shared<e8::RaftReplicationMonitor::Listener>(
            std::numeric_limits<e8::RaftLogOffset>::max()))
        .AddNode(std::make_shared<e8::RaftReplicationMonitor::Listener>(
            std::numeric_limits<e8::RaftLogOffset>::max()))
        .Start(/*quorum_size=*/3, /*unavailability=*/1);

    e8::RaftRoleMonitor role_monitor(&cluster);
    e8::RaftReplicationMonitor replication_monitor(&cluster);

    std::optional<e8::RaftMachineAddress> leader = role_monitor.FindOneRaftLeaeder();
    TEST_CONDITION(leader.has_value());

    for (unsigned i = 0; i < 100; ++i) {
        e8::CommandEntry entry;
        entry.set_run_event_id(std::to_string(i) + "_run_event");
        entry.set_command(std::to_string(i) + "_command");

        bool agreed = replication_monitor.ReplicateCommand(*leader, entry, /*num_agrees=*/5);
        TEST_CONDITION(agreed == true);
    }

    return true;
}

bool FailAgreeTest() {
    e8::LocalRaftCluster cluster;
    cluster
        .AddNode(std::make_shared<e8::RaftReplicationMonitor::Listener>(
            std::numeric_limits<e8::RaftLogOffset>::max()))
        .AddNode(std::make_shared<e8::RaftReplicationMonitor::Listener>(
            std::numeric_limits<e8::RaftLogOffset>::max()))
        .AddNode(std::make_shared<e8::RaftReplicationMonitor::Listener>(
            std::numeric_limits<e8::RaftLogOffset>::max()))
        .AddNode(std::make_shared<e8::RaftReplicationMonitor::Listener>(
            std::numeric_limits<e8::RaftLogOffset>::max()))
        .AddNode(std::make_shared<e8::RaftReplicationMonitor::Listener>(
            std::numeric_limits<e8::RaftLogOffset>::max()))
        .Start(/*quorum_size=*/3, /*unavailability=*/1);

    e8::RaftRoleMonitor role_monitor(&cluster);
    e8::RaftReplicationMonitor replication_monitor(&cluster);

    std::optional<e8::RaftMachineAddress> leader1 = role_monitor.FindOneRaftLeaeder();
    TEST_CONDITION(leader1.has_value());

    // Agree on one command.
    e8::CommandEntry entry1;
    entry1.set_run_event_id("1_run_event");
    entry1.set_command("1_command");

    bool agreed = replication_monitor.ReplicateCommand(*leader1, entry1, /*num_agrees=*/5);
    TEST_CONDITION(agreed == true);

    // Shuts down one server and continue sending commands.
    cluster.Shutdown(*leader1);
    std::this_thread::sleep_for(std::chrono::milliseconds(cluster.ElectionTimeout()));

    std::optional<e8::RaftMachineAddress> leader2 = role_monitor.FindOneRaftLeaeder();
    TEST_CONDITION(leader2.has_value());

    e8::CommandEntry entry2;
    entry2.set_run_event_id("2_run_event");
    entry2.set_command("2_command");

    agreed = replication_monitor.ReplicateCommand(*leader2, entry2, /*num_agrees=*/4);
    TEST_CONDITION(agreed == true);

    e8::CommandEntry entry3;
    entry3.set_run_event_id("3_run_event");
    entry3.set_command("3_command");

    agreed = replication_monitor.ReplicateCommand(*leader2, entry3, /*num_agrees=*/4);
    TEST_CONDITION(agreed == true);

    // Brings back the initial leader and agree on new commands
    cluster.Recover(*leader1);

    e8::CommandEntry entry4;
    entry4.set_run_event_id("4_run_event");
    entry4.set_command("4_command");

    agreed = replication_monitor.ReplicateCommand(*leader2, entry4, /*num_agrees=*/5);
    TEST_CONDITION(agreed == true);

    return true;
}

int main() {
    e8::BeginTestSuite("raft_agreement");
    e8::RunTest("BasicAgreeTest", BasicAgreeTest);
    e8::RunTest("FailAgreeTest", FailAgreeTest);
    e8::EndTestSuite();
    return 0;
}
