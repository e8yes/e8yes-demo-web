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

#include <cassert>
#include <cstdint>
#include <ctime>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/message_channel_has_user_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/module/message_channel.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/message_channel.pb.h"
#include "proto_cc/pagination.pb.h"

namespace e8 {

MessageChannelEntity CreateMessageChannel(UserId creator_id,
                                          std::optional<std::string> const &channel_name,
                                          std::optional<std::string> const &description,
                                          bool encrypted, bool close_group_channel, HostId host_id,
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

    MessageChannelHasUserEntity channel_user;
    *channel_user.channel_id.ValuePtr() = message_channel.id.Value();
    *channel_user.user_id.ValuePtr() = creator_id;
    *channel_user.ownership.ValuePtr() = MessageChannelMemberType::MCMT_ADMIN;
    *channel_user.created_at.ValuePtr() = current_timestamp;
    *channel_user.last_interaction_at.ValuePtr() = current_timestamp;

    num_rows = Update(channel_user, TableNames::MessageChannelHasUser(), /*replace=*/false, conns);
    assert(num_rows == 1);

    return message_channel;
}

std::vector<JoinedInMessageChannel>
GetJoinedInMessageChannels(UserId member_id, std::optional<Pagination> const &pagination,
                           ConnectionReservoirInterface *conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> member_id_ph;
    query.QueryPiece(TableNames::MessageChannel())
        .QueryPiece(" mc")
        .QueryPiece(" JOIN ")
        .QueryPiece(TableNames::MessageChannelHasUser())
        .QueryPiece(" mchu ON mchu.channel_id=mc.id")
        .QueryPiece(" WHERE mchu.user_id=")
        .Holder(&member_id_ph)
        .QueryPiece(" ORDER BY mchu.last_interaction_at DESC");

    query.SetValueToPlaceholder(member_id_ph, std::make_shared<SqlLong>(member_id));

    if (pagination.has_value()) {
        SqlQueryBuilder::Placeholder<SqlInt> limit_ph;
        SqlQueryBuilder::Placeholder<SqlInt> offset_ph;
        query.QueryPiece(" LIMIT ").Holder(&limit_ph).QueryPiece(" OFFSET ").Holder(&offset_ph);
        query.SetValueToPlaceholder(limit_ph,
                                    std::make_shared<SqlInt>(pagination->result_per_page()));
        query.SetValueToPlaceholder(
            offset_ph,
            std::make_shared<SqlInt>(pagination->page_number() * pagination->result_per_page()));
    }

    std::vector<std::tuple<MessageChannelEntity, MessageChannelHasUserEntity>> query_result =
        Query<MessageChannelEntity, MessageChannelHasUserEntity>(query, {"mc", "mchu"}, conns);

    std::vector<JoinedInMessageChannel> results(query_result.size());
    for (unsigned i = 0; i < query_result.size(); i++) {
        auto const &entry = query_result[i];

        JoinedInMessageChannel result;
        result.message_channel = std::get<0>(entry);
        result.member_type =
            static_cast<MessageChannelMemberType>(*std::get<1>(entry).ownership.Value());

        results[i] = result;
    }

    return results;
}

std::vector<MessageChannel>
ToMessageChannels(std::vector<JoinedInMessageChannel> const &joining_info) {
    std::vector<MessageChannel> proto_messages(joining_info.size());

    for (unsigned i = 0; i < joining_info.size(); i++) {
        auto const &info = joining_info[i];

        MessageChannel proto_message;
        proto_message.set_channel_id(*info.message_channel.id.Value());
        proto_message.set_title(*info.message_channel.channel_name.Value());
        proto_message.set_description(*info.message_channel.description.Value());
        proto_message.set_created_at(*info.message_channel.created_at.Value());

        MessageChannelRelation relation;
        relation.set_join_at(info.join_at);
        relation.set_member_type(info.member_type);

        *proto_message.mutable_relation() = relation;

        proto_messages[i] = proto_message;
    }

    return proto_messages;
}

std::vector<MessageChannelMember>
GetMessageChannelMembers(MessagechannelId channel_id, std::optional<Pagination> const &pagination,
                         ConnectionReservoirInterface *conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> channel_id_ph;
    query.QueryPiece(TableNames::AUser())
        .QueryPiece(" u")
        .QueryPiece(" JOIN ")
        .QueryPiece(TableNames::MessageChannelHasUser())
        .QueryPiece(" mchu ON mchu.user_id=u.id")
        .QueryPiece(" WHERE mchu.channel_id=")
        .Holder(&channel_id_ph)
        .QueryPiece(" ORDER BY mchu.last_interaction_at DESC");

    query.SetValueToPlaceholder(channel_id_ph, std::make_shared<SqlLong>(channel_id));

    if (pagination.has_value()) {
        SqlQueryBuilder::Placeholder<SqlInt> limit_ph;
        SqlQueryBuilder::Placeholder<SqlInt> offset_ph;
        query.QueryPiece(" LIMIT ").Holder(&limit_ph).QueryPiece(" OFFSET ").Holder(&offset_ph);
        query.SetValueToPlaceholder(limit_ph,
                                    std::make_shared<SqlInt>(pagination->result_per_page()));
        query.SetValueToPlaceholder(
            offset_ph,
            std::make_shared<SqlInt>(pagination->page_number() * pagination->result_per_page()));
    }

    std::vector<std::tuple<UserEntity, MessageChannelHasUserEntity>> query_result =
        Query<UserEntity, MessageChannelHasUserEntity>(query, {"u", "mchu"}, conns);

    std::vector<MessageChannelMember> results(query_result.size());
    for (unsigned i = 0; i < query_result.size(); i++) {
        std::tuple<UserEntity, MessageChannelHasUserEntity> const &entry = query_result[i];

        MessageChannelMember result;
        result.member = std::get<0>(entry);
        result.member_type =
            static_cast<MessageChannelMemberType>(*std::get<1>(entry).ownership.Value());
        result.join_at = *std::get<1>(entry).created_at.Value();

        results[i] = result;
    }

    return results;
}

} // namespace e8
