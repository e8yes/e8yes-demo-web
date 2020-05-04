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

#ifndef CONNECTION_FACTORY_H
#define CONNECTION_FACTORY_H

#include <memory>
#include <string>

#include "postgres/query_runner/connection/connection_interface.h"

namespace e8 {

/**
 * @brief The ConnectionFactory class Simple factory class for creating different types of database
 * connection.
 */
class ConnectionFactory {
  public:
    enum ConnectionType { PQ, MOCK };

    ConnectionFactory(ConnectionType connection_type, std::string const &host_name, int port,
                      std::string const &db_name, std::string const &user_name,
                      std::string const &password);
    ConnectionFactory(ConnectionFactory const &) = default;
    ~ConnectionFactory() = default;

    std::unique_ptr<ConnectionInterface> Create() const;

  private:
    ConnectionType connection_type_;
    std::string host_name_;
    std::string db_name_;
    std::string user_name_;
    std::string password_;
    int port_;
};

} // namespace e8

#endif // CONNECTION_FACTORY_H
