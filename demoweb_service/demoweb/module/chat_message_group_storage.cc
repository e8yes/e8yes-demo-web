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
#include <memory>
#include <string>
#include <tuple>

#include "demoweb_service/demoweb/common_entity/chat_message_group_entity.h"
#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/module/chat_message_group_storage.h"
#include "demoweb_service/demoweb/pbac/message_channel_pbac.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/chat_message.pb.h"

namespace e8 {

ChatMessageGroupEntity
CreateChatMessageGroup(UserId const viewer_id, MessageChannelId const channel_id,
                       std::string const &group_title, ChatMessageThreadType const thread_type,
                       HostId const host_id, ConnectionReservoirInterface *conns) {
    ChatMessageGroupEntity chat_message_group;
    *chat_message_group.id.ValuePtr() = TimeId(host_id);
    *chat_message_group.channel_id.ValuePtr() = channel_id;
    *chat_message_group.description.ValuePtr() = group_title;
    *chat_message_group.creator_id.ValuePtr() = viewer_id;
    *chat_message_group.group_type.ValuePtr() = thread_type;

    std::time_t timestamp;
    std::time(&timestamp);
    *chat_message_group.created_at.ValuePtr() = timestamp;
    *chat_message_group.last_interaction_at.ValuePtr() = timestamp;

    int64_t num_rows =
        Update(chat_message_group, TableNames::ChatMessageGroup(), /*replace=*/false, conns);
    assert(num_rows == 1);

    return chat_message_group;
}

std::optional<ChatMessageGroupEntity> FetchChatMessageGroup(ChatMessageGroupId const group_id,
                                                            ConnectionReservoirInterface *conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> group_id_ph;
    query.QueryPiece(TableNames::ChatMessageGroup())
        .QueryPiece(" cmg WHERE cmg.id=")
        .Holder(&group_id_ph);

    query.SetValueToPlaceholder(group_id_ph, std::make_shared<SqlLong>(group_id));

    std::vector<std::tuple<ChatMessageGroupEntity>> query_result =
        Query<ChatMessageGroupEntity>(query, {"cmg"}, conns);
    if (query_result.empty()) {
        return std::nullopt;
    }

    assert(query_result.size() == 1);

    return std::get<0>(query_result[0]);
}

} // namespace e8
