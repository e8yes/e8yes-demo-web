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

#ifndef RETRIEVE_USER_H
#define RETRIEVE_USER_H

#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

/**
 * @brief CreateUser Create a user of arbitrary group.
 *
 * @param security_key Security key associated with new user.
 * @param user_group_names Name of the groups the user will be in.
 * @param user_id Optional ID to create user with.
 *  If the ID is specified and the user with such ID exists, this function will return a nullopt.
 *  If the ID is empty, this function will generate a unique user ID.
 * @param zero-offset ID of the host machine the service is currently running on.
 * @param db_conn Connection to the DB server.
 * @return A newly created user with its associated unique ID if there is no error.
 */
std::optional<UserEntity> CreateUser(std::string const &security_key,
                                     std::vector<std::string> const &user_group_names,
                                     std::optional<UserId> user_id, HostId const host_id,
                                     ConnectionReservoirInterface *db_conn);

/**
 * @brief FetchUser Fetch user entity by user ID.
 *
 * @param user_id ID of the user to be retrieved.
 * @param db_conns Connections to the DemoWeb DB server.
 * @return The user entity if exists.
 */
std::optional<UserEntity> FetchUser(UserId user_id, ConnectionReservoirInterface *db_conns);

/**
 * @brief FetchUsers Similar to the above function, it fetches a set of users at the same time
 * instead.
 */
std::vector<UserEntity> FetchUsers(std::vector<UserId> const &user_ids,
                                   ConnectionReservoirInterface *db_conns);

} // namespace e8

#endif // RETRIEVE_USER_H
