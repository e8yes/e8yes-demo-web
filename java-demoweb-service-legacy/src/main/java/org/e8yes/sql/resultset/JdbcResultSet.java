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

import java.sql.Array;
import java.sql.ResultSet;
import java.sql.SQLException;
import org.e8yes.sql.primitive.SqlPrimitiveInterface;

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

  @Override
  public <RecordType> void setCellValueToField(int i, SqlPrimitiveInterface field)
      throws SQLException {
    Object val;
    if (field.isArray()) {
      Array arr = rs.getArray(i);
      if (rs.wasNull()) {
        return;
      }
      val = arr.getArray();
    } else {
      val = rs.getObject(i);
      if (rs.wasNull()) {
        return;
      }
    }

    field.importValueFromSqlObject(val);
  }
}
