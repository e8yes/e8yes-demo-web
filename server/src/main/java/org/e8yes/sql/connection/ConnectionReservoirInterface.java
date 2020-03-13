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
package org.e8yes.sql.connection;

import java.sql.SQLException;

/** Represents a reservoir of database connections available */
public interface ConnectionReservoirInterface {

  /**
   * Retrieve a connection from the reservoir.
   *
   * @return A database connection.
   * @throws SQLException
   */
  public ConnectionInterface take() throws SQLException;

  /**
   * Return a connection back to the reservoir.
   *
   * @param conn The database connection to put back.
   * @throws SQLException
   */
  public void put(ConnectionInterface conn) throws SQLException;
}
