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
#include <cstdlib>
#include <limits>
#include <memory>
#include <string>
#include <utility>

#include "common/time_util/time_util.h"
#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/command.pb.h"
#include "replication/runner/command_queue.h"
#include "replication/runner/fulfillment_pool.h"
#include "replication/runner/future.h"

namespace {

class Adder : public e8::CommandRunnerInterface {
  public:
    Adder();
    ~Adder();

    std::string Run(std::string const &command) override;

    e8::RaftLogOffset PreferredSnapshotFrequency() const override;

    void Save() const override;

    void Restore() override;

    long CurrentValue() const;

  private:
    long current_value_;
};

Adder::Adder() : current_value_(0) {}

Adder::~Adder() {}

std::string Adder::Run(std::string const &command) {
    current_value_ += std::atol(command.c_str());
    return std::to_string(current_value_);
}

e8::RaftLogOffset Adder::PreferredSnapshotFrequency() const {
    return std::numeric_limits<e8::RaftLogOffset>::max();
}

void Adder::Save() const { assert(false); }

void Adder::Restore() { assert(false); }

long Adder::CurrentValue() const { return current_value_; }

} // namespace

bool ApplyRepeatedSequentialCommandTest() {
    Adder adder;
    e8::FulfillmentPool fulfillment_pool;
    e8::CommandQueueProcessor processor(&fulfillment_pool, &adder);

    e8::CommandEntry command1;
    command1.set_run_event_id("1");
    command1.set_command("11");

    e8::CommandEntry command2;
    command2.set_run_event_id("2");
    command2.set_command("-22");

    e8::CommandEntry command3;
    command3.set_run_event_id("3");
    command3.set_command("33");

    // Command futures.
    e8::ReplicationFuture *command1_result = fulfillment_pool.NewRunEvent("1");
    e8::ReplicationFuture *command2_result = fulfillment_pool.NewRunEvent("2");
    e8::ReplicationFuture *command3_result = fulfillment_pool.NewRunEvent("3");

    // The adder should see command1, command2 and command3, in order.
    processor.Apply(command1);
    processor.Apply(command1);

    processor.Apply(command2);
    processor.Apply(command1);

    processor.Apply(command3);
    processor.Apply(command2);
    processor.Apply(command1);

    TEST_CONDITION(command1_result->Fulfilled());
    TEST_CONDITION(command2_result->Fulfilled());
    TEST_CONDITION(command3_result->Fulfilled());

    auto [command1_return_value, _1] = command1_result->Evaluate(e8::TimeIntervalMillis(10));
    TEST_CONDITION(command1_return_value == "11");

    auto [command2_return_value, _2] = command2_result->Evaluate(e8::TimeIntervalMillis(10));
    TEST_CONDITION(command2_return_value == "-11");

    auto [command3_return_value, _3] = command3_result->Evaluate(e8::TimeIntervalMillis(10));
    TEST_CONDITION(command3_return_value == "22");

    TEST_CONDITION(adder.CurrentValue() == 22);

    return true;
}

int main() {
    e8::BeginTestSuite("replication_command_queue");
    e8::RunTest("ApplyRepeatedSequentialCommandTest", ApplyRepeatedSequentialCommandTest);
    e8::EndTestSuite();
    return 0;
}
