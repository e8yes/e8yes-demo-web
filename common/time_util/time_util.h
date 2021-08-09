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

#ifndef ACOMMON_TIME_H
#define ACOMMON_TIME_H

#include <cstdint>
#include <time.h>

namespace e8 {

using TimestampSecs = int64_t;
using TimestampMillis = int64_t;
using TimestampMicros = int64_t;

using TimeIntervalSecs = int64_t;
using TimeIntervalMillis = int64_t;
using TimeIntervalMicros = int64_t;

/**
 * @brief CurrentTimestampSecs Current Unix epoch with second precision.
 */
TimestampSecs CurrentTimestampSecs();

/**
 * @brief CurrentTimestampMillis Current Unix epoch with millisecond precision.
 */
TimestampMillis CurrentTimestampMillis();

/**
 * @brief CurrentTimestampMicros Current Unix epoch with microsecond precision.
 */
TimestampMicros CurrentTimestampMicros();

/**
 * @brief TemporalId Ever-non-decreasing ID. Uniqueness is not guaranteed in sub-microsecond level.
 * @return Ever-non-decreasing ID.
 */
int64_t TemporalId();

/**
 * @brief FutureTimeSpec Creates a time spec set to amount_millis in the future.
 * @param amount_millis The number of milliseconds further in the future.
 */
timespec FutureTimeSpec(TimestampMillis amount_millis);

} // namespace e8

#endif // ACOMMON_TIME_H
