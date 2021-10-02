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

#ifndef REPLICATION_COMMON_TYPES_H
#define REPLICATION_COMMON_TYPES_H

#include <mutex>
#include <string>

#include "third_party/uuid/uuid4.h"

namespace e8 {

// A unique ID that identifies the event which a fulfillment task for a specific command begins.
using ReplicationRunEventId = std::string;

/**
 * @brief The ReplicationRunEventIdGenerator class Helps generate unique run event IDs. This class
 * is thread safe.
 */
class ReplicationRunEventIdGenerator {
  public:
    ReplicationRunEventIdGenerator();
    ~ReplicationRunEventIdGenerator();

    /**
     * @brief Generate Generates a new unique run event ID.
     */
    ReplicationRunEventId Generate();

  private:
    UUID4_STATE_T uuid_state_;
    std::mutex mu_;
};

} // namespace e8

#endif // REPLICATION_COMMON_TYPES_H
