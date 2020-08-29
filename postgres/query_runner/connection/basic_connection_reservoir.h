/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2020 Yichen Ma {yichenm2@uci.edu}, Chifeng Wen {daviesx66@gmail.com}
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

#ifndef BASIC_CONNECTION_RESERVOIR_H
#define BASIC_CONNECTION_RESERVOIR_H

#include <mutex>
#include <unordered_map>

#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

/**
 * @brief The BasicConnectionReservoir class A basic connection reservoir that provides no pooling.
 * All Connections will be freshly created and immediately closed. This class is thread-safe.
 */
class BasicConnectionReservoir : public ConnectionReservoirInterface {
  public:
    BasicConnectionReservoir(ConnectionFactory const &fact);
    ~BasicConnectionReservoir() override = default;

    ConnectionInterface *Take() override;
    void Put(ConnectionInterface *conn) override;
    void CloseAll() override;

  private:
    std::unordered_map<ConnectionInterface *, std::unique_ptr<ConnectionInterface>> conns_;
    std::mutex mutex_;
    ConnectionFactory fact_;
};

} // namespace e8

#endif // BASIC_CONNECTION_RESERVOIR_H
