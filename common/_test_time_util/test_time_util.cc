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
#include <thread>

#include "common/time_util/time_util.h"
#include "common/unit_test_util/unit_test_util.h"

bool TimestampRelativePrecisionTest() {
    e8::TimestampSecs timestamp_secs = e8::CurrentTimestampSecs();
    e8::TimestampMillis timestamp_millis = e8::CurrentTimestampMillis();
    e8::TimestampMicros timestamp_micros = e8::CurrentTimestampMicros();

    TEST_CONDITION(timestamp_secs > 0);
    TEST_CONDITION(timestamp_millis > 0);

    TEST_CONDITION(timestamp_millis / timestamp_secs >= 1000);
    TEST_CONDITION(timestamp_micros / timestamp_millis >= 1000);

    return true;
}

bool TimestampMicrosDurationTest() {
    e8::TimestampMicros timestamp1 = e8::CurrentTimestampMicros();
    std::this_thread::sleep_for(std::chrono::microseconds(2));
    e8::TimestampMicros timestamp2 = e8::CurrentTimestampMicros();

    TEST_CONDITION(timestamp2 > timestamp1);

    e8::TimestampMicros timestamp3 = e8::CurrentTimestampMicros();
    e8::TimestampMicros timestamp4 = e8::CurrentTimestampMicros();

    TEST_CONDITION(timestamp4 - timestamp3 < 10);

    return true;
}

bool TimestampMillisDurationTest() {
    e8::TimestampMillis timestamp1 = e8::CurrentTimestampMillis();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    e8::TimestampMillis timestamp2 = e8::CurrentTimestampMillis();

    TEST_CONDITION(timestamp2 > timestamp1);

    e8::TimestampMillis timestamp3 = e8::CurrentTimestampMillis();
    e8::TimestampMillis timestamp4 = e8::CurrentTimestampMillis();

    TEST_CONDITION(timestamp4 == timestamp3);

    return true;
}

bool TimestampSecsDurationTest() {
    e8::TimestampSecs timestamp1 = e8::CurrentTimestampSecs();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    e8::TimestampSecs timestamp2 = e8::CurrentTimestampSecs();

    TEST_CONDITION(timestamp2 > timestamp1);

    e8::TimestampSecs timestamp3 = e8::CurrentTimestampSecs();
    e8::TimestampSecs timestamp4 = e8::CurrentTimestampSecs();

    TEST_CONDITION(timestamp4 == timestamp3);

    return true;
}

int main() {
    e8::BeginTestSuite("time_util");
    e8::RunTest("TimestampRelativePrecisionTest", TimestampRelativePrecisionTest);
    e8::RunTest("TimestampMicrosDurationTest", TimestampMicrosDurationTest);
    e8::RunTest("TimestampMillisDurationTest", TimestampMillisDurationTest);
    e8::RunTest("TimestampSecsDurationTest", TimestampSecsDurationTest);
    e8::EndTestSuite();
    return 0;
}
