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

#ifndef CHAT_MESSAGE_GROUP_H
#define CHAT_MESSAGE_GROUP_H

#include <cstdint>
#include <optional>
#include <vector>

#include "demoweb_service/demoweb/common_entity/chat_message_group_entity.h"
#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/pbac/message_channel_pbac.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "proto_cc/chat_message.pb.h"
#include "proto_cc/pagination.pb.h"

namespace e8 {

/**
 * @brief CreateChatMessageGroup Creates a new chat message group. A message group allows the
 * grouping of chat messages. The accessibility of the chat messages is controlled by the chat
 * message group's associated message channel. Please refer to the specific message channel PBAC to
 * see how exactly access control is evaluated.
 *
 * @param viewer_id ID of the chat message group's creator.
 * @param channel_id ID of the message channel the chat message group is associated with.
 * @param group_title Description of the chat message group.
 * @param thread_type The type of the chat message group which determines certain behaviors
 * triggered by an update.
 * @param pbac Policy based access controller for the associated message channel.
 * @param conns Database connections.
 * @return The newly created chat message group if the creator passes the access control evaluation.
 */
std::optional<ChatMessageGroupEntity>
CreateChatMessageGroup(UserId const viewer_id, MessageChannelId const channel_id,
                       std::string const &group_title, ChatMessageThreadType const thread_type,
                       HostId const host_id, MessageChannelPbacInterface *pbac,
                       ConnectionReservoirInterface *conns);

/**
 * @brief GetChatMessageGroups In the specified message channel, returns the chat message groups
 * with a summary list of chat messages of size not more than max_num_messages_per_group. The chat
 * message groups and the summary list is ranked by their last interaction timestamp. It also checks
 * the eligbility of the viewer to read from the message channel with rules provided by the PBAC.
 *
 * @param viewer_id ID of the user to perform this search action.
 * @param channel_id ID of the message channel to search from.
 * @param text_query If specified, the result is filtered and ranked by a full text search.
 * Otherwise, the result is ranked by the timestamp.
 * @param max_num_messages_per_group Maximum number of chat message summary to attach to each chat
 * message group in the result.
 * @param pagination Pagination on the chat message group list.
 * @param pbac Policy based access controller for the associated message channel.
 * @param conns Database connections.
 * @return The chat message groups with chat message summary list.
 */
std::vector<ChatMessageThread> GetChatMessageGroupsWithChatMessageSummaryList(
    UserId const viewer_id, MessageChannelId const channel_id,
    int32_t const max_num_messages_per_group, Pagination const pagination,
    MessageChannelPbacInterface *pbac, ConnectionReservoirInterface *conns);

} // namespace e8

#endif // CHAT_MESSAGE_GROUP_H
