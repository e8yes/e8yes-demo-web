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

#ifndef MESSAGE_CHANNEL_H
#define MESSAGE_CHANNEL_H

#include <ctime>
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "proto_cc/message_channel.pb.h"
#include "proto_cc/pagination.pb.h"

namespace e8 {

/**
 * @brief CreateMessageChannel Create a new message channel. A message channel allows communication
 * among users once they are members of. The communication can be encrypted as flagged by the
 * "encrypted" parameter. The argument close_group_channel specifies a more relaxed close group RBAC
 * policy for what the members can do.
 */
MessageChannelEntity CreateMessageChannel(UserId creator_id,
                                          std::optional<std::string> const &channel_name,
                                          std::optional<std::string> const &description,
                                          bool encrypted, bool close_group_channel, HostId host_id,
                                          ConnectionReservoirInterface *conns);

struct JoinedInMessageChannel {
    MessageChannelEntity message_channel;
    MessageChannelMemberType member_type;
    std::time_t join_at;
};

/**
 * @brief GetJoinedInMessageChannels Get all the message channels that the user pointed to by the
 * member_id is a member of. It also returns the member type the user is assigned in each specific
 * message channel. The result list is ordered by the last interaction timestamp where the most
 * recently interacted message channels rank first.
 *
 * @param has_member_ids A filter to required that a returning message channel must have all of the
 * specified members.
 */
std::vector<JoinedInMessageChannel>
GetJoinedInMessageChannels(UserId const member_id, std::vector<UserId> const &has_member_ids,
                           std::optional<Pagination> const &pagination,
                           ConnectionReservoirInterface *conns);

/**
 * @brief ToMessageChannels Converts message channel entities with user joining information to
 * message channel proto messages.
 */
std::vector<MessageChannel>
ToMessageChannels(std::vector<JoinedInMessageChannel> const &joining_info);

struct MessageChannelMember {
    UserEntity member;
    MessageChannelMemberType member_type;
    std::time_t join_at;
};

/**
 * @brief GetMessageChannelMembers Get all the channel members of the channel specified by the
 * channel_id. The result list is ordered by the last interaction timestamp where the most recently
 * interacted users rank first.
 */
std::vector<MessageChannelMember>
GetMessageChannelMembers(MessagechannelId channel_id, std::optional<Pagination> const &pagination,
                         ConnectionReservoirInterface *conns);

/**
 * @brief AddUserToMessageChannel Add a user to the specified message channel. The user may or may
 * not exist in the message channel. It can be used as a mean to modify the member type privilege.
 * The viewer, if specified, must be a member of the channel for this call to be successful. If the
 * requested member_type is an admin, then the viewer must be an admin also.
 *
 * @param channel_id ID of the target message channel to add the user to.
 * @param user_id ID of the user to be added.
 * @param member_type The requested member type.
 * @return Whether the viewer has sufficient privilege to perform this operation.
 */
bool AddUserToMessageChannel(std::optional<UserId> const &viewer_id, MessagechannelId channel_id,
                             UserId const user_id, MessageChannelMemberType const member_type,
                             ConnectionReservoirInterface *conns);

} // namespace e8

#endif // MESSAGE_CHANNEL_H
