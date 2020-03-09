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

import java.lang.reflect.Field;
import java.sql.Array;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.Date;

/** JDBC implementation of the result set interface */
public class JdbcResultSet implements ResultSetInterface {

  private final ResultSet rs;
  private boolean hasNextRecord;

  public JdbcResultSet(ResultSet rs) throws SQLException {
    this.rs = rs;
    this.hasNextRecord = this.rs.next();
  }

  @Override
  public void next() throws SQLException {
    this.hasNextRecord = this.rs.next();
  }

  @Override
  public boolean hasNext() {
    return this.hasNextRecord;
  }

  private static <RecordType> void setCellValueToRegularField(
      ResultSet rs, int i, Field field, RecordType record)
      throws SQLException, IllegalArgumentException, IllegalAccessException {
    Object val = rs.getObject(i);
    if (rs.wasNull()) {
      return;
    }
    if (field.getType().equals(Integer.class)) {
      field.set(record, (Integer) val);
    } else if (field.getType().equals(Long.class)) {
      field.set(record, (Long) val);
    } else if (field.getType().equals(Boolean.class)) {
      field.set(record, (Boolean) val);
    } else if (field.getType().equals(Float.class)) {
      field.set(record, (Float) val);
    } else if (field.getType().equals(Double.class)) {
      field.set(record, (Double) val);
    } else if (field.getType().equals(Date.class)) {
      field.set(record, new Date(((Timestamp) val).getTime()));
    } else if (field.getType().equals(String.class)) {
      field.set(record, val);
    } else {
      throw new IllegalArgumentException(
          "Unexpected type when assigning cell value. type=" + field.getType());
    }
  }

  private static <RecordType> void setCellValueToArrayField(
      ResultSet rs, int i, Field field, RecordType record)
      throws SQLException, IllegalArgumentException, IllegalAccessException {
    Array val = rs.getArray(i);
    if (rs.wasNull()) {
      return;
    }
    if (field.getType().getComponentType().equals(Integer.class)) {
      field.set(record, (Integer[]) val.getArray());
    } else if (field.getType().getComponentType().equals(Long.class)) {
      field.set(record, (Long[]) val.getArray());
    } else if (field.getType().getComponentType().equals(Boolean.class)) {
      field.set(record, (Boolean[]) val.getArray());
    } else if (field.getType().getComponentType().equals(Float.class)) {
      field.set(record, (Float[]) val.getArray());
    } else if (field.getType().getComponentType().equals(Double.class)) {
      field.set(record, (Double[]) val.getArray());
    } else if (field.getType().getComponentType().equals(Date.class)) {
      Timestamp[] timestamps = (Timestamp[]) val.getArray();
      Date[] utilDates = new Date[timestamps.length];
      for (int j = 0; j < timestamps.length; j++) {
        utilDates[j] = new Date(timestamps[j].getTime());
      }
      field.set(record, utilDates);
    } else if (field.getType().getComponentType().equals(String.class)) {
      field.set(record, (String[]) val.getArray());
    } else {
      throw new IllegalArgumentException(
          "Unexpected array type when assigning cell value. type="
              + field.getType().getComponentType());
    }
  }

  @Override
  public <RecordType> void setCellValueToField(int i, Field field, RecordType record)
      throws SQLException, IllegalArgumentException, IllegalAccessException {
    if (field.getType().isArray()) {
      JdbcResultSet.setCellValueToArrayField(this.rs, i, field, record);
    } else {
      JdbcResultSet.setCellValueToRegularField(this.rs, i, field, record);
    }
  }
}
