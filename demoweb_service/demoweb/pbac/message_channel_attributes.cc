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

#include <cassert>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/pbac/message_channel_attributes.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

MessageChannelAttributes::MessageChannelAttributes() : SqlEntityInterface({&close_group_channel}) {}

MessageChannelAttributes::MessageChannelAttributes(MessageChannelAttributes const &other)
    : MessageChannelAttributes() {
    close_group_channel = other.close_group_channel;
}

MessageChannelAttributes &MessageChannelAttributes::operator=(MessageChannelAttributes const &rhs) {
    close_group_channel = rhs.close_group_channel;
    return *this;
}

std::optional<MessageChannelAttributes>
ExtractMessageChannelAttributes(MessageChannelId const channel_id,
                                ConnectionReservoirInterface *conns) {
    SqlQueryBuilder attribute_query;
    SqlQueryBuilder::Placeholder<SqlLong> channel_id_ph;
    attribute_query.QueryPiece(TableNames::MessageChannel())
        .QueryPiece("mc WHERE mc.id=")
        .Holder(&channel_id_ph);

    attribute_query.SetValueToPlaceholder(channel_id_ph, std::make_shared<SqlLong>(channel_id));

    std::vector<std::tuple<MessageChannelAttributes>> result =
        Query<MessageChannelAttributes>(attribute_query, {"mc"}, conns);

    if (result.empty()) {
        return std::nullopt;
    }

    assert(result.size() == 1);
    return std::get<0>(result[0]);
}

} // namespace e8
