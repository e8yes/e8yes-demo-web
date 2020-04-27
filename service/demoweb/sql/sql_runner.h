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

#ifndef SQL_RUNNER_H
#define SQL_RUNNER_H

#include <initializer_list>
#include <string>
#include <tuple>
#include <vector>

#include "sql/connection/connection_interface.h"
#include "sql/connection/connection_reservoir_interface.h"
#include "sql/orm/data_collection.h"
#include "sql/orm/query_completion.h"
#include "sql/resultset/result_set_interface.h"
#include "sql/sql_query_builder.h"

namespace e8 {

/**
 * Constructs and runs query with partial information provided and synchronously returns the
 * result.
 *
 * The caller must specify the entities involved in the select query in the template argument list
 * as well as the aliases for those entities used in the select query.
 * Example usage:
 * std::vector<std::tuple<User, CreditCard>> results = Query<User, CreditCard>(query, {"auser",
 * "card"}, reservoir);
 *
 * @param query Partial query where the select list is unspecified. Example: "AUser auser JOIN
 * CreditCard card ON card.user_id = auser.id WHERE auser.join_date > '2020-1-1'"
 * @param entity_aliases A list of aliases corresponding to the entities specified in the template
 * arguments.
 * @param reservoir Connection reservoir to allocate database connections.
 * @return The query result containing a list of query record. Every record is a tuple of
 * entities in the order specified in the template.
 */
template <typename EntityType, typename... Others>
std::vector<std::tuple<EntityType, Others...>>
Query(SqlQueryBuilder const &query, std::initializer_list<std::string> const &entity_aliases,
      ConnectionReservoirInterface *reservoir) {
    std::string select_query =
        CompleteSelectQuery<EntityType, Others...>(query.psql_query(), entity_aliases);

    ConnectionInterface *conn = reservoir->Take();
    std::unique_ptr<ResultSetInterface> rs = conn->run_query(select_query, query.query_params());

    std::vector<std::tuple<EntityType, Others...>> results =
        ToEntityTuples<EntityType, Others...>(rs.get());

    reservoir->Put(conn);

    return results;
}

} // namespace e8

#endif // SQL_RUNNER_H
