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

#include <vector>

#include "common/time_util/time_util.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/module/push_message.h"
#include "message_queue/publisher/publisher.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/real_time_message.pb.h"

namespace e8 {

std::vector<bool> PushMessageContent(UserId const target_user_id,
                                     RealTimeMessageContent const &message_content,
                                     HostId const host_id,
                                     std::vector<MessagePublisherInterface *> const &publishers) {
    RealTimeMessage message;
    message.set_real_time_message_id(TimeId(host_id));
    message.set_target_user_id(target_user_id);
    message.set_created_at(CurrentTimestampMicros());

    *message.mutable_content() = message_content;

    std::vector<bool> rcs;
    for (MessagePublisherInterface *publisher : publishers) {
        bool rc = publisher->Publish(target_user_id, message);
        rcs.push_back(rc);
    }
    return rcs;
}

} // namespace e8
