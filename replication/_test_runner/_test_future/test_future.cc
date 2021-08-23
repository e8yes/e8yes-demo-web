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
#include <cmath>
#include <thread>
#include <utility>

#include "common/time_util/time_util.h"
#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/service_replication.pb.h"
#include "replication/runner/future.h"

namespace {

void FulfillFn(e8::ReplicationFuture *future, e8::TimeIntervalMillis fulfill_after) {
    std::this_thread::sleep_for(std::chrono::milliseconds(fulfill_after));
    future->FulFill(/*return_value=*/"fulfilled_return_value", e8::RunCommandError::RCE_NONE);
}

} // namespace

bool EvaluateUnfulfilledTest() {
    e8::ReplicationFuture future;

    TEST_CONDITION(!future.Fulfilled());

    e8::TimestampMillis start = e8::CurrentTimestampMillis();
    auto [return_value, error] = future.Evaluate(/*timeout=*/e8::TimeIntervalMillis(200));
    e8::TimestampMillis end = e8::CurrentTimestampMillis();

    TEST_CONDITION(return_value.empty());
    TEST_CONDITION(error == e8::RunCommandError::RCE_TIMED_OUT);
    TEST_CONDITION(std::abs(end - start - e8::TimeIntervalMillis(200)) <
                   e8::TimeIntervalMillis(10));

    TEST_CONDITION(!future.Fulfilled());

    auto [return_value2, error2] = future.Evaluate(/*timeout=*/e8::TimeIntervalMillis(200));
    TEST_CONDITION(return_value2.empty());
    TEST_CONDITION(error2 == e8::RunCommandError::RCE_TIMED_OUT);

    return true;
}

bool FulfillBeforeEvaluationTest() {
    e8::ReplicationFuture future;

    TEST_CONDITION(!future.Fulfilled());

    future.FulFill(/*return_value=*/"fulfilled_return_value", e8::RunCommandError::RCE_NONE);

    TEST_CONDITION(future.Fulfilled());

    e8::TimestampMillis start = e8::CurrentTimestampMillis();
    auto [return_value, error] = future.Evaluate(/*timeout=*/e8::TimeIntervalMillis(200));
    e8::TimestampMillis end = e8::CurrentTimestampMillis();

    TEST_CONDITION(return_value == "fulfilled_return_value");
    TEST_CONDITION(error == e8::RunCommandError::RCE_NONE);
    TEST_CONDITION(std::abs(end - start) < e8::TimeIntervalMillis(10));

    TEST_CONDITION(future.Fulfilled());

    return true;
}

bool FulfillAfterEvaluationTest() {
    e8::ReplicationFuture future;

    TEST_CONDITION(!future.Fulfilled());

    std::thread fulfillment_thread(FulfillFn, &future, e8::TimeIntervalMillis(100));

    e8::TimestampMillis start = e8::CurrentTimestampMillis();
    auto [return_value, error] = future.Evaluate(/*timeout=*/e8::TimeIntervalMillis(200));
    e8::TimestampMillis end = e8::CurrentTimestampMillis();

    fulfillment_thread.join();

    TEST_CONDITION(future.Fulfilled());
    TEST_CONDITION(return_value == "fulfilled_return_value");
    TEST_CONDITION(error == e8::RunCommandError::RCE_NONE);
    TEST_CONDITION(std::abs(end - start - e8::TimeIntervalMillis(100)) <
                   e8::TimeIntervalMillis(10));

    return true;
}

int main() {
    e8::BeginTestSuite("raft_runner_future");
    e8::RunTest("EvaluateUnfulfilledTest", EvaluateUnfulfilledTest);
    e8::RunTest("FulfillBeforeEvaluationTest", FulfillBeforeEvaluationTest);
    e8::RunTest("FulfillAfterEvaluationTest", FulfillAfterEvaluationTest);
    e8::EndTestSuite();
    return 0;
}
