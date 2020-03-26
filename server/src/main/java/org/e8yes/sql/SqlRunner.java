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
import java.time.Instant;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;
import org.e8yes.sql.connection.ConnectionInterface;
import org.e8yes.sql.connection.ConnectionReservoirInterface;
import org.e8yes.sql.orm.DataCollection;
import org.e8yes.sql.orm.QueryCompletion;
import org.e8yes.sql.primitive.SqlInt;
import org.e8yes.sql.primitive.SqlLong;
import org.e8yes.sql.primitive.SqlStr;
import org.e8yes.sql.resultset.ResultSetInterface;

/** Collects information from different sources to build then execute an SQL query. */
public class SqlRunner {

  /**
   * Constructs and runs query with partial information provided and synchronously returns the
   * result.
   *
   * @param <ReturnType>
   * @param query Partial query. Example: "AUser user JOIN CreditCard card ON card.userId = user.id
   *     WHERE user.joinDate > '2020-1-1'"
   * @param entityType The type of Java object to run ORM on. It will convert query result to this
   *     target entity type
   * @param reservoir Connection reservoir to allocate database connections.
   * @return The query result.
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   * @throws SQLException
   */
  public static <ReturnType> List<ReturnType> runQuery(
      SqlQueryBuilder query, Class entityType, ConnectionReservoirInterface reservoir)
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException {
    String completedQuery = QueryCompletion.completeSelectQuery(query.jdbcQuery(), entityType);
    ConnectionInterface conn = reservoir.take();
    ResultSetInterface rs = conn.runQuery(completedQuery, query.queryParams());
    List<ReturnType> results = DataCollection.collect(rs, entityType);
    reservoir.put(conn);
    return results;
  }

  /**
   * Constructs and runs query then checks if the query returns at least 1 result.
   *
   * @param query Partial query to test against. Example: "AUser user JOIN CreditCard card ON
   *     card.userId = user.id WHERE user.joinDate > '2020-1-1'"
   * @param reservoir Connection reservoir to allocate database connections.
   * @return true if at least 1 entry returns from the query, otherwise, false.
   * @throws SQLException
   */
  public static boolean runExists(SqlQueryBuilder query, ConnectionReservoirInterface reservoir)
      throws SQLException {
    String completedQuery = "SELECT TRUE FROM " + query.jdbcQuery();
    ConnectionInterface conn = reservoir.take();
    ResultSetInterface rs = conn.runQuery(completedQuery, query.queryParams());
    boolean exists = rs.hasNext();
    reservoir.put(conn);

    return exists;
  }

  /**
   * Runs a deletion SQL query on the specified table.
   *
   * @param <ReturnType>
   * @param tableName Name of the table to run deletion query on.
   * @param query Partial deletion query. Example: "WHERE id = 100"
   * @param reservoir Connection reservoir to allocate database connections.
   * @return The number of rows deleted.
   * @throws SQLException
   */
  public static <ReturnType> int runDelete(
      String tableName, SqlQueryBuilder query, ConnectionReservoirInterface reservoir)
      throws SQLException {
    String completedQuery = "DELETE FROM " + tableName + " " + query.jdbcQuery();
    ConnectionInterface conn = reservoir.take();
    int numRowsUpdated = conn.runUpdate(completedQuery, query.queryParams());
    reservoir.put(conn);

    return numRowsUpdated;
  }

  /**
   * Save a Java object to the specified SQL table.
   *
   * @param <Type>
   * @param record Java object to be saved.
   * @param tableName Target SQL table to save to.
   * @param entityType The type of Java object to run ORM on. It will convert the entity type to the
   *     SQL row format.
   * @param override Whether or not to override existing record when conflict occurs on update.
   * @param reservoir Connection reservoir to allocate database connections.
   * @return The number of SQL rows affected by this update.
   * @throws SQLException
   * @throws IllegalAccessException
   */
  public static <Type> int runUpdate(
      Type record,
      String tableName,
      Class entityType,
      boolean override,
      ConnectionReservoirInterface reservoir)
      throws SQLException, IllegalAccessException {
    String completedQuery = QueryCompletion.completeInsertQuery(tableName, entityType, override);
    ConnectionInterface.QueryParams params =
        QueryCompletion.generateInsertQueryParams(record, entityType, override);
    ConnectionInterface conn = reservoir.take();
    int numRowsUpdated = conn.runUpdate(completedQuery, params);
    reservoir.put(conn);

    return numRowsUpdated;
  }

  /**
   * Get all the database tables.
   *
   * @param reservoir Connection reservoir to allocate database connections.
   * @return table name of all the database tables.
   * @throws java.sql.SQLException
   */
  public static Set<String> tables(ConnectionReservoirInterface reservoir) throws SQLException {
    ConnectionInterface conn = reservoir.take();
    String reflectionQuery =
        "SELECT tb.table_name FROM information_schema.tables tb "
            + "WHERE tb.table_schema='public'";
    ResultSetInterface rs = conn.runQuery(reflectionQuery, new ConnectionInterface.QueryParams());

    Set<String> tableNames = new TreeSet();
    SqlStr tableName = new SqlStr();
    for (; rs.hasNext(); rs.next()) {
      rs.setCellValueToField(1, tableName);
      assert (tableName.value() != null);
      tableNames.add(tableName.value());
    }

    reservoir.put(conn);

    return tableNames;
  }

  /**
   * Send a heartbeat to test the connection.
   *
   * @param reservoir Connection reservoir to allocate database connections.
   * @throws SQLException
   */
  public static void sendHeartBeat(ConnectionReservoirInterface reservoir) throws SQLException {
    ConnectionInterface conn = reservoir.take();
    ResultSetInterface rs = conn.runQuery("SELECT 1", new ConnectionInterface.QueryParams());

    SqlInt one = new SqlInt();
    rs.setCellValueToField(1, one);
    assert (one.value() != null);
    assert (one.value() == 1);

    reservoir.put(conn);
  }

  /**
   * Generate approximately unique ID from time.
   *
   * @return unique ID.
   */
  public static long timeId() {
    Instant inst = Instant.now();
    long micros = inst.getEpochSecond() * 1000 * 1000 + inst.getNano() / 1000;
    return Long.reverse(micros);
  }

  /**
   * Generate sequential ID from a sequence table.
   *
   * @param seqTable Name of the sequence table to generate ID from.
   * @param reservoir Connection reservoir to allocate database connections.
   * @return unique ID.
   * @throws SQLException
   */
  public static long seqId(String seqTable, ConnectionReservoirInterface reservoir)
      throws SQLException {
    ConnectionInterface conn = reservoir.take();

    ResultSetInterface rs =
        conn.runQuery("SELECT nextval('" + seqTable + "')", new ConnectionInterface.QueryParams());
    assert (rs.hasNext());
    SqlLong id = new SqlLong();
    rs.setCellValueToField(0, id);
    assert (id.value() != null);

    reservoir.put(conn);

    return id.value();
  }
}
