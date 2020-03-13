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
package org.e8yes.sql;

import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.List;
import java.util.Optional;
import org.e8yes.sql.connection.ConnectionInterface;
import org.e8yes.sql.connection.ConnectionReservoirInterface;
import org.e8yes.sql.orm.DataCollection;
import org.e8yes.sql.orm.QueryCompletion;
import org.e8yes.sql.resultset.ResultSetInterface;

/** Collects information from different sources to build then execute an SQL query. */
public class SqlRunner {

  private Class entityType;
  private Optional<SqlQueryBuilder> query = Optional.empty();
  private ConnectionReservoirInterface reservoir;

  /**
   * Connection reservoir to allocate database connections.
   *
   * @param reservoir
   * @return
   */
  public SqlRunner withConnectionReservoir(ConnectionReservoirInterface reservoir) {
    this.reservoir = reservoir;
    return this;
  }

  /**
   * Example: query = "AUser user JOIN CreditCard card ON card.userId = user.id WHERE user.joinDate
   * > '2020-1-1'"
   *
   * @param query
   * @param resultType
   * @return
   */
  public SqlRunner withSelect(SqlQueryBuilder query, Class resultType) {
    this.query = Optional.of(query);
    this.entityType = resultType;
    return this;
  }

  /**
   * Constructs and runs query with the information provided and synchronously returns the result.
   *
   * @param <ReturnType>
   * @return The query result.
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   * @throws SQLException
   */
  public <ReturnType> List<ReturnType> runQuery()
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException {
    if (entityType == null) {
      throw new IllegalArgumentException("Result type not specified.");
    }
    if (query.isEmpty()) {
      throw new IllegalArgumentException("Query not specified.");
    }
    if (reservoir == null) {
      throw new IllegalArgumentException("Connection reservoir not specified.");
    }

    String completedQuery =
        QueryCompletion.completeSelectQuery(query.get().jdbcQuery(), entityType);
    ConnectionInterface conn = reservoir.take();
    ResultSetInterface rs = conn.runQuery(completedQuery, query.get().queryParams());
    List<ReturnType> results = DataCollection.collect(rs, entityType);
    reservoir.put(conn);
    return results;
  }
}
