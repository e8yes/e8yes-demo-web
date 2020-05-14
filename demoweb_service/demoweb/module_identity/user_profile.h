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

#ifndef USER_PROFILE_H
#define USER_PROFILE_H

#include <optional>
#include <string>

#include "demoweb_service/demoweb/module_identity/user_entity.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

/**
 * @brief UpdateProfile Update a user's profile. See below for what parameters a user's profile
 * have.
 *
 * @param user Table entity of the user whose profile needs to be updated. The content of this
 * entity will be updated with the specified parameters after the function call.
 * @param db_conns Connections to the DemoWeb DB server.
 * @return If the user pointed to by the ID of the entity doesn't exist, it will return false.
 * Otherwise, it returns true.
 */
bool UpdateProfile(std::optional<std::string> const &alias, UserEntity *user,
                   ConnectionReservoirInterface *db_conns);

} // namespace e8

#endif // USER_PROFILE_H
