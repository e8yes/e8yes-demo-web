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
package org.e8yes.sql.resultset;

import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import org.e8yes.environment.DatabaseConnection;
import org.e8yes.environment.EnvironmentContext;
import org.e8yes.sql.connection.ConnectionInterface;
import org.e8yes.sql.connection.JdbcConnection;
import org.e8yes.sql.primitive.SqlBool;
import org.e8yes.sql.primitive.SqlBoolArr;
import org.e8yes.sql.primitive.SqlDate;
import org.e8yes.sql.primitive.SqlDateArr;
import org.e8yes.sql.primitive.SqlDouble;
import org.e8yes.sql.primitive.SqlDoubleArr;
import org.e8yes.sql.primitive.SqlFloat;
import org.e8yes.sql.primitive.SqlFloatArr;
import org.e8yes.sql.primitive.SqlInt;
import org.e8yes.sql.primitive.SqlIntArr;
import org.e8yes.sql.primitive.SqlLong;
import org.e8yes.sql.primitive.SqlLongArr;
import org.e8yes.sql.primitive.SqlStr;
import org.e8yes.sql.primitive.SqlStrArr;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class JdbcResultSetTest {
  @BeforeEach
  public void setUp() throws SQLException {
    DatabaseConnection.init(new EnvironmentContext(EnvironmentContext.Mode.Test));
  }

  @AfterEach
  public void tearDown() throws SQLException {
    DatabaseConnection.deleteAllData();
  }

  private static class Record {
    public SqlInt intField = new SqlInt();
    public SqlLong longField = new SqlLong();
    public SqlBool booleanField = new SqlBool();
    public SqlFloat floatField = new SqlFloat();
    public SqlDouble doubleField = new SqlDouble();
    public SqlDate dateField = new SqlDate();
    public SqlStr stringField = new SqlStr();

    public SqlIntArr intArrayField = new SqlIntArr();
    public SqlLongArr longArrayField = new SqlLongArr();
    public SqlBoolArr booleanArrayField = new SqlBoolArr();
    public SqlFloatArr floatArrayField = new SqlFloatArr();
    public SqlDoubleArr doubleArrayField = new SqlDoubleArr();
    public SqlDateArr dateArrayField = new SqlDateArr();
    public SqlStrArr stringArrayField = new SqlStrArr();
  }

  @Test
  public void retrieveOneRecordTest()
      throws SQLException, IllegalArgumentException, IllegalAccessException, NoSuchMethodException,
          InvocationTargetException {
    JdbcConnection conn =
        new JdbcConnection(
            /*hostName=*/ "localHost",
            /*port=*/ 5432,
            /*dbName=*/ "demowebtest",
            /*userName=*/ "postgres",
            /*password=*/ "password");

    String dropTableStmt = "DROP TABLE IF EXISTS JdbcResultSetTestTable;";
    String createTableStmt =
        "CREATE TABLE IF NOT EXISTS JdbcResultSetTestTable ("
            + " intField INT, "
            + " longField BIGINT, "
            + " booleanField BOOLEAN, "
            + " floatField REAL, "
            + " doubleField DOUBLE PRECISION, "
            + " dateField TIMESTAMP WITHOUT TIME ZONE, "
            + " stringField CHARACTER VARYING, "
            + " intArrayField INT[], "
            + " longArrayField BIGINT[], "
            + " booleanArrayField BOOLEAN[], "
            + " floatArrayField REAL[], "
            + " doubleArrayField DOUBLE PRECISION[], "
            + " dateArrayField TIMESTAMP WITHOUT TIME ZONE[], "
            + " stringArrayField CHARACTER VARYING[] "
            + ");";
    conn.runUpdate(dropTableStmt, new ConnectionInterface.QueryParams());
    conn.runUpdate(createTableStmt, new ConnectionInterface.QueryParams());

    String insertRecordStmt =
        "INSERT INTO JdbcResultSetTestTable ("
            + " intField, "
            + " longField, "
            + " booleanField, "
            + " floatField, "
            + " doubleField, "
            + " dateField,"
            + " stringField, "
            + " intArrayField, "
            + " longArrayField, "
            + " booleanArrayField, "
            + " floatArrayField, "
            + " doubleArrayField, "
            + " dateArrayField, "
            + " stringArrayField) "
            + "VALUES ("
            + " 10, "
            + " 100, "
            + " TRUE, "
            + " 0.5, "
            + " 0.25, "
            + " to_timestamp(CAST(111 AS BIGINT)), "
            + " 'string_value', "
            + " ARRAY[10, 11], "
            + " ARRAY[100, 101], "
            + " ARRAY[TRUE, FALSE], "
            + " ARRAY[0.5, 1.5], "
            + " ARRAY[0.25, 1.25], "
            + " ARRAY[to_timestamp(CAST(111 AS BIGINT)), to_timestamp(CAST(112 AS BIGINT))], "
            + " ARRAY['string_value0', 'string_value1'] "
            + ");";
    conn.runUpdate(insertRecordStmt, new ConnectionInterface.QueryParams());

    String queryStmt = "SELECT * FROM JdbcResultSetTestTable;";

    ResultSetInterface jdbcRs = conn.runQuery(queryStmt, new ConnectionInterface.QueryParams());
    Assertions.assertTrue(jdbcRs instanceof JdbcResultSet);
    Assertions.assertTrue(jdbcRs.hasNext());

    Record record = new Record();
    jdbcRs.setCellValueToField(1, record.intField);
    Assertions.assertEquals((Integer) 10, record.intField.value());

    jdbcRs.setCellValueToField(2, record.longField);
    Assertions.assertEquals((Long) 100L, record.longField.value());

    jdbcRs.setCellValueToField(3, record.booleanField);
    Assertions.assertEquals((Boolean) true, record.booleanField.value());

    jdbcRs.setCellValueToField(4, record.floatField);
    Assertions.assertEquals((Float) 0.5f, record.floatField.value());

    jdbcRs.setCellValueToField(5, record.doubleField);
    Assertions.assertEquals((Double) 0.25, record.doubleField.value());

    jdbcRs.setCellValueToField(6, record.dateField);
    Assertions.assertEquals((Long) 111000L, (Long) record.dateField.value().getTime());

    jdbcRs.setCellValueToField(7, record.stringField);
    Assertions.assertEquals("string_value", record.stringField.value());

    jdbcRs.setCellValueToField(8, record.intArrayField);
    Assertions.assertEquals((Integer) 10, record.intArrayField.value()[0]);
    Assertions.assertEquals((Integer) 11, record.intArrayField.value()[1]);

    jdbcRs.setCellValueToField(9, record.longArrayField);
    Assertions.assertEquals((Long) 100L, record.longArrayField.value()[0]);
    Assertions.assertEquals((Long) 101L, record.longArrayField.value()[1]);

    jdbcRs.setCellValueToField(10, record.booleanArrayField);
    Assertions.assertEquals((Boolean) true, record.booleanArrayField.value()[0]);
    Assertions.assertEquals((Boolean) false, record.booleanArrayField.value()[1]);

    jdbcRs.setCellValueToField(11, record.floatArrayField);
    Assertions.assertEquals((Float) 0.5f, record.floatArrayField.value()[0]);
    Assertions.assertEquals((Float) 1.5f, record.floatArrayField.value()[1]);

    jdbcRs.setCellValueToField(12, record.doubleArrayField);
    Assertions.assertEquals((Double) 0.25, record.doubleArrayField.value()[0]);
    Assertions.assertEquals((Double) 1.25, record.doubleArrayField.value()[1]);

    jdbcRs.setCellValueToField(13, record.dateArrayField);
    Assertions.assertEquals((Long) 111000L, (Long) record.dateArrayField.value()[0].getTime());
    Assertions.assertEquals((Long) 112000L, (Long) record.dateArrayField.value()[1].getTime());

    jdbcRs.setCellValueToField(14, record.stringArrayField);
    Assertions.assertEquals("string_value0", record.stringArrayField.value()[0]);
    Assertions.assertEquals("string_value1", record.stringArrayField.value()[1]);

    jdbcRs.next();
    Assertions.assertFalse(jdbcRs.hasNext());

    conn.runUpdate(dropTableStmt, new ConnectionInterface.QueryParams());
    conn.close();
  }
}
