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
#include <cstdint>
#include <ctime>
#include <optional>
#include <string>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/message_channel_has_user_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/module/message_channel_storage.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {
namespace {

MessageChannelHasUserEntity
ToMessageChannelHasUserEntity(MessageChannelId channel_id, UserId const user_id,
                              MessageChannelMemberType const member_type) {
    MessageChannelHasUserEntity channel_member;
    *channel_member.channel_id.ValuePtr() = channel_id;
    *channel_member.user_id.ValuePtr() = user_id;
    *channel_member.ownership.ValuePtr() = member_type;

    std::time_t timestamp;
    std::time(&timestamp);
    *channel_member.created_at.ValuePtr() = timestamp;
    *channel_member.last_interaction_at.ValuePtr() = timestamp;

    return channel_member;
}

} // namespace

MessageChannelEntity CreateMessageChannel(std::optional<std::string> const &channel_name,
                                          std::optional<std::string> const &description,
                                          bool const encrypted, bool const close_group_channel,
                                          HostId const host_id,
                                          ConnectionReservoirInterface *conns) {

    std::time_t current_timestamp;
    std::time(&current_timestamp);

    MessageChannelEntity message_channel;
    *message_channel.id.ValuePtr() = TimeId(host_id);
    *message_channel.channel_name.ValuePtr() = channel_name;
    *message_channel.description.ValuePtr() = description;
    *message_channel.encryption_enabled.ValuePtr() = encrypted;
    *message_channel.close_group_channel.ValuePtr() = close_group_channel;
    *message_channel.created_at.ValuePtr() = current_timestamp;

    int64_t num_rows =
        Update(message_channel, TableNames::MessageChannel(), /*replace=*/false, conns);
    assert(num_rows == 1);

    return message_channel;
}

bool CreateMessageChannelMembership(MessageChannelId const channel_id, UserId const user_id,
                                    MessageChannelMemberType const member_type,
                                    ConnectionReservoirInterface *conns) {
    MessageChannelHasUserEntity channel_member =
        ToMessageChannelHasUserEntity(channel_id, user_id, member_type);
    int64_t num_rows =
        Update(channel_member, TableNames::MessageChannelHasUser(), /*replace=*/false, conns);

    return num_rows == 1;
}

void UpdateMessageChannelMembership(MessageChannelId channel_id, UserId const user_id,
                                    MessageChannelMemberType const member_type,
                                    ConnectionReservoirInterface *conns) {
    MessageChannelHasUserEntity channel_member =
        ToMessageChannelHasUserEntity(channel_id, user_id, member_type);
    int64_t num_rows =
        Update(channel_member, TableNames::MessageChannelHasUser(), /*replace=*/true, conns);

    assert(num_rows == 1);
}

bool DeleteMessageChannelMembership(MessageChannelId const channel_id, UserId const user_id,
                                    ConnectionReservoirInterface *conns) {
    SqlQueryBuilder removal_query;
    SqlQueryBuilder::Placeholder<SqlLong> channel_id_ph;
    SqlQueryBuilder::Placeholder<SqlLong> user_id_ph;

    removal_query.QueryPiece("WHERE channel_id=")
        .Holder(&channel_id_ph)
        .QueryPiece(" AND user_id=")
        .Holder(&user_id_ph);

    removal_query.SetValueToPlaceholder(channel_id_ph, std::make_shared<SqlLong>(channel_id));
    removal_query.SetValueToPlaceholder(user_id_ph, std::make_shared<SqlLong>(user_id));

    return 1L == Delete(TableNames::MessageChannelHasUser(), removal_query, conns);
}

} // namespace e8
