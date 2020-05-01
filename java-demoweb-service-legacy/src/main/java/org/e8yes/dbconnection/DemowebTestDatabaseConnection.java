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
package org.e8yes.dbconnection;

import java.sql.SQLException;
import java.util.Set;
import org.e8yes.sql.SqlQueryBuilder;
import org.e8yes.sql.SqlRunner;
import org.e8yes.sql.connection.BasicConnectionReservoir;
import org.e8yes.sql.connection.ConnectionFactory;
import org.e8yes.sql.connection.ConnectionReservoirInterface;

/** Store connections to the test DemoWeb database. */
public class DemowebTestDatabaseConnection implements DatabaseConnectionInterface {

  private static final String HOST_NAME = "localhost";
  private static final int HOST_PORT = 5432;
  private static final String DB_NAME = "demowebtest";
  private static final String DB_USER = "postgres";
  private static final String DB_PASS = "password";

  private final ConnectionReservoirInterface demowebDbConnections;

  public DemowebTestDatabaseConnection() {
    ConnectionFactory factory =
        new ConnectionFactory(
            ConnectionFactory.ConnectionType.JDBC, HOST_NAME, HOST_PORT, DB_NAME, DB_USER, DB_PASS);
    demowebDbConnections = new BasicConnectionReservoir(factory);
  }

  @Override
  public ConnectionReservoirInterface connectionReservoir() {
    return demowebDbConnections;
  }

  @Override
  public void testConnection() throws SQLException {
    SqlRunner.sendHeartBeat(connectionReservoir());
  }

  @Override
  public void clearAllTables() throws SQLException {
    Set<String> tableNames = SqlRunner.tables(connectionReservoir());
    SqlQueryBuilder constraint = new SqlQueryBuilder().queryPiece("CASCADE");
    for (String tbName : tableNames) {
      SqlRunner.runDelete(tbName, constraint, connectionReservoir());
    }
  }

  @Override
  public void cleanUp() throws SQLException {
    demowebDbConnections.closeAll();
  }
}
