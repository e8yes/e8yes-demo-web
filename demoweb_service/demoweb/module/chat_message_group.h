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

#include <optional>
#include <string>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/message_group_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

/**
 * @brief CreateMessageGroup
 * @param viewer_id
 * @param channel_id
 * @param group_title
 * @param conns
 * @return
 */
std::optional<MessageGroupEntity> CreateMessageGroup(std::optional<UserId> const viewer_id,
                                                     MessageChannelId const channel_id,
                                                     std::string const &group_title,
                                                     ConnectionReservoirInterface *conns);

} // namespace e8

#endif // CHAT_MESSAGE_GROUP_H
