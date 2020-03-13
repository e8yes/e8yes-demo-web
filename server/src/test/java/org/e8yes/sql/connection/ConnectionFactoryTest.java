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
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class ConnectionFactoryTest {

  @Test
  public void testCreateMockConnection() throws SQLException {
    ConnectionFactory factory =
        new ConnectionFactory(
            ConnectionFactory.ConnectionType.MOCK,
            /*hostName=*/ null,
            /*port=*/ -1,
            /*dbName=*/ null,
            /*userName=*/ null,
            /*password=*/ null);
    ConnectionInterface conn = factory.create();
    Assertions.assertTrue(conn instanceof MockConnection);
  }

  @Test
  public void testCreateJdbcConnection() throws SQLException {
    ConnectionFactory factory =
        new ConnectionFactory(
            ConnectionFactory.ConnectionType.JDBC,
            /*hostName=*/ "localhost",
            /*port=*/ 5432,
            /*dbName=*/ "demowebtest",
            /*userName=*/ "postgres",
            /*password=*/ "password");
    ConnectionInterface conn = factory.create();
    Assertions.assertTrue(conn instanceof JdbcConnection);
    Assertions.assertFalse(conn.isClosed());
    conn.close();
  }
}
