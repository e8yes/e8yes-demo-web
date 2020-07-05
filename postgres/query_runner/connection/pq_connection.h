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

#ifndef PQ_CONNECTION_H
#define PQ_CONNECTION_H

#include <memory>
#include <pqxx/pqxx>
#include <stdint.h>
#include <string>

#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/resultset/result_set_interface.h"

namespace e8 {

/**
 * @brief The PqConnection class PQ client implementation of the connection interface.
 * This implementation is not thread safe.
 */
class PqConnection : public ConnectionInterface {
  public:
    PqConnection(std::string const &host_name, int port, std::string const &db_name,
                 std::string const &user_name, std::string const &password);
    ~PqConnection() override;
    PqConnection(PqConnection const &) = delete;

    std::unique_ptr<ResultSetInterface> RunQuery(ParameterizedQuery const &query,
                                                  QueryParams const &params) override;
    uint64_t RunUpdate(ParameterizedQuery const &query, QueryParams const &params) override;
    bool IsClosed() const override;

  private:
    class PqConnectionImpl;
    std::unique_ptr<PqConnectionImpl> impl_;
};

} // namespace e8

#endif // PQ_CONNECTION_H
