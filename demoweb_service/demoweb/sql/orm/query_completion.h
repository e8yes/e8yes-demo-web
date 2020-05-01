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

#ifndef QUERY_COMPLETION_H
#define QUERY_COMPLETION_H

#include <cassert>
#include <initializer_list>
#include <string>

#include "sql/connection/connection_interface.h"
#include "sql/reflection/sql_entity_interface.h"
#include "sql/reflection/sql_primitive_interface.h"

namespace e8 {
namespace query_completion_internal {

template <typename EntityType>
std::string
CompleteSelectList(std::initializer_list<std::string>::const_iterator const &entity_alias_it,
                   std::initializer_list<std::string>::const_iterator const &end_it) {
    assert(entity_alias_it != end_it);
    std::string const fully_qualified_prefix = *entity_alias_it + ".";

    EntityType extracted_entity_fields;
    std::vector<SqlPrimitiveInterface *> const &fields = extracted_entity_fields.fields();

    std::string select_list = fully_qualified_prefix + fields[0]->field_name();
    for (unsigned i = 1; i < fields.size(); i++) {
        assert(fields[i] != nullptr);
        select_list += "," + fully_qualified_prefix + fields[i]->field_name();
    }
    return select_list;
}

template <typename EntityType1, typename EntityType2, typename... Others>
std::string
CompleteSelectList(std::initializer_list<std::string>::const_iterator const &entity_alias_it,
                   std::initializer_list<std::string>::const_iterator const &end_it) {
    return CompleteSelectList<EntityType1>(entity_alias_it, end_it) + "," +
           CompleteSelectList<EntityType2, Others...>(entity_alias_it + 1, end_it);
}

} // namespace query_completion_internal

/**
 * It takes in a partial select query that is missing the select list and completes the missing
 * select list to form a valid SQL select query.
 *
 * The caller must specify the entities involved in the select query in the template argument list
 * as well as the aliases for those entities used in the select query.
 * Example usage:
 * CompleteSelectQuery<User, CreditCard>(
 * "AUser auser JOIN CreditCard card ON card.user_id=auser.id WHERE auser.join_date > '2020-1-1'",
 * {"auser", "card"});
 *
 * @param query A partial query to be completed.
 * @param entity_aliases A list of aliases corresponding to the entities specified in the template
 * arguments.
 * @return A complete select SQL query.
 */
template <typename EntityType, typename... Others>
std::string CompleteSelectQuery(std::string const &query,
                                std::initializer_list<std::string> const &entity_aliases) {
    std::string select_list = query_completion_internal::CompleteSelectList<EntityType, Others...>(
        entity_aliases.begin(), entity_aliases.end());
    assert(!select_list.empty());
    return "SELECT " + select_list + " FROM " + query;
}

/**
 * @brief The InsertQueryInfo struct Contains the insertion query and the query parameters
 * associated with the query.
 */
struct InsertQueryAndParams {
    std::string query;
    ConnectionInterface::QueryParams query_params;
};

/**
 * @brief GenerateInsertQuery It takes in the target table name and entity that is going to insert
 * then constructs an insertion sql query as well as the query parameters associated with that
 * query.
 *
 * @param table_name Name of the table the entity is going to insert to.
 * @param entity The entity to be inserted.
 * @param with_upsert Whether or not to generate a query that will update the entity record if it
 * exists.
 * @return The insertion query and its parameters.
 */
InsertQueryAndParams GenerateInsertQuery(std::string const &table_name,
                                         SqlEntityInterface const &entity, bool with_upsert);

} // namespace e8

#endif // QUERY_COMPLETION_H
