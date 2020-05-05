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

#include "demoweb_service/demoweb/module_identity/user_entity.h"

namespace e8 {

UserEntity::UserEntity()
    : SqlEntityInterface({&id, &id_str, &emails, &alias, &avatar_path, &avatar_preview_path,
                          &security_key_hash, &group_names, &active_level, &created_at}) {}

UserEntity::UserEntity(UserEntity const &other) : UserEntity() {
    id = other.id;
    id_str = other.id_str;
    emails = other.emails;
    alias = other.alias;
    avatar_path = other.avatar_path;
    avatar_preview_path = other.avatar_preview_path;
    security_key_hash = other.security_key_hash;
    group_names = other.group_names;
    active_level = other.active_level;
    created_at = other.created_at;
}

} // namespace e8
