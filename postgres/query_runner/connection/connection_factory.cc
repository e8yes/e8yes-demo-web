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

#include <cassert>
#include <memory>

#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/connection/mock_connection.h"
#include "postgres/query_runner/connection/pq_connection.h"

namespace e8 {

ConnectionFactory::ConnectionFactory(ConnectionType connection_type, std::string const &host_name,
                                     std::string const &db_name)
    : connection_type_(connection_type), host_name_(host_name), db_name_(db_name) {}

std::unique_ptr<ConnectionInterface> ConnectionFactory::Create() const {
    switch (connection_type_) {
    case PQ:
        return std::make_unique<PqConnection>(host_name_, db_name_);
    case MOCK:
        return std::make_unique<MockConnection>();
    }
    assert(false);
}

} // namespace e8
