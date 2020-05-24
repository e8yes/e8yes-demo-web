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

#ifndef USERGROUPHASFILEENTITY_H
#define USERGROUPHASFILEENTITY_H

#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {

/**
 * @brief The UserGroupHasFileEntity class C++ class representation of the database table
 * "user_group_has_file".
 */
class UserGroupHasFileEntity : public SqlEntityInterface {
  public:
    UserGroupHasFileEntity();
    UserGroupHasFileEntity(UserGroupHasFileEntity const &other);
    ~UserGroupHasFileEntity() = default;

    UserGroupHasFileEntity &operator=(UserGroupHasFileEntity const &other);

    SqlStr group_name = SqlStr("group_name");
    SqlStr file_path = SqlStr("file_path");
    SqlBool can_read = SqlBool("can_read");
    SqlBool can_write = SqlBool("can_write");
};

} // namespace e8

#endif // USERGROUPHASFILEENTITY_H
