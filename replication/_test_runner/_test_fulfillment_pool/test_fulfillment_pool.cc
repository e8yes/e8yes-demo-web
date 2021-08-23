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

#include <utility>

#include "common/time_util/time_util.h"
#include "common/unit_test_util/unit_test_util.h"
#include "replication/runner/fulfillment_pool.h"
#include "replication/runner/future.h"

bool NewRunEventAndFulfillTest() {
    e8::FulfillmentPool pool;

    e8::ReplicationFuture *future = pool.NewRunEvent(/*run_event_id=*/"run_event_1");
    TEST_CONDITION(!future->Fulfilled());
    TEST_CONDITION(!pool.Fulfilled(/*run_event_id=*/"run_event_1"));

    pool.Fulfill(/*run_event_id=*/"run_event_1", /*return_value=*/"1_return_value",
                 /*error=*/e8::RunCommandError::RCE_NONE);

    TEST_CONDITION(future->Fulfilled());
    auto [return_value, error] = future->Evaluate(/*timeout=*/e8::TimeIntervalMillis(10));
    TEST_CONDITION(return_value == "1_return_value");
    TEST_CONDITION(error == e8::RunCommandError::RCE_NONE);

    e8::ReplicationFuture *future2 = pool.NewRunEvent(/*run_event_id=*/"run_event_1");
    TEST_CONDITION(future2 == future);

    return true;
}

int main() {
    e8::BeginTestSuite("replication_fulfillment_pool");
    e8::RunTest("NewRunEventTest", NewRunEventAndFulfillTest);
    e8::EndTestSuite();
    return 0;
}
