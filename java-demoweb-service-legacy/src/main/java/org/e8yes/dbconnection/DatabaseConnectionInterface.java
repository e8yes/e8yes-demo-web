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
import org.e8yes.sql.connection.ConnectionReservoirInterface;

/** Represents a connection store to a database server. */
public interface DatabaseConnectionInterface {

  /**
   * Connection reservoir to the database server.
   *
   * @return The connection reservoir.
   */
  public ConnectionReservoirInterface connectionReservoir();

  /**
   * Test the database connection.
   *
   * @throws SQLException
   */
  public void testConnection() throws SQLException;

  /**
   * Delete all rows from all the table in the database.
   *
   * @throws SQLException
   */
  public void clearAllTables() throws SQLException;

  /**
   * Clean up connections.
   *
   * @throws SQLException
   */
  public void cleanUp() throws SQLException;
}
