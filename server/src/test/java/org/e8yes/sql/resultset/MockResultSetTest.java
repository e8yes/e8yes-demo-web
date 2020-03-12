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
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class MockResultSetTest {

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

    MockResultSetTest.Record record = new MockResultSetTest.Record();
    rs.setCellValueToField(1, record.intField);
    Assertions.assertEquals((Integer) 10, record.intField.value());

    rs.setCellValueToField(2, record.longField);
    Assertions.assertEquals((Long) 100L, record.longField.value());

    rs.setCellValueToField(3, record.booleanField);
    Assertions.assertEquals((Boolean) true, record.booleanField.value());

    rs.setCellValueToField(4, record.floatField);
    Assertions.assertEquals((Float) 0.5f, record.floatField.value());

    rs.setCellValueToField(5, record.doubleField);
    Assertions.assertEquals((Double) 0.25, record.doubleField.value());

    rs.setCellValueToField(6, record.dateField);
    Assertions.assertEquals((Long) 111000L, (Long) record.dateField.value().getTime());

    rs.setCellValueToField(7, record.stringField);
    Assertions.assertEquals("string_value", record.stringField.value());

    rs.setCellValueToField(8, record.intArrayField);
    Assertions.assertEquals((Integer) 10, record.intArrayField.value()[0]);
    Assertions.assertEquals((Integer) 11, record.intArrayField.value()[1]);

    rs.setCellValueToField(9, record.longArrayField);
    Assertions.assertEquals((Long) 100L, record.longArrayField.value()[0]);
    Assertions.assertEquals((Long) 101L, record.longArrayField.value()[1]);

    rs.setCellValueToField(10, record.booleanArrayField);
    Assertions.assertEquals((Boolean) true, record.booleanArrayField.value()[0]);
    Assertions.assertEquals((Boolean) false, record.booleanArrayField.value()[1]);

    rs.setCellValueToField(11, record.floatArrayField);
    Assertions.assertEquals((Float) 0.5f, record.floatArrayField.value()[0]);
    Assertions.assertEquals((Float) 1.5f, record.floatArrayField.value()[1]);

    rs.setCellValueToField(12, record.doubleArrayField);
    Assertions.assertEquals((Double) 0.25, record.doubleArrayField.value()[0]);
    Assertions.assertEquals((Double) 1.25, record.doubleArrayField.value()[1]);

    rs.setCellValueToField(13, record.dateArrayField);
    Assertions.assertEquals((Long) 111000L, (Long) record.dateArrayField.value()[0].getTime());
    Assertions.assertEquals((Long) 112000L, (Long) record.dateArrayField.value()[1].getTime());

    rs.setCellValueToField(14, record.stringArrayField);
    Assertions.assertEquals("string_value0", record.stringArrayField.value()[0]);
    Assertions.assertEquals("string_value1", record.stringArrayField.value()[1]);

    rs.next();
    Assertions.assertFalse(rs.hasNext());
  }
}
