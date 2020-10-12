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

#ifndef MESSAGE_CHANNEL_ATTRIBUTES_H
#define MESSAGE_CHANNEL_ATTRIBUTES_H

#include <optional>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {

struct MessageChannelAttributes : public SqlEntityInterface {
    MessageChannelAttributes();
    MessageChannelAttributes(MessageChannelAttributes const &other);

    MessageChannelAttributes &operator=(MessageChannelAttributes const &rhs);

    SqlBool close_group_channel = SqlBool("close_group_channel");
};

/**
 * @brief ExtractMessageChannelAttributes Extract message channel's attributes needed for access
 * control.
 */
std::optional<MessageChannelAttributes>
ExtractMessageChannelAttributes(MessageChannelId const channel_id,
                                ConnectionReservoirInterface *conns);

} // namespace e8

#endif // MESSAGE_CHANNEL_ATTRIBUTES_H
