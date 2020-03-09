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
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/** This implementations tailors to testing purposes */
public class MockResultSet implements ResultSetInterface {

  private final int numCells;
  private final List<Object[]> records = new ArrayList();
  private int curRecord = 0;

  public MockResultSet(int numCells) {
    this.numCells = numCells;
  }

  public void addRecord(Object[] record) throws IllegalArgumentException {
    if (record.length > numCells) {
      throw new IllegalArgumentException(
          "Record length is greater than the number of cells available. numCells=" + numCells);
    }
    records.add(record);
  }

  @Override
  public void next() throws SQLException {
    if (curRecord >= records.size()) {
      throw new SQLException("Cursor has reached the end of the result set.");
    }
    curRecord++;
  }

  @Override
  public boolean hasNext() {
    return curRecord < records.size();
  }

  @Override
  public <RecordType> void setCellValueToField(int i, Field field, RecordType record)
      throws IllegalArgumentException, IllegalAccessException, SQLException {
    if (i <= 0 || i > numCells) {
      throw new SQLException(
          "Cell index is out of bound. lowerBound="
              + 1
              + "|upperBound="
              + numCells
              + "|actual="
              + i);
    }
    Object val = records.get(curRecord)[i - 1];
    if (val == null) {
      return;
    }
    field.set(record, val);
  }
}
