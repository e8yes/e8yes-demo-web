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

/**
 * A basic connection reservoir that provides no pooling. All Connections will be freshly created
 * and immediately closed
 */
public class BasicConnectionReservoir implements ConnectionReservoirInterface {

  private final ConnectionFactory factory;

  public BasicConnectionReservoir(ConnectionFactory factory) {
    this.factory = factory;
  }

  @Override
  public ConnectionInterface take() throws SQLException {
    return factory.create();
  }

  @Override
  public void put(ConnectionInterface conn) throws SQLException {
    conn.close();
  }

  @Override
  public void closeAll() throws SQLException {}
}
