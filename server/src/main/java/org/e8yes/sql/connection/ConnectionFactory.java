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

/** Simple factory class for creating different types of database connection. */
public class ConnectionFactory {

  public enum ConnectionType {
    JDBC,
    MOCK
  }

  private final ConnectionType connectionType;
  private final String hostName;
  private final int port;
  private final String dbName;
  private final String userName;
  private final String password;

  /**
   * Takes in parameters necessary for creating a new database connection.
   *
   * @param connectionType Type of connection to create.
   * @param hostName Host name of the database server.
   * @param port Port of the database server.
   * @param dbName Name of the database to connect to.
   * @param userName Name of the database user to act as.
   * @param password Password of the database user.
   */
  public ConnectionFactory(
      ConnectionType connectionType,
      String hostName,
      int port,
      String dbName,
      String userName,
      String password) {
    this.connectionType = connectionType;
    this.hostName = hostName;
    this.port = port;
    this.dbName = dbName;
    this.userName = userName;
    this.password = password;
  }

  /**
   * Create a new database connection.
   *
   * @return A new connection, not null.
   * @throws SQLException
   */
  public ConnectionInterface create() throws SQLException {
    switch (connectionType) {
      case JDBC:
        return new JdbcConnection(hostName, port, dbName, userName, password);
      case MOCK:
        return new MockConnection();
      default:
        throw new IllegalArgumentException("Unrecognized connectionType=" + connectionType);
    }
  }
}
