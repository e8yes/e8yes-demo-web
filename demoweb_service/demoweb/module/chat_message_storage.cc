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
#include <string>
#include <tuple>
#include <vector>

#include "demoweb_service/demoweb/common_entity/chat_message_entity.h"
#include "demoweb_service/demoweb/common_entity/chat_message_group_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/module/chat_message_storage.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/chat_message.pb.h"

namespace e8 {

ChatMessageEntity CreateChatMessage(ChatMessageGroupId const chat_message_group_id,
                                    UserId const sender_id,
                                    std::vector<std::string> const &text_entries,
                                    std::vector<std::string> const &binary_content_paths,
                                    ConnectionReservoirInterface *conns) {
    ChatMessageEntity chat_message;
    *chat_message.group_id.ValuePtr() = chat_message_group_id;
    *chat_message.message_seq_id.ValuePtr() = e8::TemporalId();
    *chat_message.sender_id.ValuePtr() = sender_id;
    *chat_message.text_entries.ValuePtr() = text_entries;
    *chat_message.binary_content_paths.ValuePtr() = binary_content_paths;
    std::time_t timestamp;
    std::time(&timestamp);
    *chat_message.created_at.ValuePtr() = timestamp;
    *chat_message.last_interaction_at.ValuePtr() = timestamp;

    int64_t num_rows = Update(chat_message, TableNames::ChatMessage(), /*replace=*/false, conns);
    assert(num_rows == 1);

    return chat_message;
}

std::optional<ChatMessageEntity> FetchChatMessage(ChatMessageId const chat_message_id,
                                                  ConnectionReservoirInterface *conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> chat_message_group_id_ph;
    SqlQueryBuilder::Placeholder<SqlLong> chat_message_seq_id_ph;
    query.QueryPiece(TableNames::ChatMessage())
        .QueryPiece(" cm WHERE cm.group_id=")
        .Holder(&chat_message_group_id_ph)
        .QueryPiece(" AND cm.message_seq_id=")
        .Holder(&chat_message_seq_id_ph);

    query.SetValueToPlaceholder(
        chat_message_group_id_ph,
        std::make_shared<SqlLong>(std::get<CMID_CHAT_MESSAGE_GROUP>(chat_message_id)));
    query.SetValueToPlaceholder(
        chat_message_seq_id_ph,
        std::make_shared<SqlLong>(std::get<CMID_CHAT_MESSAGE_SEQ>(chat_message_id)));

    std::vector<std::tuple<ChatMessageEntity>> query_result =
        Query<ChatMessageEntity>(query, {"cm"}, conns);
    if (query_result.empty()) {
        return std::nullopt;
    }

    return std::get<0>(query_result[0]);
}

} // namespace e8
