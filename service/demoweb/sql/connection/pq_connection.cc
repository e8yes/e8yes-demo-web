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

#include <cassert>
#include <memory>
#include <pqxx/pqxx>

#include "sql/connection/pq_connection.h"
#include "sql/resultset/pq_result_set.h"
#include "sql/resultset/result_set_interface.h"
#include "util/lru_hash_map.h"

namespace e8 {
namespace {

static unsigned const kStatementCacheLimit = 1000;

using StatementId = uint32_t;

class OnFetch {
  public:
    OnFetch(pqxx::connection *conn) : conn_(conn) {}

    StatementId operator()(ConnectionInterface::ParameterizedQuery const &query) {
        StatementId statement = this->allocate_statement();
        conn_->prepare(std::to_string(statement), query);
        return statement;
    }

  private:
    StatementId allocate_statement() { return next_statement_id_++; }

    pqxx::connection *const conn_;
    StatementId next_statement_id_ = 0;
};

class OnEvict {
  public:
    OnEvict(pqxx::connection *conn) : conn_(conn) {}

    void operator()(StatementId statement_id) { conn_->unprepare(std::to_string(statement_id)); }

  private:
    pqxx::connection *const conn_;
};

} // namespace

class PqConnection::PqConnectionImpl {
  public:
    PqConnectionImpl(std::unique_ptr<pqxx::connection> conn)
        : conn(std::move(conn)),
          statement_cache(kStatementCacheLimit, OnFetch(conn.get()), OnEvict(conn.get())) {}

    std::unique_ptr<pqxx::connection> const conn;
    LruHashMap<ParameterizedQuery, StatementId, OnFetch, OnEvict> statement_cache;
};

PqConnection::PqConnection(std::string const &host_name, int port, std::string const &db_name,
                           std::string const &user_name, std::string const &password)
    : impl_(std::make_unique<PqConnectionImpl>(/*conn=*/std::make_unique<pqxx::connection>(
          "hostaddr=" + host_name + " port=" + std::to_string(port) + " dbname=" + db_name +
          " user=" + user_name + " password=" + password))) {}

PqConnection::~PqConnection() {}

std::unique_ptr<ResultSetInterface> PqConnection::run_query(ParameterizedQuery const &query,
                                                            QueryParams const &params) {
    StatementId id = impl_->statement_cache.Fetch(query);

    pqxx::work query_work(*impl_->conn);
    pqxx::prepare::invocation invocation = query_work.prepared(std::to_string(id));
    for (auto const &[slot_id, param] : params.parameters()) {
        param->export_to_invocation(&invocation);
    }

    auto rs = std::make_unique<PqResultSet>(invocation.exec());
    query_work.commit();

    return rs;
}

uint64_t PqConnection::run_update(ParameterizedQuery const &query, QueryParams const &params) {
    StatementId id = impl_->statement_cache.Fetch(query);

    pqxx::work update_work(*impl_->conn);
    pqxx::prepare::invocation invocation = update_work.prepared(std::to_string(id));
    for (auto const &[slot_id, param] : params.parameters()) {
        param->export_to_invocation(&invocation);
    }

    pqxx::result rs = invocation.exec();
    update_work.commit();

    assert(rs.size() == 1);
    uint64_t num_rows_updated = rs.begin()->at(0).as<uint64_t>();
    return num_rows_updated;
}

bool PqConnection::is_closed() const { return !impl_->conn->is_open(); }

} // namespace e8
