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

/** Long integer primitive type. */
public class SqlLong implements SqlPrimitiveInterface {

  private Long val;

  public SqlLong() {}

  public SqlLong(Long val) {
    this.val = val;
  }

  @Override
  public void exportValueToStatement(PreparedStatement stmt, int position, Connection conn)
      throws SQLException {
    if (val != null) {
      stmt.setLong(position, val);
    } else {
      stmt.setNull(position, java.sql.Types.BIGINT);
    }
  }

  @Override
  public void importValueFromSqlObject(Object o) throws SQLException {
    val = (Long) o;
  }

  public void assign(Long val) {
    this.val = val;
  }

  @Override
  public Long value() {
    return val;
  }

  @Override
  public boolean isArray() {
    return false;
  }

  @Override
  public boolean equals(Object obj) {
    return val.equals(((SqlLong) obj).val);
  }

  @Override
  public int hashCode() {
    return val.hashCode();
  }
}
