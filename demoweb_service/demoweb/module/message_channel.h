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

#ifndef MESSAGE_CHANNEL_H
#define MESSAGE_CHANNEL_H

#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/proto_cc/message_channel.pb.h"
#include "demoweb_service/demoweb/proto_cc/pagination.pb.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

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
};

/**
 * @brief GetJoinedInMessageChannels Get all the message channels that the user pointed to by the
 * member_id is a member of. It also returns the member type the user is assigned in each specific
 * message channel.
 */
std::vector<JoinedInMessageChannel>
GetJoinedInMessageChannels(UserId member_id, std::optional<Pagination> const &pagination,
                           ConnectionReservoirInterface *conns);

} // namespace e8

#endif // MESSAGE_CHANNEL_H
