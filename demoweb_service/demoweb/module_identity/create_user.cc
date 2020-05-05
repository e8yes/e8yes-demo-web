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
#include <crypt.h>
#include <cstdint>
#include <ctime>
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/constant/table_names.h"
#include "demoweb_service/demoweb/module_identity/create_user.h"
#include "demoweb_service/demoweb/module_identity/user_entity.h"
#include "demoweb_service/demoweb/module_rbac/system_user_group.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {
namespace {

static constexpr char kSaltCharacters[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

std::string GenSalt() {
    int64_t s0 = TimeId(/*host_id=*/0);
    unsigned i0 = static_cast<unsigned>(s0 % static_cast<int64_t>(sizeof(kSaltCharacters)));
    int64_t s1 = TimeId(/*host_id=*/1);
    unsigned i1 = static_cast<unsigned>(s1 % static_cast<int64_t>(sizeof(kSaltCharacters)));

    std::string salt;
    salt += kSaltCharacters[i0];
    salt += kSaltCharacters[i1];

    return salt;
}

std::string HashSecurityKey(std::string const &security_key) {
    crypt_data data;
    char *hash = crypt_r(security_key.c_str(), GenSalt().c_str(), &data);
    return std::string(hash);
}

} // namespace

UserEntity CreateUser(std::string const &security_key,
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
    *user.security_key_hash.value_ptr() = HashSecurityKey(security_key);
    *user.group_names.value_ptr() = user_group_names;
    *user.active_level.value_ptr() = 0;

    time_t timestamp;
    std::time(&timestamp);
    std::tm tm;
    gmtime_r(&timestamp, &tm);
    *user.created_at.value_ptr() = timestamp;

    uint64_t num_rows_affected = Update(user, TableNames::AUser(), /*overrdie=*/false, db_conn);
    assert(num_rows_affected == 1);

    return user;
}

UserEntity CreateBaselineUser(std::string const &security_key, std::optional<UserId> user_id,
                              unsigned host_id, ConnectionReservoirInterface *db_conn) {
    return CreateUser(
        security_key,
        std::vector<std::string>({kSystemUserGroupStrings[SystemUserGroup::BASELINE_USER_GROUP]}),
        user_id, host_id, db_conn);
}

} // namespace e8
