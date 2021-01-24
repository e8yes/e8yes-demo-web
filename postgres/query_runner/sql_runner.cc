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

#include <chrono>
#include <cstdint>
#include <exception>
#include <ios>
#include <memory>
#include <string>
#include <unordered_set>

#include "common/time_util/time_util.h"
#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/orm/query_completion.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/resultset/result_set_interface.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {
namespace sql_runner_internal {

int64_t ReverseBytes(int64_t original) {
    return (original & 0xFF) << 56 | ((original >> 8) & 0xFF) << 48 |
           ((original >> 16) & 0xFF) << 40 | ((original >> 24) & 0xFF) << 32 |
           ((original >> 32) & 0xFF) << 24 | ((original >> 40) & 0xFF) << 16 |
           ((original >> 48) & 0xFF) << 8 | ((original >> 56) & 0xFF);
}

unsigned SkipWhiteSpaces(std::string const &text, unsigned i_reader) {
    while (i_reader < text.size() && text[i_reader] == ' ') {
        ++i_reader;
    }
    return i_reader;
}

void TokenizePlainTextQuery(std::string *plain_text) {
    unsigned i_reader = SkipWhiteSpaces(*plain_text, /*i_reader=*/0);
    unsigned i_writer = 0;

    while (i_reader < plain_text->size()) {
        (*plain_text)[i_writer] = (*plain_text)[i_reader];

        if ((*plain_text)[i_reader] == ' ') {
            i_reader = SkipWhiteSpaces(*plain_text, i_reader);
            (*plain_text)[i_writer] = '&';
            ++i_writer;
        } else {
            ++i_reader;
            ++i_writer;
        }
    }

    if (i_writer > 0 && (*plain_text)[i_writer - 1] == '&') {
        plain_text->resize(i_writer - 1);
    } else {
        plain_text->resize(i_writer);
    }
}
std::string ToSearchQuery(std::string const &target_collection, std::string const &full_text_query,
                          bool prefix_search, bool rank_result, std::optional<unsigned> limit,
                          std::optional<unsigned> offset,
                          ConnectionInterface::QueryParams *query_params) {
    std::string pq_ts_query = full_text_query;
    TokenizePlainTextQuery(&pq_ts_query);
    if (prefix_search && !pq_ts_query.empty()) {
        pq_ts_query += ":*";
    }

    // TODO: Make this full text argument parameterizable.
    //    ConnectionInterface::QueryParams::SlotId full_text_slot = query_params->AllocateSlot();
    //    query_params->SetParam(full_text_slot, std::make_shared<SqlStr>(pq_full_text_query));

    std::string select_query = "SELECT target_collection.* FROM (" + target_collection +
                               ") AS target_collection, TO_TSQUERY('" + pq_ts_query +
                               "') AS q WHERE target_collection.search_terms @@ q";
    if (rank_result) {
        select_query += " ORDER BY TS_RANK_CD(target_collection.search_terms, q) DESC";
    }

    if (limit.has_value()) {
        ConnectionInterface::QueryParams::SlotId limit_slot = query_params->AllocateSlot();
        query_params->SetParam(limit_slot, std::make_shared<SqlInt>(*limit));
        select_query += " LIMIT $" + std::to_string(limit_slot);
    }
    if (offset.has_value()) {
        ConnectionInterface::QueryParams::SlotId offset_slot = query_params->AllocateSlot();
        query_params->SetParam(offset_slot, std::make_shared<SqlInt>(*offset));
        select_query += " OFFSET $" + std::to_string(offset_slot);
    }

    return select_query;
}

} // namespace sql_runner_internal

uint64_t Update(SqlEntityInterface const &entity, std::string const &table_name, bool override,
                ConnectionReservoirInterface *reservoir) {
    InsertQueryAndParams query_and_params = GenerateInsertQuery(table_name, entity, override);

    ConnectionInterface *conn = reservoir->Take();
    uint64_t numRowsUpdated =
        conn->RunUpdate(query_and_params.query, query_and_params.query_params);
    reservoir->Put(conn);

    return numRowsUpdated;
}

uint64_t Delete(std::string const &table_name, SqlQueryBuilder const &query,
                ConnectionReservoirInterface *reservoir) {
    std::string completed_query = "DELETE FROM " + table_name + " " + query.PsqlQuery();

    ConnectionInterface *conn = reservoir->Take();
    uint64_t numRowsUpdated = conn->RunUpdate(completed_query, query.QueryParams());
    reservoir->Put(conn);

    return numRowsUpdated;
}

bool Exists(SqlQueryBuilder const &query, ConnectionReservoirInterface *reservoir) {
    std::string exists_query = "SELECT TRUE FROM " + query.PsqlQuery();

    ConnectionInterface *conn = reservoir->Take();

    std::unique_ptr<ResultSetInterface> rs = conn->RunQuery(exists_query, query.QueryParams());
    bool exists = rs->HasNext();

    reservoir->Put(conn);

    return exists;
}

std::unordered_set<std::string> Tables(ConnectionReservoirInterface *reservoir) {
    ConnectionInterface *conn = reservoir->Take();
    std::string reflection_query =
        "SELECT tb.table_name FROM information_schema.tables tb WHERE tb.table_schema='public'";
    std::unique_ptr<ResultSetInterface> rs =
        conn->RunQuery(reflection_query, ConnectionInterface::QueryParams());

    std::unordered_set<std::string> table_names;
    SqlStr table_name("table_name");
    for (; rs->HasNext(); rs->Next()) {
        rs->SetField(0, &table_name);
        assert(table_name.Value().has_value());
        table_names.insert(table_name.Value().value());
    }

    reservoir->Put(conn);

    return table_names;
}

bool SendHeartBeat(ConnectionReservoirInterface *reservoir) {
    ConnectionInterface *conn = reservoir->Take();

    std::unique_ptr<ResultSetInterface> rs =
        conn->RunQuery("SELECT 1", ConnectionInterface::QueryParams());

    if (!rs->HasNext()) {
        reservoir->Put(conn);
        return false;
    }

    SqlInt one("heart_beat");
    rs->SetField(0, &one);
    if (!one.Value().has_value() || one.Value().value() != 1) {
        reservoir->Put(conn);
        return false;
    }

    reservoir->Put(conn);

    return true;
}

bool SendHeartBeat(ConnectionInterface *conn) {
    std::unique_ptr<ResultSetInterface> rs =
        conn->RunQuery("SELECT 1", ConnectionInterface::QueryParams());

    if (!rs->HasNext()) {
        return false;
    }

    SqlInt one("heart_beat");
    rs->SetField(0, &one);
    if (!one.Value().has_value() || one.Value().value() != 1) {
        return false;
    }

    return true;
}

int64_t TimeId(unsigned host_id) {
    int64_t timestamp = TemporalId();
    int64_t unique_id = host_id * 0xFFFFFFFFFFFFF + timestamp;
    return sql_runner_internal::ReverseBytes(unique_id);
}

int64_t SeqId(std::string const &seq_table, ConnectionReservoirInterface *reservoir) {
    ConnectionInterface *conn = reservoir->Take();

    std::unique_ptr<ResultSetInterface> rs =
        conn->RunQuery("SELECT nextval('" + seq_table + "')", ConnectionInterface::QueryParams());
    assert(rs->HasNext());

    SqlLong id("id");
    rs->SetField(0, &id);
    assert(id.Value().has_value());

    reservoir->Put(conn);

    return id.Value().value();
}

void ClearAllTables(ConnectionReservoirInterface *reservoir) {
    std::unordered_set<std::string> table_names = Tables(reservoir);
    SqlQueryBuilder constraint;
    constraint.QueryPiece("CASCADE");
    for (std::string const &tb_name : table_names) {
        Delete(tb_name, constraint, reservoir);
    }
}

} // namespace e8
