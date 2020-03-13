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
import org.e8yes.sql.primitive.SqlLong;
import org.e8yes.sql.primitive.SqlStr;
import org.e8yes.sql.resultset.ResultSetInterface;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class JdbcConnectionTest {

  @Test
  public void RunUpdateAndQuery() throws SQLException {
    JdbcConnection conn =
        new JdbcConnection(
            /*hostName=*/ "localHost",
            /*port=*/ 5432,
            /*dbName=*/ "demowebtest",
            /*userName=*/ "postgres",
            /*password=*/ "password");

    // Prepares table.
    String dropTableStmt = "DROP TABLE IF EXISTS JdbcConnectionTest";
    conn.runUpdate(dropTableStmt, new ConnectionInterface.QueryParams());

    String createTableStmt =
        "CREATE TABLE JdbcConnectionTest("
            + "id BIGINT NOT NULL,"
            + "testName CHARACTER VARYING NOT NULL,"
            + "PRIMARY KEY (id))";
    conn.runUpdate(createTableStmt, new ConnectionInterface.QueryParams());

    // Insert test records.
    String insertStmt = "INSERT INTO JdbcConnectionTest(id, testName) " + "VALUES (?, ?)";
    ConnectionInterface.QueryParams insertionParams = new ConnectionInterface.QueryParams();
    insertionParams.setParam(1, new SqlLong(1L));
    insertionParams.setParam(2, new SqlStr("test_string"));
    int numRows = conn.runUpdate(insertStmt, insertionParams);

    Assertions.assertEquals(1, numRows);

    // Query records.
    String queryStmt = "SELECT * FROM JdbcConnectionTest WHERE id=?";
    ConnectionInterface.QueryParams queryParams = new ConnectionInterface.QueryParams();
    queryParams.setParam(1, new SqlLong(1L));
    ResultSetInterface rs = conn.runQuery(queryStmt, queryParams);

    Assertions.assertTrue(rs.hasNext());

    SqlLong idField = new SqlLong();
    SqlStr testNameField = new SqlStr();
    rs.setCellValueToField(1, idField);
    rs.setCellValueToField(2, testNameField);

    Assertions.assertEquals((Long) 1L, idField.value());
    Assertions.assertEquals("test_string", testNameField.value());

    rs.next();
    Assertions.assertFalse(rs.hasNext());

    // Clean up.
    conn.runUpdate(dropTableStmt, new ConnectionInterface.QueryParams());
  }
}
