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
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module/search_user.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/pagination.pb.h"

namespace e8 {

std::vector<UserEntity> SearchUser(std::optional<UserId> const &viewer_id,
                                   std::string const &query_text, Pagination const &pagination,
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

    if (viewer_id.has_value()) {
        query.QueryPiece(" ORDER BY cr.last_interaction_at DESC, u.alias ASC, u.id ASC");
    } else {
        query.QueryPiece(" ORDER BY u.alias ASC, u.id ASC");
    }

    std::vector<std::tuple<UserEntity>> query_results = Search<UserEntity>(
        query, {"u"}, /*search_target_entity=*/"u", query_text,
        /*prefix_search=*/true, /*rank_result=*/false, /*limit=*/pagination.result_per_page(),
        /*offset=*/pagination.page_number() * pagination.result_per_page(), db_conns);

    std::vector<UserEntity> results(query_results.size());
    for (unsigned i = 0; i < query_results.size(); i++) {
        results[i] = std::get<0>(query_results[i]);
    }
    return results;
}

} // namespace e8
