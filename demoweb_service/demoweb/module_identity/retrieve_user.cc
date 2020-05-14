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
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module_identity/retrieve_user.h"
#include "demoweb_service/demoweb/module_identity/user_entity.h"
#include "demoweb_service/demoweb/proto_cc/pagination.pb.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

std::optional<UserEntity> RetrieveUser(UserId user_id, ConnectionReservoirInterface *db_conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> user_id_ph;
    query.query_piece(TableNames::AUser()).query_piece(" u WHERE u.id=").placeholder(&user_id_ph);

    SqlLong user_id_ph_value(user_id);
    query.set_value_to_placeholder(user_id_ph, &user_id_ph_value);

    std::vector<std::tuple<UserEntity>> results = Query<UserEntity>(query, {"u"}, db_conns);
    if (results.empty()) {
        return std::nullopt;
    }
    assert(results.size() == 1);

    return std::get<0>(results[0]);
}

std::vector<UserEntity> SearchUser(std::optional<UserId> const &user_id_prefix,
                                   std::optional<std::string> const &alias_prefix,
                                   Pagination const &pagination,
                                   ConnectionReservoirInterface *db_conns) {
    SqlQueryBuilder query;
    query.query_piece(TableNames::AUser());
    query.query_piece(" u WHERE TRUE");

    SqlStr user_id_prefix_ph_value("", "");
    if (user_id_prefix.has_value()) {
        SqlQueryBuilder::Placeholder<SqlStr> user_id_prefix_ph;
        query.query_piece(" AND u.id_str LIKE ").placeholder(&user_id_prefix_ph);

        *user_id_prefix_ph_value.value_ptr() = std::to_string(user_id_prefix.value()) + "%";
        query.set_value_to_placeholder(user_id_prefix_ph, &user_id_prefix_ph_value);
    }

    SqlStr alias_prefix_ph_value("", "");
    if (alias_prefix.has_value()) {
        SqlQueryBuilder::Placeholder<SqlStr> alias_prefix_ph;
        query.query_piece(" AND u.alias LIKE ").placeholder(&alias_prefix_ph);

        *alias_prefix_ph_value.value_ptr() = alias_prefix.value() + "%";
        query.set_value_to_placeholder(alias_prefix_ph, &alias_prefix_ph_value);
    }

    query.query_piece(" ORDER BY u.id ASC");

    SqlQueryBuilder::Placeholder<SqlInt> limit_ph;
    SqlQueryBuilder::Placeholder<SqlInt> offset_ph;
    query.query_piece(" LIMIT ")
        .placeholder(&limit_ph)
        .query_piece(" OFFSET ")
        .placeholder(&offset_ph);

    SqlInt limit_ph_value(pagination.result_per_page());
    SqlInt offset_ph_value(pagination.page_number() * pagination.result_per_page());
    query.set_value_to_placeholder(limit_ph, &limit_ph_value);
    query.set_value_to_placeholder(offset_ph, &offset_ph_value);

    std::vector<std::tuple<UserEntity>> query_results = Query<UserEntity>(query, {"u"}, db_conns);
    std::vector<UserEntity> results(query_results.size());
    for (unsigned i = 0; i < query_results.size(); i++) {
        results[i] = std::get<0>(query_results[i]);
    }
    return results;
}

} // namespace e8
