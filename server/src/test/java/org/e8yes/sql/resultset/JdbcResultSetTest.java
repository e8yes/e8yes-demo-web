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

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Date;
import org.apache.ibatis.session.SqlSession;
import org.e8yes.srvs.EnvironmentContext;
import org.e8yes.srvs.dao.DatabaseConnection;
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

  public static class Record {
    public Integer intField;
    public Long longField;
    public Boolean booleanField;
    public Float floatField;
    public Double doubleField;
    public Date dateField;
    public String stringField;

    public Integer[] intArrayField;
    public Long[] longArrayField;
    public Boolean[] booleanArrayField;
    public Float[] floatArrayField;
    public Double[] doubleArrayField;
    public Date[] dateArrayField;
    public String[] stringArrayField;
  }

  @Test
  public void retrieveOneRecordTest()
      throws SQLException, IllegalArgumentException, IllegalAccessException {
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
    SqlSession sess = DatabaseConnection.openSession();
    Connection conn = sess.getConnection();
    conn.setAutoCommit(true);
    conn.createStatement().executeUpdate(dropTableStmt);
    conn.createStatement().executeUpdate(createTableStmt);

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
    conn.createStatement().executeUpdate(insertRecordStmt);

    String queryStmt = "SELECT * FROM JdbcResultSetTestTable;";
    ResultSet rs = conn.createStatement().executeQuery(queryStmt);
    JdbcResultSet jdbcRs = new JdbcResultSet(rs);
    Assertions.assertTrue(jdbcRs.hasNext());

    Record record = new Record();
    jdbcRs.setCellValueToField(1, Record.class.getFields()[0], record);
    Assertions.assertEquals((Integer) 10, record.intField);

    jdbcRs.setCellValueToField(2, Record.class.getFields()[1], record);
    Assertions.assertEquals((Long) 100L, record.longField);

    jdbcRs.setCellValueToField(3, Record.class.getFields()[2], record);
    Assertions.assertEquals((Boolean) true, record.booleanField);

    jdbcRs.setCellValueToField(4, Record.class.getFields()[3], record);
    Assertions.assertEquals((Float) 0.5f, record.floatField);

    jdbcRs.setCellValueToField(5, Record.class.getFields()[4], record);
    Assertions.assertEquals((Double) 0.25, record.doubleField);

    jdbcRs.setCellValueToField(6, Record.class.getFields()[5], record);
    Assertions.assertEquals((Long) 111000L, (Long) record.dateField.getTime());

    jdbcRs.setCellValueToField(7, Record.class.getFields()[6], record);
    Assertions.assertEquals("string_value", record.stringField);

    jdbcRs.setCellValueToField(8, Record.class.getFields()[7], record);
    Assertions.assertEquals((Integer) 10, record.intArrayField[0]);
    Assertions.assertEquals((Integer) 11, record.intArrayField[1]);

    jdbcRs.setCellValueToField(9, Record.class.getFields()[8], record);
    Assertions.assertEquals((Long) 100L, record.longArrayField[0]);
    Assertions.assertEquals((Long) 101L, record.longArrayField[1]);

    jdbcRs.setCellValueToField(10, Record.class.getFields()[9], record);
    Assertions.assertEquals((Boolean) true, record.booleanArrayField[0]);
    Assertions.assertEquals((Boolean) false, record.booleanArrayField[1]);

    jdbcRs.setCellValueToField(11, Record.class.getFields()[10], record);
    Assertions.assertEquals((Float) 0.5f, record.floatArrayField[0]);
    Assertions.assertEquals((Float) 1.5f, record.floatArrayField[1]);

    jdbcRs.setCellValueToField(12, Record.class.getFields()[11], record);
    Assertions.assertEquals((Double) 0.25, record.doubleArrayField[0]);
    Assertions.assertEquals((Double) 1.25, record.doubleArrayField[1]);

    jdbcRs.setCellValueToField(13, Record.class.getFields()[12], record);
    Assertions.assertEquals((Long) 111000L, (Long) record.dateArrayField[0].getTime());
    Assertions.assertEquals((Long) 112000L, (Long) record.dateArrayField[1].getTime());

    jdbcRs.setCellValueToField(14, Record.class.getFields()[13], record);
    Assertions.assertEquals("string_value0", record.stringArrayField[0]);
    Assertions.assertEquals("string_value1", record.stringArrayField[1]);

    jdbcRs.next();
    Assertions.assertFalse(jdbcRs.hasNext());

    conn.createStatement().executeUpdate(dropTableStmt);
    DatabaseConnection.closeSession(sess);
  }
}
