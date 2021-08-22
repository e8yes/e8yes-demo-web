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
#include <optional>

#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/command.pb.h"
#include "proto_cc/raft.pb.h"
#include "replication/raft/context.h"
#include "replication/raft/journal.h"

class TestCommitListener : public e8::RaftCommitListener {
  public:
    TestCommitListener();
    virtual ~TestCommitListener() override;

    void Apply(e8::CommandEntry const &entry) override;
};

TestCommitListener::TestCommitListener() {}

TestCommitListener::~TestCommitListener() {}

void TestCommitListener::Apply(e8::CommandEntry const & /*entry*/) {}

bool CreateContextTest() {
    e8::RaftConfig config;
    config.set_me("localhost:11110");

    config.mutable_peers()->Add("localhost:11110");
    config.mutable_peers()->Add("localhost:11111");
    config.mutable_peers()->Add("localhost:11112");
    config.mutable_peers()->Add("localhost:11113");
    config.mutable_peers()->Add("localhost:11114");

    config.set_quorum_size(3);

    config.set_log_path("./test_raft_log");
    config.set_unavailability(1.0f);

    auto listener = std::make_unique<TestCommitListener>();
    std::unique_ptr<e8::RaftContext> context = e8::CreateRaftContext(listener.get(), config);

    TEST_CONDITION(context->follower_schedule != nullptr);
    TEST_CONDITION(context->candidate_schedule != nullptr);
    TEST_CONDITION(context->leader_schedule != nullptr);
    TEST_CONDITION(context->peers != nullptr);
    TEST_CONDITION(context->persister != nullptr);
    TEST_CONDITION(context->journal != nullptr);
    TEST_CONDITION(context->journal_replicator != nullptr);
    TEST_CONDITION(context->role_at_term != nullptr);
    TEST_CONDITION(context->voting_record != nullptr);
    TEST_CONDITION(context->election_committee != nullptr);

    TEST_CONDITION(context->me == "localhost:11110");

    TEST_CONDITION(context->peers->PeerCount() == 5);
    TEST_CONDITION(context->peers->Stub("localhost:11110"));
    TEST_CONDITION(context->peers->Stub("localhost:11111"));
    TEST_CONDITION(context->peers->Stub("localhost:11112"));
    TEST_CONDITION(context->peers->Stub("localhost:11113"));
    TEST_CONDITION(context->peers->Stub("localhost:11114"));

    TEST_CONDITION(context->voting_record->LastVotedFor() == std::nullopt);

    context = nullptr;
    int result = std::remove("./test_raft_log");

    TEST_CONDITION(result == 0);

    return true;
}

int main() {
    e8::BeginTestSuite("raft_context");
    e8::RunTest("CreateContextTest", CreateContextTest);
    e8::EndTestSuite();
    return 0;
}
