/**
 * e8yes demo web server.
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

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {

MessageChannelEntity::MessageChannelEntity()
    : SqlEntityInterface({&id, &channel_name, &description, &encryption_enabled,
                          &close_group_channel, &created_at}) {}

MessageChannelEntity::MessageChannelEntity(MessageChannelEntity const &other)
    : MessageChannelEntity() {
    id = other.id;
    channel_name = other.channel_name;
    description = other.description;
    encryption_enabled = other.encryption_enabled;
    close_group_channel = other.close_group_channel;
    created_at = other.created_at;
}

MessageChannelEntity &MessageChannelEntity::operator=(MessageChannelEntity const &other) {
    id = other.id;
    channel_name = other.channel_name;
    description = other.description;
    encryption_enabled = other.encryption_enabled;
    close_group_channel = other.close_group_channel;
    created_at = other.created_at;
    return *this;
}

} // namespace e8
