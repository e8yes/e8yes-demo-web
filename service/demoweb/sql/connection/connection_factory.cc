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

#include "sql/connection/connection_factory.h"
#include "sql/connection/connection_interface.h"
#include "sql/connection/mock_connection.h"
#include "sql/connection/pq_connection.h"

namespace e8 {

ConnectionFactory::ConnectionFactory(ConnectionType connection_type, std::string const &host_name,
                                     int port, std::string const &db_name,
                                     std::string const &user_name, std::string const &password)
    : connection_type_(connection_type), host_name_(host_name), db_name_(db_name),
      user_name_(user_name), password_(password), port_(port) {}

std::unique_ptr<ConnectionInterface> ConnectionFactory::Create() const {
    switch (connection_type_) {
    case PQ:
        return std::make_unique<PqConnection>(host_name_, port_, db_name_, user_name_, password_);
    case MOCK:
        return std::make_unique<MockConnection>();
    }
    assert(false);
}

} // namespace e8
