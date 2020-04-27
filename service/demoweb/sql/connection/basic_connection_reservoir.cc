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

#include "sql/connection/basic_connection_reservoir.h"
#include "sql/connection/connection_factory.h"
#include "sql/connection/connection_interface.h"

namespace e8 {

BasicConnectionReservoir::BasicConnectionReservoir(ConnectionFactory const &fact) : fact_(fact) {}

ConnectionInterface *BasicConnectionReservoir::Take() {
    std::unique_ptr<ConnectionInterface> conn = fact_.Create();

    ConnectionInterface *ptr = conn.get();
    conns_.insert(std::make_pair(ptr, std::move(conn)));

    return ptr;
}

void BasicConnectionReservoir::Put(ConnectionInterface *conn) { conns_.erase(conn); }

void BasicConnectionReservoir::CloseAll() { conns_.clear(); }

} // namespace e8
