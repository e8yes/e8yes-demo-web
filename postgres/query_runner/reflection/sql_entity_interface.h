/**
 * e8yes demo web server.
 *
 * <p>Copyright (C) 2020 Yichen Ma {yichenm2@uci.edu}, Chifeng Wen {daviesx66@gmail.com}
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

#ifndef SQLENTITYINTERFACE_H
#define SQLENTITYINTERFACE_H

#include <initializer_list> // IWYU pragma: keep
#include <vector>

#include "postgres/query_runner/reflection/sql_primitive_interface.h"

namespace e8 {

/**
 * @brief The SqlEntityInterface class Represents some arbitrary sql entity that contains a list of
 * columns of SQL primitive types.
 *
 * Example usage:
 *
 * class User : public SqlEntityInterface {
 *   public:
 *     SqlInt id = SqlInt("id");
 *     SqlStr user_name = SqlStr("user_name");
 *     User() : SqlEntityInterface{&id, &user_name} {}
 * };
 */
class SqlEntityInterface {
  public:
    /**
     * @brief SqlEntityInterface Construct metadata about the fields that the entity has, as mean of
     * reflection.
     * @param fields The primitive fields the entity contains (that it is interested in).
     */
    SqlEntityInterface(std::initializer_list<SqlPrimitiveInterface *> const &fields);
    SqlEntityInterface(SqlEntityInterface const &) = delete;

    /**
     * @brief nested_fields The nested fields this entity contains.
     * @return The nested fields
     */
    std::vector<SqlPrimitiveInterface *> const &fields();

    /**
     * @brief fields Constant version of the above function.
     */
    std::vector<SqlPrimitiveInterface *> const &fields() const;

  private:
    std::vector<SqlPrimitiveInterface *> const fields_;
};

} // namespace e8

#endif // SQLENTITYINTERFACE_H
