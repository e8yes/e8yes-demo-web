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

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import org.e8yes.sql.primitive.SqlPrimitiveInterface;
import org.e8yes.sql.resultset.JdbcResultSet;
import org.e8yes.sql.resultset.ResultSetInterface;

/** */
public class JdbcConnection implements ConnectionInterface {

  private final Connection conn;
  private final Map<String, PreparedStatement> statementCache = new HashMap();

  public JdbcConnection(String hostName, int port, String dbName, String userName, String password)
      throws SQLException {
    String connectionStr =
        "jdbc:postgresql://" + hostName + ":" + port + "/" + dbName + "?useSSL=false";
    conn = DriverManager.getConnection(connectionStr, userName, password);
  }

  private PreparedStatement createStmt(String query) throws SQLException {
    PreparedStatement stmt = statementCache.get(query);
    if (stmt == null) {
      stmt = conn.prepareStatement(query);
      statementCache.put(query, stmt);
    }
    return stmt;
  }

  @Override
  public ResultSetInterface runQuery(String query, QueryParams params) throws SQLException {
    PreparedStatement stmt = createStmt(query);

    // Set values to placeholders.
    for (Map.Entry<Integer, SqlPrimitiveInterface> positionedParam : params.params.entrySet()) {
      int position = positionedParam.getKey();
      positionedParam.getValue().exportValueToStatement(stmt, position, conn);
    }

    // Run JDBC query.
    ResultSet rs = stmt.executeQuery();
    return new JdbcResultSet(rs);
  }

  @Override
  public int runUpdate(String query, QueryParams params) throws SQLException {
    PreparedStatement stmt = createStmt(query);

    // Set values to placeholders.
    for (Map.Entry<Integer, SqlPrimitiveInterface> positionedParam : params.params.entrySet()) {
      int position = positionedParam.getKey();
      positionedParam.getValue().exportValueToStatement(stmt, position, conn);
    }

    // Run JDBC update.
    return stmt.executeUpdate();
  }

  @Override
  public void close() throws SQLException {
    conn.close();
  }

  @Override
  public boolean isClosed() throws SQLException {
    return conn.isClosed();
  }
}
