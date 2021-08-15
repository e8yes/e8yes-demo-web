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

#include <cstdio>
#include <memory>
#include <string>
#include <utility>

#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/persister.h"

bool PersistAndRecoverTest() {
    std::string recovery_file = "./raft_persister_test_file";
    std::remove(recovery_file.c_str());

    auto persister = std::make_unique<e8::RaftPersister>(recovery_file);

    // Should contains only default values.
    auto [current_term, current_role] = persister->TermAndRole();
    TEST_CONDITION(current_term == e8::RaftTerm());
    TEST_CONDITION(current_role == e8::RAFT_FOLLOWER);
    TEST_CONDITION(persister->LogEntriesRef()->empty());

    // Writes something to the persister and saves the data to file.
    persister->SetTermAndRole(std::make_pair(e8::RaftTerm(10), e8::RAFT_LEADER));

    e8::LogEntry log_entry;
    log_entry.set_term(10);
    log_entry.mutable_entry()->set_run_event_id("run_event1");
    log_entry.mutable_entry()->set_command("SET a=10");
    *persister->LogEntriesRef()->Add() = log_entry;

    persister->Persist();

    // Destroys the object and recovers data from file.
    persister = std::make_unique<e8::RaftPersister>(recovery_file);

    auto [recovered_term, recovered_role] = persister->TermAndRole();
    TEST_CONDITION(recovered_term == 10);
    TEST_CONDITION(recovered_role == e8::RAFT_LEADER);
    TEST_CONDITION(persister->LogEntriesRef()->size() == 1);
    TEST_CONDITION((*persister->LogEntriesRef())[0].term() == 10);
    TEST_CONDITION((*persister->LogEntriesRef())[0].entry().run_event_id() == "run_event1");
    TEST_CONDITION((*persister->LogEntriesRef())[0].entry().command() == "SET a=10");

    // Cleans up.
    persister = nullptr;

    int result = std::remove(recovery_file.c_str());
    TEST_CONDITION(result == 0);

    return true;
}

int main() {
    e8::BeginTestSuite("raft_persister");
    e8::RunTest("PersistAndRecoverTest", PersistAndRecoverTest);
    e8::EndTestSuite();
    return 0;
}
