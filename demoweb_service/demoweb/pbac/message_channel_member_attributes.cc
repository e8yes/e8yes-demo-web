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
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/message_channel_has_user_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/pbac/message_channel_member_attributes.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/message_channel.pb.h"

namespace e8 {

std::unordered_map<UserId, MessageChannelMemberAttributes>
ExtractMessageChannelMemberAttributes(MessageChannelId const channel_id,
                                      ConnectionReservoirInterface *conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> channel_id_ph;
    query.QueryPiece(TableNames::MessageChannelHasUser())
        .QueryPiece(" mchu WHERE mchu.channel_id=")
        .Holder(&channel_id_ph);

    query.SetValueToPlaceholder(channel_id_ph, std::make_shared<SqlLong>(channel_id));

    std::vector<std::tuple<MessageChannelHasUserEntity>> members =
        Query<MessageChannelHasUserEntity>(query, {"mchu"}, conns);

    std::unordered_map<UserId, MessageChannelMemberAttributes> result;
    for (auto const &member : members) {
        MessageChannelHasUserEntity const &entity = std::get<0>(member);

        MessageChannelMemberAttributes attrs;
        attrs.member_type = static_cast<MessageChannelMemberType>(*entity.ownership.Value());

        result[*entity.user_id.Value()] = attrs;
    }

    return result;
}

std::optional<MessageChannelMemberAttributes>
ExtractMessageChannelMemberAttributes(MessageChannelId const channel_id, UserId const user_id,
                                      ConnectionReservoirInterface *conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> channel_id_ph;
    SqlQueryBuilder::Placeholder<SqlLong> user_id_ph;
    query.QueryPiece(TableNames::MessageChannelHasUser())
        .QueryPiece(" mchu WHERE mchu.channel_id=")
        .Holder(&channel_id_ph)
        .QueryPiece(" AND mchu.user_id=")
        .Holder(&user_id_ph);

    query.SetValueToPlaceholder(channel_id_ph, std::make_shared<SqlLong>(channel_id));
    query.SetValueToPlaceholder(user_id_ph, std::make_shared<SqlLong>(user_id));

    std::vector<std::tuple<MessageChannelHasUserEntity>> members =
        Query<MessageChannelHasUserEntity>(query, {"mchu"}, conns);

    if (members.empty()) {
        return std::nullopt;
    }

    assert(members.size() == 1);
    MessageChannelHasUserEntity const &entity = std::get<0>(members[0]);

    MessageChannelMemberAttributes attrs;
    attrs.member_type = static_cast<MessageChannelMemberType>(*entity.ownership.Value());

    return attrs;
}

} // namespace e8
