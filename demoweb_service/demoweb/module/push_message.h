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

#ifndef PUSH_MESSAGE_H
#define PUSH_MESSAGE_H

#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "message_queue/publisher/publisher.h"
#include "proto_cc/real_time_message.pb.h"

namespace e8 {

/**
 * @brief PushMessageContent Pushes a keyed message to all the message publishers.
 *
 * @return The return code of each individual publisher.
 */
std::vector<bool> PushMessageContent(UserId const target_user_id,
                                     RealTimeMessageContent const &message_content,
                                     HostId const host_id,
                                     std::vector<MessagePublisherInterface *> const &publishers);

} // namespace e8

#endif // PUSH_MESSAGE_H
