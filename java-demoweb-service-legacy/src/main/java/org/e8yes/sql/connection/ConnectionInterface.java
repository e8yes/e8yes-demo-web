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
import java.util.HashMap;
import java.util.Map;
import org.e8yes.sql.primitive.SqlPrimitiveInterface;
import org.e8yes.sql.resultset.ResultSetInterface;

/** Represents a connection to a database server */
public interface ConnectionInterface {

  /** Stores information about the parameter values to a parameterized query */
  public static class QueryParams {
    Map<Integer, SqlPrimitiveInterface> params = new HashMap();

    /** Clear all the parameter values. */
    public void clear() {
      params.clear();
    }

    /**
     * Set value to the position-th parameter placeholder.
     *
     * @param position Position to set value to.
     * @param val value to set.
     */
    public void setParam(int position, SqlPrimitiveInterface val) {
      params.put(position, val);
    }

    /**
     * Get the value set to the position-th placeholder.
     *
     * @param position Position to read from.
     * @return The value if it exists. Otherwise null.
     */
    public SqlPrimitiveInterface getParam(int position) {
      return params.get(position);
    }

    /**
     * Get the number of placeholders that are assigned a value of.
     *
     * @return The number of placeholders.
     */
    public int numPositions() {
      return params.size();
    }
  }

  /**
   * Run a parameterized query.
   *
   * @param query Query to run.
   * @param params Parameters for the query.
   * @return Query's result set.
   * @throws java.sql.SQLException
   */
  ResultSetInterface runQuery(String query, QueryParams params) throws SQLException;

  /**
   * Run a parameterized update query.
   *
   * @param query Update query to run.
   * @param params Parameters for the query.
   * @return The number of rows updated by the query.
   * @throws java.sql.SQLException
   */
  int runUpdate(String query, QueryParams params) throws SQLException;

  /**
   * Closing the connection.
   *
   * @throws java.sql.SQLException
   */
  void close() throws SQLException;

  /**
   * Check if the connection is closed
   *
   * @throws java.sql.SQLException
   * @return true if it is closed.
   */
  boolean isClosed() throws SQLException;
}
