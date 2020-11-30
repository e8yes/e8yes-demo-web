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

#ifndef MESSAGEENTITY_H
#define MESSAGEENTITY_H

#include <cstdint>
#include <tuple>

#include "demoweb_service/demoweb/module/chat_message_group.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {

enum ChatMessageIdComps { CMID_CHAT_MESSAGE_GROUP = 0, CMID_CHAT_MESSAGE_SEQ };

using ChatMessageSeqWeakId = int64_t;
using ChatMessageId = std::tuple<ChatMessageGroupId, ChatMessageSeqWeakId>;

/**
 * @brief The MessageEntity class C++ class representation of the database table
 * "message". This entity is weakly functionally dependent on the chat message group. Note that: the
 * message sequence ID isn't unique across all chat messages. Uniqueness is only guaranteed within a
 * chat message group. Besides uniqueness, this sequence ID is guaranteed to be ever-increasing for
 * the purpose of temporal range search.
 */
class ChatMessageEntity : public SqlEntityInterface {
  public:
    ChatMessageEntity();
    ChatMessageEntity(ChatMessageEntity const &other);
    ~ChatMessageEntity() = default;

    ChatMessageEntity &operator=(ChatMessageEntity const &other);

    SqlLong group_id = SqlLong("group_id");
    SqlLong message_seq_id = SqlLong("message_seq_id");
    SqlLong sender_id = SqlLong("sender_id");
    SqlStrArr text_entries = SqlStrArr("text_content");
    SqlStrArr binary_content_paths = SqlStrArr("binary_content_paths");
    SqlStrArr media_file_paths = SqlStrArr("media_file_paths");
    SqlStrArr media_file_preview_paths = SqlStrArr("media_file_preview_paths");
    SqlTimestamp created_at = SqlTimestamp("created_at");
    SqlTimestamp last_interaction_at = SqlTimestamp("last_interaction_at");
};

} // namespace e8

#endif // MESSAGEENTITY_H
