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

import java.sql.SQLException;
import java.util.Date;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class MockResultSetTest {

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
    MockResultSet rs = new MockResultSet(/*numCells=*/ 14);
    rs.addRecord(
        new Object[] {
          (Integer) 10,
          (Long) 100L,
          (Boolean) true,
          (Float) 0.5f,
          (Double) 0.25,
          new Date(111000L),
          "string_value",
          new Integer[] {(Integer) 10, (Integer) 11},
          new Long[] {(Long) 100L, (Long) 101L},
          new Boolean[] {(Boolean) true, (Boolean) false},
          new Float[] {(Float) 0.5f, (Float) 1.5f},
          new Double[] {(Double) 0.25, (Double) 1.25},
          new Date[] {new Date(111000L), new Date(112000L)},
          new String[] {"string_value0", "string_value1"}
        });

    Assertions.assertTrue(rs.hasNext());

    JdbcResultSetTest.Record record = new JdbcResultSetTest.Record();
    rs.setCellValueToField(1, JdbcResultSetTest.Record.class.getFields()[0], record);
    Assertions.assertEquals((Integer) 10, record.intField);

    rs.setCellValueToField(2, JdbcResultSetTest.Record.class.getFields()[1], record);
    Assertions.assertEquals((Long) 100L, record.longField);

    rs.setCellValueToField(3, JdbcResultSetTest.Record.class.getFields()[2], record);
    Assertions.assertEquals((Boolean) true, record.booleanField);

    rs.setCellValueToField(4, JdbcResultSetTest.Record.class.getFields()[3], record);
    Assertions.assertEquals((Float) 0.5f, record.floatField);

    rs.setCellValueToField(5, JdbcResultSetTest.Record.class.getFields()[4], record);
    Assertions.assertEquals((Double) 0.25, record.doubleField);

    rs.setCellValueToField(6, JdbcResultSetTest.Record.class.getFields()[5], record);
    Assertions.assertEquals((Long) 111000L, (Long) record.dateField.getTime());

    rs.setCellValueToField(7, JdbcResultSetTest.Record.class.getFields()[6], record);
    Assertions.assertEquals("string_value", record.stringField);

    rs.setCellValueToField(8, JdbcResultSetTest.Record.class.getFields()[7], record);
    Assertions.assertEquals((Integer) 10, record.intArrayField[0]);
    Assertions.assertEquals((Integer) 11, record.intArrayField[1]);

    rs.setCellValueToField(9, JdbcResultSetTest.Record.class.getFields()[8], record);
    Assertions.assertEquals((Long) 100L, record.longArrayField[0]);
    Assertions.assertEquals((Long) 101L, record.longArrayField[1]);

    rs.setCellValueToField(10, JdbcResultSetTest.Record.class.getFields()[9], record);
    Assertions.assertEquals((Boolean) true, record.booleanArrayField[0]);
    Assertions.assertEquals((Boolean) false, record.booleanArrayField[1]);

    rs.setCellValueToField(11, JdbcResultSetTest.Record.class.getFields()[10], record);
    Assertions.assertEquals((Float) 0.5f, record.floatArrayField[0]);
    Assertions.assertEquals((Float) 1.5f, record.floatArrayField[1]);

    rs.setCellValueToField(12, JdbcResultSetTest.Record.class.getFields()[11], record);
    Assertions.assertEquals((Double) 0.25, record.doubleArrayField[0]);
    Assertions.assertEquals((Double) 1.25, record.doubleArrayField[1]);

    rs.setCellValueToField(13, JdbcResultSetTest.Record.class.getFields()[12], record);
    Assertions.assertEquals((Long) 111000L, (Long) record.dateArrayField[0].getTime());
    Assertions.assertEquals((Long) 112000L, (Long) record.dateArrayField[1].getTime());

    rs.setCellValueToField(14, JdbcResultSetTest.Record.class.getFields()[13], record);
    Assertions.assertEquals("string_value0", record.stringArrayField[0]);
    Assertions.assertEquals("string_value1", record.stringArrayField[1]);

    rs.next();
    Assertions.assertFalse(rs.hasNext());
  }
}
