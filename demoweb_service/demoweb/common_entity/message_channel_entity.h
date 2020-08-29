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

#ifndef MESSAGECHANNELENTITY_H
#define MESSAGECHANNELENTITY_H

#include <cstdint>

#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {

using MessagechannelId = int64_t;

/**
 * @brief The MessageChannelEntity class C++ class representation of the database table
 * "message_channel".
 */
class MessageChannelEntity : public SqlEntityInterface {
  public:
    MessageChannelEntity();
    MessageChannelEntity(MessageChannelEntity const &other);
    ~MessageChannelEntity() = default;

    MessageChannelEntity &operator=(MessageChannelEntity const &other);

    SqlLong id = SqlLong("id");
    SqlStr channel_name = SqlStr("channel_name");
    SqlStr description = SqlStr("description");
    SqlBool encryption_enabled = SqlBool("encryption_enabled");
    SqlBool close_group_channel = SqlBool("close_group_channel");
    SqlTimestamp created_at = SqlTimestamp("created_at");
};

} // namespace e8

#endif // MESSAGECHANNELENTITY_H
