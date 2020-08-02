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
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module/retrieve_user.h"
#include "proto_cc/pagination.pb.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

std::optional<UserEntity> RetrieveUser(UserId user_id, ConnectionReservoirInterface *db_conns) {
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

std::vector<UserEntity> SearchUser(std::optional<UserId> const &viewer_id,
                                   std::optional<UserId> const &user_id_prefix,
                                   std::optional<std::string> const &alias_prefix,
                                   Pagination const &pagination,
                                   ConnectionReservoirInterface *db_conns) {
    SqlQueryBuilder query;
    query.QueryPiece(TableNames::AUser());
    query.QueryPiece(" u");

    if (viewer_id.has_value()) {
        SqlQueryBuilder::Placeholder<SqlInt> viewer_id_ph;

        query.QueryPiece(" LEFT JOIN ");
        query.QueryPiece(TableNames::ContactRelation());
        query.QueryPiece(" cr ON cr.src_user_id=").Holder(&viewer_id_ph);
        query.QueryPiece(" AND cr.dst_user_id=u.id");

        query.SetValueToPlaceholder(viewer_id_ph, std::make_shared<SqlInt>(*viewer_id));
    }

    query.QueryPiece(" WHERE FALSE");

    if (user_id_prefix.has_value()) {
        SqlQueryBuilder::Placeholder<SqlStr> user_id_prefix_ph;
        query.QueryPiece(" OR u.id_str LIKE ").Holder(&user_id_prefix_ph);
        query.SetValueToPlaceholder(
            user_id_prefix_ph,
            std::make_shared<SqlStr>(std::to_string(user_id_prefix.value()) + "%",
                                     /*field_name=*/""));
    }

    if (alias_prefix.has_value()) {
        SqlQueryBuilder::Placeholder<SqlStr> alias_prefix_ph;
        query.QueryPiece(" OR u.alias LIKE ").Holder(&alias_prefix_ph);
        query.SetValueToPlaceholder(alias_prefix_ph,
                                    std::make_shared<SqlStr>(alias_prefix.value() + "%",
                                                             /*field_name=*/""));
    }

    if (viewer_id.has_value()) {
        query.QueryPiece(" ORDER BY cr.last_interaction_at DESC, u.alias ASC, u.id ASC");
    } else {
        query.QueryPiece(" ORDER BY u.alias ASC, u.id ASC");
    }

    SqlQueryBuilder::Placeholder<SqlInt> limit_ph;
    SqlQueryBuilder::Placeholder<SqlInt> offset_ph;
    query.QueryPiece(" LIMIT ").Holder(&limit_ph).QueryPiece(" OFFSET ").Holder(&offset_ph);

    query.SetValueToPlaceholder(limit_ph, std::make_shared<SqlInt>(pagination.result_per_page()));
    query.SetValueToPlaceholder(offset_ph, std::make_shared<SqlInt>(pagination.page_number() *
                                                                    pagination.result_per_page()));

    std::vector<std::tuple<UserEntity>> query_results = Query<UserEntity>(query, {"u"}, db_conns);

    std::vector<UserEntity> results(query_results.size());
    for (unsigned i = 0; i < query_results.size(); i++) {
        results[i] = std::get<0>(query_results[i]);
    }
    return results;
}

} // namespace e8
