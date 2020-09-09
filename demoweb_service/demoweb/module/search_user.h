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

#ifndef SEARCH_USER_H
#define SEARCH_USER_H

#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "proto_cc/pagination.pb.h"
#include "proto_cc/user_relation.pb.h"

namespace e8 {

/**
 * @brief SearchUser Search for user entities condition on different optional filters. The search
 * result is ordered by last interaction timestamp between the viewer and the searched user in
 * descending order, user alias and ID in ascending order. If the viewer isn't present, the result
 * is ordered only by the user alias and user id in ascending order.
 *
 * @param query_text Raw query text string. Empty string will return empty result. So don't confuse
 * std::nullopt with empty string. An std::nullopt effectively turn of this text search option.
 * @param oneof_user_relations specifies that the result must contain at least one of these
 * directional relations from the viewer. If no viewer is provided, this filter is ignored.
 * @param pagination Pagination constraint.
 * @param db_conns Connections to the DemoWeb DB server.
 * @return The search result is a list of user entities.
 */
std::vector<UserEntity> SearchUser(std::optional<UserId> const &viewer_id,
                                   std::optional<std::string> const &query_text,
                                   std::unordered_set<UserRelation> const &oneof_user_relations,
                                   Pagination const &pagination,
                                   ConnectionReservoirInterface *db_conns);

} // namespace e8

#endif // SEARCH_USER_H
