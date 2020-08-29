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

#include "demoweb_service/demoweb/common_entity/user_group_has_file_entity.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {

UserGroupHasFileEntity::UserGroupHasFileEntity()
    : SqlEntityInterface({&group_name, &file_path, &can_read, &can_write}) {}

UserGroupHasFileEntity::UserGroupHasFileEntity(UserGroupHasFileEntity const &other)
    : UserGroupHasFileEntity() {
    group_name = other.group_name;
    file_path = other.file_path;
    can_read = other.can_read;
    can_write = other.can_write;
}

UserGroupHasFileEntity &UserGroupHasFileEntity::operator=(const UserGroupHasFileEntity &other) {
    group_name = other.group_name;
    file_path = other.file_path;
    can_read = other.can_read;
    can_write = other.can_write;
    return *this;
}

} // namespace e8
