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

#include "common/unit_test_util/unit_test_util.h"
#include "replication/raft/common_types.h"
#include "replication/raft/persister.h"
#include "replication/raft/role_at_term.h"

namespace {

constexpr char const *gRecoveryFile = "./raft_test_recovery_file";

struct TestData {
    TestData();
    ~TestData();

    std::unique_ptr<e8::RaftPersister> persister;
    std::unique_ptr<e8::RoleAtTerm> role_at_term;
};

TestData::TestData()
    : persister(std::make_unique<e8::RaftPersister>(gRecoveryFile)),
      role_at_term(std::make_unique<e8::RoleAtTerm>(persister.get())) {}

TestData::~TestData() { std::remove(gRecoveryFile); }

} // namespace

bool SetTermAndRoleTest() {
    auto test_data = std::make_unique<TestData>();

    auto [default_term, default_role] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(default_term == 0);
    TEST_CONDITION(default_role == e8::RAFT_FOLLOWER);

    test_data->role_at_term->SetTermAndRole(/*term=*/10, /*role=*/e8::RAFT_LEADER);

    auto [updated_term, udpated_role] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(updated_term == 10);
    TEST_CONDITION(udpated_role == e8::RAFT_LEADER);

    return true;
}

bool FollowerTransitionsTest() {
    auto test_data = std::make_unique<TestData>();

    auto [default_term, default_role] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(default_term == 0);
    TEST_CONDITION(default_role == e8::RAFT_FOLLOWER);

    // No upgrade conditions.
    bool updated = test_data->role_at_term->UpgradeTerm(
        /*node=*/"test_node", /*new_term=*/1, e8::RoleAtTerm::WON_AN_ELECTION);
    TEST_CONDITION(updated == false);

    auto [no_upgrade_term, no_upgrade_role] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(no_upgrade_term == 0);
    TEST_CONDITION(no_upgrade_role == e8::RAFT_FOLLOWER);

    updated = test_data->role_at_term->UpgradeTerm(
        /*node=*/"test_node", /*new_term=*/1, e8::RoleAtTerm::LOST_AN_ELECTION);
    TEST_CONDITION(updated == false);

    auto [no_upgrade_term2, no_upgrade_role2] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(no_upgrade_term2 == 0);
    TEST_CONDITION(no_upgrade_role2 == e8::RAFT_FOLLOWER);

    // Encountered higher termed message.
    updated = test_data->role_at_term->UpgradeTerm(
        /*node=*/"test_node", /*new_term=*/1, e8::RoleAtTerm::ENCOUNTERED_HIGHER_TERM_MESSAGE);
    TEST_CONDITION(updated == true);

    auto [updated_term, updated_role] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(updated_term == 1);
    TEST_CONDITION(updated_role == e8::RAFT_FOLLOWER);

    // Lack of heartbeat.
    updated = test_data->role_at_term->UpgradeTerm(
        /*node=*/"test_node", /*new_term=*/1, e8::RoleAtTerm::LACK_OF_HEARTBEAT);
    TEST_CONDITION(updated == true);

    auto [updated_term2, updated_role2] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(updated_term2 == 1);
    TEST_CONDITION(updated_role2 == e8::RAFT_CANDIDATE);

    return true;
}

bool CandidateTransitionsTest() {
    auto test_data = std::make_unique<TestData>();

    test_data->role_at_term->SetTermAndRole(/*term=*/0, e8::RAFT_CANDIDATE);

    // No upgrade conditions.
    bool updated = test_data->role_at_term->UpgradeTerm(
        /*node=*/"test_node", /*new_term=*/1, e8::RoleAtTerm::LACK_OF_HEARTBEAT);
    TEST_CONDITION(updated == false);

    auto [no_upgrade_term, no_upgrade_role] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(no_upgrade_term == 0);
    TEST_CONDITION(no_upgrade_role == e8::RAFT_CANDIDATE);

    // Loses an election.
    updated = test_data->role_at_term->UpgradeTerm(/*node=*/"test_node", /*new_term=*/1,
                                                   e8::RoleAtTerm::LOST_AN_ELECTION);

    TEST_CONDITION(updated == true);

    auto [updated_term, updated_role] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(updated_term == 1);
    TEST_CONDITION(updated_role == e8::RAFT_CANDIDATE);

    // Wins an election.
    updated = test_data->role_at_term->UpgradeTerm(/*node=*/"test_node", /*new_term=*/2,
                                                   e8::RoleAtTerm::WON_AN_ELECTION);

    TEST_CONDITION(updated == true);

    auto [updated_term2, updated_role2] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(updated_term2 == 2);
    TEST_CONDITION(updated_role2 == e8::RAFT_LEADER);

    // Encounters higher term message.
    test_data->role_at_term->SetTermAndRole(/*term=*/0, e8::RAFT_CANDIDATE);

    updated = test_data->role_at_term->UpgradeTerm(
        /*node=*/"test_node", /*new_term=*/1, e8::RoleAtTerm::ENCOUNTERED_HIGHER_TERM_MESSAGE);
    TEST_CONDITION(updated == true);

    auto [updated_term3, updated_role3] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(updated_term3 == 1);
    TEST_CONDITION(updated_role3 == e8::RAFT_FOLLOWER);

    return true;
}

bool LeaderTransitionsTest() {
    auto test_data = std::make_unique<TestData>();

    test_data->role_at_term->SetTermAndRole(/*term=*/0, e8::RAFT_LEADER);

    // No upgrade conditions.
    bool updated = test_data->role_at_term->UpgradeTerm(
        /*node=*/"test_node", /*new_term=*/1, e8::RoleAtTerm::LOST_AN_ELECTION);
    TEST_CONDITION(updated == false);

    auto [no_upgrade_term, no_upgrade_role] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(no_upgrade_term == 0);
    TEST_CONDITION(no_upgrade_role == e8::RAFT_LEADER);

    updated = test_data->role_at_term->UpgradeTerm(
        /*node=*/"test_node", /*new_term=*/1, e8::RoleAtTerm::WON_AN_ELECTION);
    TEST_CONDITION(updated == false);

    auto [no_upgrade_term2, no_upgrade_role2] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(no_upgrade_term2 == 0);
    TEST_CONDITION(no_upgrade_role2 == e8::RAFT_LEADER);

    updated = test_data->role_at_term->UpgradeTerm(
        /*node=*/"test_node", /*new_term=*/1, e8::RoleAtTerm::LOST_AN_ELECTION);
    TEST_CONDITION(updated == false);

    auto [no_upgrade_term3, no_upgrade_role3] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(no_upgrade_term3 == 0);
    TEST_CONDITION(no_upgrade_role3 == e8::RAFT_LEADER);

    // Encounters higher term message.
    updated = test_data->role_at_term->UpgradeTerm(
        /*node=*/"test_node", /*new_term=*/1, e8::RoleAtTerm::ENCOUNTERED_HIGHER_TERM_MESSAGE);
    TEST_CONDITION(updated == true);

    auto [updated_term3, updated_role3] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(updated_term3 == 1);
    TEST_CONDITION(updated_role3 == e8::RAFT_FOLLOWER);

    return true;
}

bool CrashRecoveryTest() {
    auto test_data = std::make_unique<TestData>();

    test_data->role_at_term->UpgradeTerm(
        /*node=*/"test_node", /*new_term=*/1, e8::RoleAtTerm::LACK_OF_HEARTBEAT);

    test_data->role_at_term = nullptr;
    test_data->persister = nullptr;

    test_data->persister = std::make_unique<e8::RaftPersister>(gRecoveryFile);
    test_data->role_at_term = std::make_unique<e8::RoleAtTerm>(test_data->persister.get());

    auto [recovered_term, recovered_role] = test_data->role_at_term->TermAndRole();
    TEST_CONDITION(recovered_term == 1);
    TEST_CONDITION(recovered_role == e8::RAFT_CANDIDATE);

    return true;
}

int main() {
    e8::BeginTestSuite("raft_role_at_term");
    e8::RunTest("SetTermAndRoleTest", SetTermAndRoleTest);
    e8::RunTest("FollowerTransitionsTest", FollowerTransitionsTest);
    e8::RunTest("CandidateTransitionsTest", CandidateTransitionsTest);
    e8::RunTest("LeaderTransitionsTest", LeaderTransitionsTest);
    e8::RunTest("CrashRecoveryTest", CrashRecoveryTest);
    e8::EndTestSuite();
    return 0;
}
