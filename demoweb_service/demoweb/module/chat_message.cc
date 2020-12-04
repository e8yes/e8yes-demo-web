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
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/chat_message_group_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module/chat_message.h"
#include "demoweb_service/demoweb/module/chat_message_group_storage.h"
#include "demoweb_service/demoweb/module/chat_message_storage.h"
#include "demoweb_service/demoweb/pbac/message_channel_pbac.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/chat_message.pb.h"
#include "proto_cc/file.pb.h"

namespace e8 {
namespace {

ChatMessageEntry ToChatMessageEntry(ChatMessageEntity const &entity) {
    ChatMessageEntry chat_message;
    chat_message.set_thread_id(*entity.group_id.Value());
    chat_message.set_message_seq_id(*entity.message_seq_id.Value());
    chat_message.set_sender_id(*entity.sender_id.Value());
    chat_message.set_created_at(*entity.created_at.Value());
    *chat_message.mutable_texts() = {entity.text_entries.Value().begin(),
                                     entity.text_entries.Value().end()};
    return chat_message;
}

} // namespace

std::optional<SendChatMessageResult>
SendChatMessage(UserId const sender_id, ChatMessageGroupId const group_id,
                std::vector<std::string> const &texts,
                std::vector<FileFormat> const & /*media_file_formats*/,
                std::vector<FileFormat> const & /*binary_file_formats*/,
                MessageChannelPbacInterface *pbac, ConnectionReservoirInterface *conns) {
    std::optional<ChatMessageGroupEntity> group = FetchChatMessageGroup(group_id, conns);
    if (!group.has_value()) {
        return std::nullopt;
    }
    if (!pbac->AllowSendChatMessage(sender_id, *group->channel_id.Value())) {
        return std::nullopt;
    }

    ChatMessageEntity entity =
        CreateChatMessage(group_id, sender_id, texts,
                          /*binary_content_paths=*/std::vector<std::string>(), conns);

    switch (*group->group_type.Value()) {
    case CMTT_POPUP: {
        *group->last_interaction_at.ValuePtr() = *entity.created_at.Value();
        int64_t rows = Update(*group, TableNames::ChatMessageGroup(), /*replace=*/true, conns);
        assert(rows == 1);
        break;
    }
    case CMTT_TEMPORAL: {
        // Do nothing.
        break;
    }
    }

    SendChatMessageResult result;
    result.message = ToChatMessageEntry(entity);

    return result;
}

} // namespace e8
