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

#include <vector>

#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/orm/query_completion.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitive_interface.h"

namespace e8 {
namespace {

std::string ConstructInsertQuery(std::string const &table_name, SqlEntityInterface const &entity,
                                 bool with_upsert) {
    std::vector<SqlPrimitiveInterface *> const &fields = entity.Fields();
    assert(!fields.empty());

    std::string query = "INSERT INTO ";
    query += table_name;
    query += "(";
    query += fields[0]->FieldName();
    for (unsigned i = 1; i < fields.size(); i++) {
        query += ',';
        query += fields[i]->FieldName();
    }

    query += ")VALUES($1";
    unsigned i = 1;
    for (; i < fields.size(); i++) {
        query += ",$" + std::to_string(i + 1);
    }
    query.append(")");

    if (with_upsert) {
        // Update record on primary key conflict.
        query += "ON CONFLICT ON CONSTRAINT ";
        query += table_name + "_pkey DO UPDATE SET ";
        query += fields[0]->FieldName() + "=$" + std::to_string(i + 1);
        i++;
        for (unsigned j = 1; j < fields.size(); j++, i++) {
            query += ',';
            query += fields[j]->FieldName() + "=$" + std::to_string(i + 1);
        }
    } else {
        query += "ON CONFLICT DO NOTHING";
    }

    return query;
}

ConnectionInterface::QueryParams ConstructQueryParams(SqlEntityInterface const &entity,
                                                      bool with_upsert) {
    std::vector<SqlPrimitiveInterface *> const &fields = entity.Fields();
    assert(!fields.empty());

    ConnectionInterface::QueryParams params;
    unsigned i = 0;
    for (i = 0; i < fields.size(); i++) {
        params.SetParam(i, fields[i]);
    }

    if (with_upsert) {
        for (unsigned j = 0; j < fields.size(); i++, j++) {
            params.SetParam(i, fields[j]);
        }
    }

    return params;
}

} // namespace

InsertQueryAndParams GenerateInsertQuery(std::string const &table_name,
                                         SqlEntityInterface const &entity, bool with_upsert) {
    InsertQueryAndParams query_and_params;
    query_and_params.query = ConstructInsertQuery(table_name, entity, with_upsert);
    query_and_params.query_params = ConstructQueryParams(entity, with_upsert);

    return query_and_params;
}

} // namespace e8
