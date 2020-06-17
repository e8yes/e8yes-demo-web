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

#include "demoweb_service/demoweb/common_entity/contact_relation_entity.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {

ContactRelationEntity::ContactRelationEntity()
    : SqlEntityInterface({&src_user_id, &dst_user_id, &relation, &created_at}) {}

ContactRelationEntity::ContactRelationEntity(ContactRelationEntity const &other)
    : ContactRelationEntity() {
    src_user_id = other.src_user_id;
    dst_user_id = other.dst_user_id;
    relation = other.relation;
    created_at = other.created_at;
}

ContactRelationEntity &ContactRelationEntity::operator=(ContactRelationEntity const &other) {
    src_user_id = other.src_user_id;
    dst_user_id = other.dst_user_id;
    relation = other.relation;
    created_at = other.created_at;
    return *this;
}

} // namespace e8
