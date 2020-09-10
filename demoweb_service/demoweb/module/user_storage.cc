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
#include <ctime>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/module/baseline_user.h"
#include "demoweb_service/demoweb/module/user_identity.h"
#include "demoweb_service/demoweb/module/user_storage.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

std::optional<UserEntity> CreateUser(std::string const &security_key,
                                     std::vector<std::string> const &user_group_names,
                                     std::optional<UserId> user_id, HostId const host_id,
                                     ConnectionReservoirInterface *db_conn) {
    if (!user_id.has_value()) {
        user_id = TimeId(host_id);
    }

    UserEntity user;
    *user.id.ValuePtr() = user_id.value();
    *user.id_str.ValuePtr() = std::to_string(user_id.value());

    // Stores a irreversibly hashed security key.
    std::optional<SecurityKeyHash> security_hash = DigestSecurityKey(security_key);
    assert(security_hash.has_value());
    *user.security_key_hash.ValuePtr() = security_hash.value();
    *user.group_names.ValuePtr() = user_group_names;
    *user.active_level.ValuePtr() = 0;

    std::time_t timestamp;
    std::time(&timestamp);
    *user.created_at.ValuePtr() = timestamp;

    uint64_t num_rows_affected = Update(user, TableNames::AUser(), /*overrdie=*/false, db_conn);
    if (num_rows_affected == 0) {
        return std::nullopt;
    }

    assert(num_rows_affected == 1);

    return user;
}

std::optional<UserEntity> FetchUser(UserId user_id, ConnectionReservoirInterface *db_conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> user_id_ph;
    query.QueryPiece(TableNames::AUser()).QueryPiece(" u WHERE u.id=").Holder(&user_id_ph);

    query.SetValueToPlaceholder(user_id_ph, std::make_shared<SqlLong>(user_id));

    std::vector<std::tuple<UserEntity>> results = Query<UserEntity>(query, {"u"}, db_conns);
    if (results.empty()) {
        return std::nullopt;
    }
    assert(results.size() == 1);

    return std::get<0>(results[0]);
}

std::vector<UserEntity> FetchUsers(std::vector<UserId> const &user_ids,
                                   ConnectionReservoirInterface *db_conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLongArr> user_ids_ph;
    query.QueryPiece(TableNames::AUser())
        .QueryPiece(" u WHERE u.id=ANY(")
        .Holder(&user_ids_ph)
        .QueryPiece(")");

    query.SetValueToPlaceholder(user_ids_ph, std::make_shared<SqlLongArr>(user_ids));

    std::vector<std::tuple<UserEntity>> query_result = Query<UserEntity>(query, {"u"}, db_conns);

    std::vector<UserEntity> users(query_result.size());
    for (unsigned i = 0; i < query_result.size(); ++i) {
        users[i] = std::get<0>(query_result[i]);
    }

    return users;
}

} // namespace e8
