/**
 * e8yes demo web server.
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

#ifndef CONNECTION_RESERVOIR_INTERFACE_H
#define CONNECTION_RESERVOIR_INTERFACE_H

#include "sql/connection/connection_interface.h"

namespace e8 {

/**
 * @brief The ConnectionReservoirInterface class Represents a reservoir of database connections
 * available.
 */
class ConnectionReservoirInterface {
  public:
    ConnectionReservoirInterface() = default;
    ConnectionReservoirInterface(ConnectionReservoirInterface const &) = delete;
    virtual ~ConnectionReservoirInterface() = default;

    /**
     * @brief Retrieve a connection from the reservoir.
     * @return A database connection.
     */
    virtual ConnectionInterface *Take() = 0;

    /**
     * @brief Return a connection back to the reservoir.
     * @param conn The database connection to put back.
     */
    virtual void Put(ConnectionInterface *conn) = 0;

    /**
     * @brief Close all the connections.
     */
    virtual void CloseAll() = 0;
};

} // namespace e8

#endif // CONNECTION_RESERVOIR_INTERFACE_H
