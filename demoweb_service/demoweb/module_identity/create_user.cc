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
#include <ctime>
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module_identity/create_user.h"
#include "demoweb_service/demoweb/module_identity/user_identity.h"
#include "demoweb_service/demoweb/module_rbac/system_user_group.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

std::optional<UserEntity> CreateUser(std::string const &security_key,
                                     std::vector<std::string> const &user_group_names,
                                     std::optional<UserId> user_id, unsigned host_id,
                                     ConnectionReservoirInterface *db_conn) {
    if (!user_id.has_value()) {
        user_id = TimeId(host_id);
    }

    UserEntity user;
    *user.id.value_ptr() = user_id.value();
    *user.id_str.value_ptr() = std::to_string(user_id.value());

    // Stores a irreversibly hashed security key.
    std::optional<SecurityKeyHash> security_hash = DigestSecurityKey(security_key);
    assert(security_hash.has_value());
    *user.security_key_hash.value_ptr() = security_hash.value();
    *user.group_names.value_ptr() = user_group_names;
    *user.active_level.value_ptr() = 0;

    time_t timestamp;
    std::time(&timestamp);
    *user.created_at.value_ptr() = timestamp;

    uint64_t num_rows_affected = Update(user, TableNames::AUser(), /*overrdie=*/false, db_conn);
    if (num_rows_affected == 0) {
        return std::nullopt;
    }

    assert(num_rows_affected == 1);

    return user;
}

std::optional<UserEntity> CreateBaselineUser(std::string const &security_key,
                                             std::optional<UserId> user_id, unsigned host_id,
                                             ConnectionReservoirInterface *db_conn) {
    return CreateUser(
        security_key,
        std::vector<std::string>({kSystemUserGroupStrings[SystemUserGroup::BASELINE_USER_GROUP]}),
        user_id, host_id, db_conn);
}

} // namespace e8
