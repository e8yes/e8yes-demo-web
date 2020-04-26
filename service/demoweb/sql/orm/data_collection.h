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

#ifndef DATA_COLLECTION_H
#define DATA_COLLECTION_H

#include <tuple>
#include <vector>

#include "sql/reflection/sql_entity_interface.h"
#include "sql/reflection/sql_primitive_interface.h"
#include "sql/resultset/result_set_interface.h"

namespace e8 {
namespace data_collection_internal {

template <typename EntityType>
void SetRecordsToEntities(ResultSetInterface *rs, unsigned *base_record_idx, EntityType &&entity) {
    std::vector<SqlPrimitiveInterface *> const &fields = entity.fields();
    for (unsigned i = 0; i < fields.size(); i++) {
        rs->set_field(*base_record_idx + i, fields[i]);
    }
    *base_record_idx += fields.size();
}

template <typename EntityType1, typename EntityType2, typename... Others>
void SetRecordsToEntities(ResultSetInterface *rs, unsigned *base_record_idx, EntityType1 &&entity1,
                          EntityType2 &&entity2, Others... others) {
    SetRecordsToEntities(rs, base_record_idx, entity1);
    SetRecordsToEntities(rs, base_record_idx, entity2, others...);
}

} // namespace data_collection_internal

/**
 * @brief ToEntityTuples
 * @param rs
 * @return
 */
template <typename EntityType, typename... Others>
std::vector<std::tuple<EntityType, Others...>> ToEntityTuples(ResultSetInterface *rs) {
    std::vector<std::tuple<EntityType, Others...>> records;
    for (; rs->has_next(); rs->next()) {
        std::tuple<EntityType, Others...> entity_tuple;

        unsigned base_record_idx = 0;
        std::apply(
            [rs, &base_record_idx](auto &&... entities) {
                data_collection_internal::SetRecordsToEntities(rs, &base_record_idx, entities...);
            },
            entity_tuple);

        records.push_back(entity_tuple);
    }
    return records;
}

} // namespace e8

#endif // DATA_COLLECTION_H
