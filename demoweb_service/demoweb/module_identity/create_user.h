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

#ifndef CREATE_USER_H
#define CREATE_USER_H

#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
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
                                     std::optional<UserId> user_id, unsigned host_id,
                                     ConnectionReservoirInterface *db_conn);

/**
 * @brief CreateBaselineUser Similar to the above, but the new user will be in the system's
 * baseline user group.
 *
 * @param security_key Security key associated with new user.
 * @param user_id Optional ID to create user with.
 *  If the ID is specified and the user with such ID exists, this function will return a nullopt.
 *  If the ID is empty, this function will generate a unique user ID.
 * @param zero-offset ID of the host machine the service is currently running on.
 * @param db_conn Connection to the DB server.
 * @return A newly created user with its associated unique ID if there is no error.
 */
std::optional<UserEntity> CreateBaselineUser(std::string const &security_key,
                                             std::optional<UserId> user_id, unsigned host_id,
                                             ConnectionReservoirInterface *db_conn);

} // namespace e8

#endif // CREATE_USER_H
