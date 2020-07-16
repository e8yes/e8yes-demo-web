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

#include <cassert>
#include <cstdint>
#include <ctime>
#include <string>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/message_channel_has_user_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/module/message_channel.h"
#include "demoweb_service/demoweb/proto_cc/message_channel.pb.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

MessageChannelEntity CreateMessageChannel(UserId creator_id, std::string const &channel_name,
                                          bool encrypted, HostId host_id,
                                          ConnectionReservoirInterface *conns) {
    MessageChannelEntity message_channel;
    *message_channel.id.ValuePtr() = TimeId(host_id);
    *message_channel.channel_name.ValuePtr() = channel_name;
    *message_channel.encryption_enabled.ValuePtr() = encrypted;
    std::time_t timestamp;
    std::time(&timestamp);
    *message_channel.created_at.ValuePtr() = timestamp;

    int64_t num_rows =
        Update(message_channel, TableNames::MessageChannel(), /*replace=*/false, conns);
    assert(num_rows == 1);

    MessageChannelHasUserEntity channel_user;
    *channel_user.channel_id.ValuePtr() = message_channel.id.Value();
    *channel_user.user_id.ValuePtr() = creator_id;
    *channel_user.ownership.ValuePtr() = MessageChannelUserType::MCUT_ADMIN;

    num_rows = Update(channel_user, TableNames::MessageChannelHasUser(), /*replace=*/false, conns);
    assert(num_rows == 1);

    return message_channel;
}

} // namespace e8
