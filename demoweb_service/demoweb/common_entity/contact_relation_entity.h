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

#ifndef CONTACTRELATIONENTITY_H
#define CONTACTRELATIONENTITY_H

#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {

/**
 * @brief The ContactRelationEntity class C++ class representation of the database table
 * "contact_relation".
 */
class ContactRelationEntity : public SqlEntityInterface {
  public:
    ContactRelationEntity();
    ContactRelationEntity(ContactRelationEntity const &other);
    ~ContactRelationEntity() = default;

    ContactRelationEntity &operator=(ContactRelationEntity const &other);

    SqlLong src_user_id = SqlLong("src_user_id");
    SqlLong dst_user_id = SqlLong("dst_user_id");
    SqlInt relation = SqlInt("relation");
    SqlTimestamp created_at = SqlTimestamp("created_at");
};

} // namespace e8

#endif // CONTACTRELATIONENTITY_H
