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
import org.e8yes.sql.primitive.SqlPrimitiveInterface;

/** An iterator to access the set of rows resulting from a query. */
public interface ResultSetInterface {

  /**
   * Move cursor to the next record if possible.
   *
   * @throws SQLException When hasNext() returns false.
   */
  public void next() throws SQLException;

  /**
   * Check if the cursor reaches the end of the result set.
   *
   * @return false when the cursor has moved to the end of the result set. Otherwise, it returns
   *     true.
   */
  public boolean hasNext();

  /**
   * Set the value of the ith cell at the current row cursor position to the record's field.
   *
   * @param <RecordType> The type of record.
   * @param i The ith cell to pull value from.
   * @param field The field to assign value to.
   * @throws SQLException
   * @throws IllegalArgumentException
   */
  public <RecordType> void setCellValueToField(int i, SqlPrimitiveInterface field)
      throws SQLException;
}
