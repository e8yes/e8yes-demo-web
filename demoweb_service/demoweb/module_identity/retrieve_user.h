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

#ifndef RETRIEVE_USER_H
#define RETRIEVE_USER_H

#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/module_identity/user_entity.h"
#include "demoweb_service/demoweb/proto_cc/pagination.pb.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

/**
 * @brief RetrieveUser Retrieve user entity by user ID..
 *
 * @param user_id ID of the user to be retrieved.
 * @param db_conns Connections to the DemoWeb DB server.
 * @return The user entity if exists.
 */
std::optional<UserEntity> RetrieveUser(UserId user_id, ConnectionReservoirInterface *db_conns);

/**
 * @brief SearchUser Search for user entities condition on different optional filters. The search
 * result is ordered by the user id in ascending order.
 *
 * @param user_id_prefix Filter by the prefix of the user ID.
 * @param alias_prefix Filter by the prefix of the user alias.
 * @param pagination Pagination constraint.
 * @param db_conns Connections to the DemoWeb DB server.
 * @return The search result is a list of user entities.
 */
std::vector<UserEntity> SearchUser(std::optional<UserId> const &user_id_prefix,
                                   std::optional<std::string> const &alias_prefix,
                                   Pagination const &pagination,
                                   ConnectionReservoirInterface *db_conns);

} // namespace e8

#endif // RETRIEVE_USER_H
