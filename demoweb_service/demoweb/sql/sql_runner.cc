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

#include <chrono>
#include <cstdint>
#include <exception>
#include <ios>
#include <memory>
#include <string>
#include <unordered_set>

#include "sql/connection/connection_interface.h"
#include "sql/connection/connection_reservoir_interface.h"
#include "sql/orm/query_completion.h"
#include "sql/reflection/sql_primitives.h"
#include "sql/resultset/result_set_interface.h"
#include "sql/sql_runner.h"

namespace e8 {
namespace {

int64_t reverse_bytes(int64_t original) {
    return (original & 0xFF) << 56 | ((original >> 8) & 0xFF) << 48 |
           ((original >> 16) & 0xFF) << 40 | ((original >> 24) & 0xFF) << 32 |
           ((original >> 32) & 0xFF) << 24 | ((original >> 40) & 0xFF) << 16 |
           ((original >> 48) & 0xFF) << 8 | ((original >> 56) & 0xFF);
}

} // namespace

uint64_t Update(SqlEntityInterface const &entity, std::string const &table_name, bool override,
                ConnectionReservoirInterface *reservoir) {
    InsertQueryAndParams query_and_params = GenerateInsertQuery(table_name, entity, override);

    ConnectionInterface *conn = reservoir->Take();
    uint64_t numRowsUpdated =
        conn->run_update(query_and_params.query, query_and_params.query_params);
    reservoir->Put(conn);

    return numRowsUpdated;
}

uint64_t Delete(std::string const &table_name, SqlQueryBuilder const &query,
                ConnectionReservoirInterface *reservoir) {
    std::string completed_query = "DELETE FROM " + table_name + " " + query.psql_query();

    ConnectionInterface *conn = reservoir->Take();
    uint64_t numRowsUpdated = conn->run_update(completed_query, query.query_params());
    reservoir->Put(conn);

    return numRowsUpdated;
}

bool Exists(SqlQueryBuilder const &query, ConnectionReservoirInterface *reservoir) {
    std::string exists_query = "SELECT TRUE FROM " + query.psql_query();

    ConnectionInterface *conn = reservoir->Take();

    std::unique_ptr<ResultSetInterface> rs = conn->run_query(exists_query, query.query_params());
    bool exists = rs->has_next();

    reservoir->Put(conn);

    return exists;
}

std::unordered_set<std::string> Tables(ConnectionReservoirInterface *reservoir) {
    ConnectionInterface *conn = reservoir->Take();
    std::string reflection_query =
        "SELECT tb.table_name FROM information_schema.tables tb WHERE tb.table_schema='public'";
    std::unique_ptr<ResultSetInterface> rs =
        conn->run_query(reflection_query, ConnectionInterface::QueryParams());

    std::unordered_set<std::string> table_names;
    SqlStr table_name("table_name");
    for (; rs->has_next(); rs->next()) {
        rs->set_field(0, &table_name);
        assert(table_name.value().has_value());
        table_names.insert(table_name.value().value());
    }

    reservoir->Put(conn);

    return table_names;
}

void SendHeartBeat(ConnectionReservoirInterface *reservoir) {
    ConnectionInterface *conn = reservoir->Take();

    std::unique_ptr<ResultSetInterface> rs =
        conn->run_query("SELECT 1", ConnectionInterface::QueryParams());

    if (!rs->has_next()) {
        throw std::ios_base::failure("Heart beat results in an empty result set.");
    }

    SqlInt one("heart_beat");
    rs->set_field(0, &one);
    if (!one.value().has_value() || one.value().value() != 1) {
        throw std::system_error();
    }

    reservoir->Put(conn);
}

int64_t TimeId() {
    auto now = std::chrono::high_resolution_clock::now();
    auto nanos = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
    auto dura = std::chrono::duration_cast<std::chrono::nanoseconds>(nanos.time_since_epoch());
    int64_t timestamp = dura.count();
    return reverse_bytes(timestamp);
}

int64_t SeqId(std::string const &seq_table, ConnectionReservoirInterface *reservoir) {
    ConnectionInterface *conn = reservoir->Take();

    std::unique_ptr<ResultSetInterface> rs =
        conn->run_query("SELECT nextval('" + seq_table + "')", ConnectionInterface::QueryParams());
    assert(rs->has_next());

    SqlLong id("id");
    rs->set_field(0, &id);
    assert(id.value().has_value());

    reservoir->Put(conn);

    return id.value().value();
}

} // namespace e8
