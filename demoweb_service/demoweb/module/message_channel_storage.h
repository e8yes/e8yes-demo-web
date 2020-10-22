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

#ifndef MESSAGE_CHANNEL_STORAGE_H
#define MESSAGE_CHANNEL_STORAGE_H

#include <optional>
#include <string>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "proto_cc/message_channel.pb.h"

namespace e8 {

/**
 * @brief CreateMessageChannel Creates and persists a new message channel.
 * @return The newly created message channel.
 */
MessageChannelEntity CreateMessageChannel(std::optional<std::string> const &channel_name,
                                          std::optional<std::string> const &description,
                                          bool encrypted, bool close_group_channel, HostId host_id,
                                          ConnectionReservoirInterface *conns);

/**
 * @brief CreateMessageChannelMembership Create a new membership of a message channel specified by
 * the (channel_id, user_id) pair.
 */
bool CreateMessageChannelMembership(MessageChannelId const channel_id, UserId const user_id,
                                    MessageChannelMemberType const member_type,
                                    ConnectionReservoirInterface *conns);

/**
 * @brief UpdateMessageChannelMembership Update a membership of a message channel specified by the
 * (channel_id, user_id) pair.
 */
void UpdateMessageChannelMembership(MessageChannelId const channel_id, UserId const user_id,
                                    MessageChannelMemberType const member_type,
                                    ConnectionReservoirInterface *conns);

} // namespace e8

#endif // MESSAGE_CHANNEL_STORAGE_H
