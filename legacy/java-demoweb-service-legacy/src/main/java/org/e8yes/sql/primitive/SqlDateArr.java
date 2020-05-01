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
package org.e8yes.sql.primitive;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.Arrays;
import java.util.Date;

/** Date array primitive type. */
public class SqlDateArr implements SqlPrimitiveInterface {
  private Date[] val;

  public SqlDateArr() {}

  public SqlDateArr(Date[] val) {
    this.val = val;
  }

  @Override
  public void exportValueToStatement(PreparedStatement stmt, int position, Connection conn)
      throws SQLException {
    if (val != null) {
      stmt.setArray(position, conn.createArrayOf("TIMESTAMP WITHOUT TIME ZONE", val));
    } else {
      stmt.setNull(position, java.sql.Types.ARRAY);
    }
  }

  @Override
  public void importValueFromSqlObject(Object o) {
    val = (Date[]) o;
  }

  public void assign(Date[] val) {
    this.val = val;
  }

  @Override
  public Date[] value() {
    return val;
  }

  @Override
  public boolean isArray() {
    return true;
  }

  @Override
  public boolean equals(Object obj) {
    return Arrays.equals(val, ((SqlDateArr) obj).val);
  }

  @Override
  public int hashCode() {
    return Arrays.hashCode(val);
  }
}
