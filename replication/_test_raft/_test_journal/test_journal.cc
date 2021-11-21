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

#include <cassert>
#include <cstdio>
#include <google/protobuf/repeated_field.h>
#include <iostream>
#include <limits>
#include <memory>
#include <string>

#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/command.pb.h"
#include "proto_cc/raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/journal.h"

namespace {

constexpr char const *gRecoveryFile = "./raft_test_recovery_file";

struct TestData {
    TestData(e8::RaftCommitListener *commit_listener);
    ~TestData();

    std::unique_ptr<e8::RaftPersister> persister;
    std::unique_ptr<e8::RaftJournal> journal;
};

TestData::TestData(e8::RaftCommitListener *commit_listener)
    : persister(std::make_unique<e8::RaftPersister>(gRecoveryFile)),
      journal(std::make_unique<e8::RaftJournal>(persister.get(), commit_listener)) {}

TestData::~TestData() { std::remove(gRecoveryFile); }

class NoOpCommitListener : public e8::RaftCommitListener {
  public:
    NoOpCommitListener(e8::RaftLogOffset preferred_snapshot_frequency)
        : preferred_snapshot_frequency_(preferred_snapshot_frequency) {}
    ~NoOpCommitListener() override {}

    void Apply(e8::CommandEntry const & /*entry*/) override {}

    e8::RaftLogOffset PreferredSnapshotFrequency() const override {
        return preferred_snapshot_frequency_;
    }

    void Save() const override {}

    void Restore() override {}

  private:
    e8::RaftLogOffset const preferred_snapshot_frequency_;
};

class CommandCommitChecker : public e8::RaftCommitListener {
  public:
    CommandCommitChecker(std::vector<e8::CommandEntry> const &expected_commands);
    ~CommandCommitChecker();

    void Apply(e8::CommandEntry const &entry) override;
    e8::RaftLogOffset PreferredSnapshotFrequency() const override;
    void Save() const override;
    void Restore() override;

    unsigned ApplyCount() const;

  private:
    std::vector<e8::CommandEntry> const expected_commands_;
    unsigned index_;
};

CommandCommitChecker::CommandCommitChecker(std::vector<e8::CommandEntry> const &expected_commands)
    : expected_commands_(expected_commands), index_(0) {}

CommandCommitChecker::~CommandCommitChecker() {}

void CommandCommitChecker::Apply(e8::CommandEntry const &entry) {
    assert(index_ < expected_commands_.size());
    assert(entry.SerializeAsString() == expected_commands_[index_++].SerializeAsString());
}

e8::RaftLogOffset CommandCommitChecker::PreferredSnapshotFrequency() const { return 1; }

void CommandCommitChecker::Save() const {}

void CommandCommitChecker::Restore() {}

unsigned CommandCommitChecker::ApplyCount() const { return index_; }

} // namespace

bool AppendAndExportTest() {
    auto no_op = std::make_unique<NoOpCommitListener>(
        /*preferred_snapshot_frequency=*/std::numeric_limits<e8::RaftLogOffset>::max());
    auto test_data = std::make_unique<TestData>(no_op.get());

    e8::LogEntry log1;
    log1.set_term(1);
    log1.mutable_entry()->set_run_event_id("ID1");
    log1.mutable_entry()->set_command("COMMAND1");

    e8::LogEntry log2;
    log2.set_term(2);
    log2.mutable_entry()->set_run_event_id("ID2");
    log2.mutable_entry()->set_command("COMMAND2");

    e8::RaftLogOffset log_index = test_data->journal->AppendLog(log1);
    TEST_CONDITION(log_index == 0);

    log_index = test_data->journal->AppendLog(log2);
    TEST_CONDITION(log_index == 1);

    e8::RaftLogOffset exported_snapshot_progress;
    google::protobuf::RepeatedPtrField<e8::LogEntry> exported_logs;
    e8::RaftTerm preceding_term;
    bool exportable = test_data->journal->Export(/*start=*/0, &exported_snapshot_progress,
                                                 &exported_logs, &preceding_term);
    TEST_CONDITION(exportable == true);
    TEST_CONDITION(exported_snapshot_progress == 0);
    TEST_CONDITION(preceding_term == 0);
    TEST_CONDITION(exported_logs.size() == 2);
    TEST_CONDITION(exported_logs[0].SerializeAsString() == log1.SerializeAsString());
    TEST_CONDITION(exported_logs[1].SerializeAsString() == log2.SerializeAsString());

    exported_logs.Clear();
    exportable = test_data->journal->Export(/*start=*/1, &exported_snapshot_progress,
                                            &exported_logs, &preceding_term);
    TEST_CONDITION(exportable == true);
    TEST_CONDITION(exported_snapshot_progress == 0);
    TEST_CONDITION(preceding_term == 1);
    TEST_CONDITION(exported_logs.size() == 1);
    TEST_CONDITION(exported_logs[0].SerializeAsString() == log2.SerializeAsString());

    exported_logs.Clear();
    exportable = test_data->journal->Export(/*start=*/2, &exported_snapshot_progress,
                                            &exported_logs, &preceding_term);
    TEST_CONDITION(exportable == true);
    TEST_CONDITION(exported_snapshot_progress == 0);
    TEST_CONDITION(preceding_term == 2);
    TEST_CONDITION(exported_logs.empty());

    exported_logs.Clear();
    exportable = test_data->journal->Export(/*start=*/3, &exported_snapshot_progress,
                                            &exported_logs, &preceding_term);
    TEST_CONDITION(exportable == false);
    TEST_CONDITION(exported_snapshot_progress == 0);
    TEST_CONDITION(preceding_term == 0);
    TEST_CONDITION(exported_logs.empty());

    return true;
}

bool ImportAndExportTest() {
    auto no_op = std::make_unique<NoOpCommitListener>(
        /*preferred_snapshot_frequency=*/std::numeric_limits<e8::RaftLogOffset>::max());
    auto test_data = std::make_unique<TestData>(no_op.get());

    e8::LogEntry log1;
    log1.set_term(1);
    log1.mutable_entry()->set_run_event_id("ID1");
    log1.mutable_entry()->set_command("COMMAND1");

    e8::LogEntry log2;
    log2.set_term(2);
    log2.mutable_entry()->set_run_event_id("ID2");
    log2.mutable_entry()->set_command("COMMAND2");

    test_data->journal->AppendLog(log1);
    test_data->journal->AppendLog(log2);

    // Identical foreign logs.
    // Current journal state:
    // 1, ID1, COMMAND1
    // 2, ID2, COMMAND2
    google::protobuf::RepeatedPtrField<e8::LogEntry> foreign_logs1;
    *foreign_logs1.Add() = log1;
    *foreign_logs1.Add() = log2;

    bool resolvable = test_data->journal->Import(/*from=*/0, /*foreign_snapshot_progress=*/0,
                                                 foreign_logs1, /*preceding_log_term=*/0);
    TEST_CONDITION(resolvable == true);

    e8::RaftLogOffset exported_snapshot_progress;
    google::protobuf::RepeatedPtrField<e8::LogEntry> exported_logs;
    e8::RaftTerm preceding_term;
    bool exportable = test_data->journal->Export(/*start=*/0, &exported_snapshot_progress,
                                                 &exported_logs, &preceding_term);
    TEST_CONDITION(exportable == true);
    TEST_CONDITION(exported_snapshot_progress == 0);
    TEST_CONDITION(preceding_term == 0);
    TEST_CONDITION(exported_logs.size() == 2);
    TEST_CONDITION(exported_logs[0].SerializeAsString() == log1.SerializeAsString());
    TEST_CONDITION(exported_logs[1].SerializeAsString() == log2.SerializeAsString());

    // Outdated foreign logs.
    // Current journal state:
    // 1, ID1, COMMAND1
    // 2, ID2, COMMAND2
    google::protobuf::RepeatedPtrField<e8::LogEntry> foreign_logs2;
    *foreign_logs2.Add() = log1;

    resolvable = test_data->journal->Import(/*from=*/0, /*foreign_snapshot_progress=*/0,
                                            foreign_logs2, /*preceding_log_term=*/0);
    TEST_CONDITION(resolvable == true);

    exported_logs.Clear();
    exportable = test_data->journal->Export(/*start=*/0, &exported_snapshot_progress,
                                            &exported_logs, &preceding_term);
    TEST_CONDITION(exportable == true);
    TEST_CONDITION(exported_snapshot_progress == 0);
    TEST_CONDITION(preceding_term == 0);
    TEST_CONDITION(exported_logs.size() == 2);
    TEST_CONDITION(exported_logs[0].SerializeAsString() == log1.SerializeAsString());
    TEST_CONDITION(exported_logs[1].SerializeAsString() == log2.SerializeAsString());

    // Foreign logs with extras.
    // Current journal state:
    // 1, ID1, COMMAND1
    // 2, ID2, COMMAND2
    google::protobuf::RepeatedPtrField<e8::LogEntry> foreign_logs3;

    e8::LogEntry log3;
    log3.set_term(3);
    log3.mutable_entry()->set_run_event_id("ID3");
    log3.mutable_entry()->set_command("COMMAND3");

    *foreign_logs3.Add() = log1;
    *foreign_logs3.Add() = log2;
    *foreign_logs3.Add() = log3;

    resolvable = test_data->journal->Import(/*from=*/0, /*foreign_snapshot_progress=*/0,
                                            foreign_logs3, /*preceding_log_term=*/0);
    TEST_CONDITION(resolvable == true);

    exported_logs.Clear();
    exportable = test_data->journal->Export(/*start=*/0, &exported_snapshot_progress,
                                            &exported_logs, &preceding_term);
    TEST_CONDITION(exportable == true);
    TEST_CONDITION(exported_snapshot_progress == 0);
    TEST_CONDITION(preceding_term == 0);
    TEST_CONDITION(exported_logs.size() == 3);
    TEST_CONDITION(exported_logs[0].SerializeAsString() == log1.SerializeAsString());
    TEST_CONDITION(exported_logs[1].SerializeAsString() == log2.SerializeAsString());
    TEST_CONDITION(exported_logs[2].SerializeAsString() == log3.SerializeAsString());

    // Different foreign logs.
    // Current journal state:
    // 1, ID1, COMMAND1
    // 2, ID2, COMMAND2
    // 3, ID3, COMMAND3
    google::protobuf::RepeatedPtrField<e8::LogEntry> foreign_logs4;

    e8::LogEntry log2_var;
    log2_var.set_term(4);
    log2_var.mutable_entry()->set_run_event_id("ID2_VAR");
    log2_var.mutable_entry()->set_command("COMMAND2_VAR");

    *foreign_logs4.Add() = log2_var;

    resolvable = test_data->journal->Import(/*from=*/1, /*foreign_snapshot_progress=*/0,
                                            foreign_logs4, /*preceding_log_term=*/log1.term());
    TEST_CONDITION(resolvable == true);

    exported_logs.Clear();
    exportable = test_data->journal->Export(/*start=*/0, &exported_snapshot_progress,
                                            &exported_logs, &preceding_term);
    TEST_CONDITION(exportable == true);
    TEST_CONDITION(exported_snapshot_progress == 0);
    TEST_CONDITION(preceding_term == 0);
    TEST_CONDITION(exported_logs.size() == 2);
    TEST_CONDITION(exported_logs[0].SerializeAsString() == log1.SerializeAsString());
    TEST_CONDITION(exported_logs[1].SerializeAsString() == log2_var.SerializeAsString());

    // Unresolvable foreign logs with gap.
    // Current journal state:
    // 1, ID1, COMMAND1
    // 4, ID2_VAR, COMMAND2_VAR
    google::protobuf::RepeatedPtrField<e8::LogEntry> foreign_logs5;

    e8::LogEntry log4;
    log4.set_term(4);
    log4.mutable_entry()->set_run_event_id("ID4");
    log4.mutable_entry()->set_command("COMMAND4");

    *foreign_logs5.Add() = log4;

    resolvable = test_data->journal->Import(/*from=*/3, /*foreign_snapshot_progress=*/0,
                                            foreign_logs5, /*preceding_log_term=*/4);
    TEST_CONDITION(resolvable == false);

    exported_logs.Clear();
    exportable = test_data->journal->Export(/*start=*/0, &exported_snapshot_progress,
                                            &exported_logs, &preceding_term);
    TEST_CONDITION(exportable == true);
    TEST_CONDITION(exported_snapshot_progress == 0);
    TEST_CONDITION(preceding_term == 0);
    TEST_CONDITION(exported_logs.size() == 2);
    TEST_CONDITION(exported_logs[0].SerializeAsString() == log1.SerializeAsString());
    TEST_CONDITION(exported_logs[1].SerializeAsString() == log2_var.SerializeAsString());

    // Unresolvable foreign logs with mismatching preceding term.
    // Current journal state:
    // 1, ID1, COMMAND1
    // 4, ID2_VAR, COMMAND2_VAR
    google::protobuf::RepeatedPtrField<e8::LogEntry> foreign_logs6;

    *foreign_logs6.Add() = log3;

    resolvable = test_data->journal->Import(/*from=*/2, /*foreign_snapshot_progress=*/0,
                                            foreign_logs6, /*preceding_log_term=*/2);
    TEST_CONDITION(resolvable == false);

    exported_logs.Clear();
    exportable = test_data->journal->Export(/*start=*/0, &exported_snapshot_progress,
                                            &exported_logs, &preceding_term);
    TEST_CONDITION(exportable == true);
    TEST_CONDITION(exported_snapshot_progress == 0);
    TEST_CONDITION(preceding_term == 0);
    TEST_CONDITION(exported_logs.size() == 2);
    TEST_CONDITION(exported_logs[0].SerializeAsString() == log1.SerializeAsString());
    TEST_CONDITION(exported_logs[1].SerializeAsString() == log2_var.SerializeAsString());

    // Empty append.
    // Current journal state:
    // 1, ID1, COMMAND1
    // 4, ID2_VAR, COMMAND2_VAR
    google::protobuf::RepeatedPtrField<e8::LogEntry> foreign_logs7;

    resolvable = test_data->journal->Import(/*from=*/2, /*foreign_snapshot_progress=*/0,
                                            foreign_logs7, /*preceding_log_term=*/4);
    TEST_CONDITION(resolvable == true);

    exported_logs.Clear();
    exportable = test_data->journal->Export(/*start=*/0, &exported_snapshot_progress,
                                            &exported_logs, &preceding_term);
    TEST_CONDITION(exportable == true);
    TEST_CONDITION(exported_snapshot_progress == 0);
    TEST_CONDITION(preceding_term == 0);
    TEST_CONDITION(exported_logs.size() == 2);
    TEST_CONDITION(exported_logs[0].SerializeAsString() == log1.SerializeAsString());
    TEST_CONDITION(exported_logs[1].SerializeAsString() == log2_var.SerializeAsString());

    return true;
}

bool LivenessTest() {
    auto no_op = std::make_unique<NoOpCommitListener>(
        /*preferred_snapshot_frequency=*/std::numeric_limits<e8::RaftLogOffset>::max());
    auto test_data = std::make_unique<TestData>(no_op.get());

    e8::LogSourceLiveness liveness = test_data->journal->Liveness();

    TEST_CONDITION(liveness.log_progress() == 0);
    TEST_CONDITION(liveness.highest_log_term() == 0);

    e8::LogEntry log1;
    log1.set_term(10);
    log1.mutable_entry()->set_run_event_id("ID1");
    log1.mutable_entry()->set_command("COMMAND1");

    test_data->journal->AppendLog(log1);

    liveness = test_data->journal->Liveness();

    TEST_CONDITION(liveness.log_progress() == 1);
    TEST_CONDITION(liveness.highest_log_term() == 10);

    return true;
}

bool StalenessTest() {
    auto no_op = std::make_unique<NoOpCommitListener>(
        /*preferred_snapshot_frequency=*/std::numeric_limits<e8::RaftLogOffset>::max());
    auto test_data = std::make_unique<TestData>(no_op.get());

    // Current journal state:
    // progress=0   highest_term=0
    e8::LogSourceLiveness foreign_journal_liveness;
    foreign_journal_liveness.set_log_progress(0);
    foreign_journal_liveness.set_highest_log_term(0);

    TEST_CONDITION(false == test_data->journal->Stale(foreign_journal_liveness));

    // Current journal state:
    // progress=0   highest_term=0
    e8::LogEntry log1;
    log1.set_term(1);
    log1.mutable_entry()->set_run_event_id("ID1");
    log1.mutable_entry()->set_command("COMMAND1");

    e8::LogEntry log2;
    log2.set_term(2);
    log2.mutable_entry()->set_run_event_id("ID2");
    log2.mutable_entry()->set_command("COMMAND2");

    test_data->journal->AppendLog(log1);
    test_data->journal->AppendLog(log2);

    TEST_CONDITION(true == test_data->journal->Stale(foreign_journal_liveness));

    // Current journal state:
    // progress=2   highest_term=2
    foreign_journal_liveness.set_log_progress(2);
    foreign_journal_liveness.set_highest_log_term(2);

    TEST_CONDITION(false == test_data->journal->Stale(foreign_journal_liveness));

    // Current journal state:
    // progress=2   highest_term=2
    foreign_journal_liveness.set_log_progress(1);
    foreign_journal_liveness.set_highest_log_term(2);

    TEST_CONDITION(true == test_data->journal->Stale(foreign_journal_liveness));

    // Current journal state:
    // progress=2   highest_term=2
    foreign_journal_liveness.set_log_progress(5);
    foreign_journal_liveness.set_highest_log_term(1);

    TEST_CONDITION(true == test_data->journal->Stale(foreign_journal_liveness));

    return true;
}

bool EndWithTermTest() {
    auto no_op = std::make_unique<NoOpCommitListener>(
        /*preferred_snapshot_frequency=*/std::numeric_limits<e8::RaftLogOffset>::max());
    auto test_data = std::make_unique<TestData>(no_op.get());

    e8::LogEntry log1;
    log1.set_term(1);
    log1.mutable_entry()->set_run_event_id("ID1");
    log1.mutable_entry()->set_command("COMMAND1");

    e8::LogEntry log2;
    log2.set_term(2);
    log2.mutable_entry()->set_run_event_id("ID2");
    log2.mutable_entry()->set_command("COMMAND2");

    test_data->journal->AppendLog(log1);
    test_data->journal->AppendLog(log2);

    e8::RaftLogOffset bound = test_data->journal->EndWithTerm(/*term=*/2, /*end=*/2);
    TEST_CONDITION(bound == 2);

    bound = test_data->journal->EndWithTerm(/*term=*/2, /*end=*/3);
    TEST_CONDITION(bound == 2);

    bound = test_data->journal->EndWithTerm(/*term=*/1, /*end=*/2);
    TEST_CONDITION(bound == 1);

    bound = test_data->journal->EndWithTerm(/*term=*/3, /*end=*/2);
    TEST_CONDITION(bound == 0);

    bound = test_data->journal->EndWithTerm(/*term=*/2, /*end=*/1);
    TEST_CONDITION(bound == 0);

    return true;
}

bool PushCommitProgressTest() {
    e8::LogEntry log1;
    log1.set_term(1);
    log1.mutable_entry()->set_run_event_id("ID1");
    log1.mutable_entry()->set_command("COMMAND1");

    e8::LogEntry log2;
    log2.set_term(2);
    log2.mutable_entry()->set_run_event_id("ID2");
    log2.mutable_entry()->set_command("COMMAND2");

    auto commit_checker = std::make_unique<CommandCommitChecker>(
        std::vector<e8::CommandEntry>{log1.entry(), log2.entry()});
    auto test_data = std::make_unique<TestData>(commit_checker.get());

    test_data->journal->AppendLog(log1);
    test_data->journal->AppendLog(log2);

    test_data->journal->PushCommitProgress(/*safe_commit_progress=*/1, /*full_commit_progress=*/0);
    test_data->journal->PushCommitProgress(/*safe_commit_progress=*/0, /*full_commit_progress=*/0);
    test_data->journal->PushCommitProgress(/*safe_commit_progress=*/2, /*full_commit_progress=*/0);

    TEST_CONDITION(commit_checker->ApplyCount() == 2);

    return true;
}

bool CrashRecoveryTest() {
    auto no_op = std::make_unique<NoOpCommitListener>(
        /*preferred_snapshot_frequency=*/std::numeric_limits<e8::RaftLogOffset>::max());
    auto test_data = std::make_unique<TestData>(no_op.get());

    e8::LogEntry log1;
    log1.set_term(1);
    log1.mutable_entry()->set_run_event_id("ID1");
    log1.mutable_entry()->set_command("COMMAND1");

    test_data->journal->AppendLog(log1);

    test_data->journal = nullptr;
    test_data->persister = nullptr;

    test_data->persister = std::make_unique<e8::RaftPersister>(gRecoveryFile);
    test_data->journal = std::make_unique<e8::RaftJournal>(test_data->persister.get(), no_op.get());

    return true;
}

int main() {
    e8::BeginTestSuite("raft_journal");
    e8::RunTest("AppendAndExportTest", AppendAndExportTest);
    e8::RunTest("ImportAndExportTest", ImportAndExportTest);
    e8::RunTest("LivenessTest", LivenessTest);
    e8::RunTest("StalenessTest", StalenessTest);
    e8::RunTest("EndWithTermTest", EndWithTermTest);
    e8::RunTest("PushCommitProgressTest", PushCommitProgressTest);
    e8::RunTest("CrashRecoveryTest", CrashRecoveryTest);
    e8::EndTestSuite();
    return 0;
}
