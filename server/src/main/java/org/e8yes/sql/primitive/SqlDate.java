package org.e8yes.sql.primitive;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.Date;

/** Date primitive type. */
public class SqlDate implements SqlPrimitiveInterface {
  private Date val;

  public SqlDate() {}

  public SqlDate(Date val) {
    this.val = val;
  }

  @Override
  public void exportValueToStatement(PreparedStatement stmt, int position, Connection conn)
      throws SQLException {
    if (val != null) {
      stmt.setTimestamp(position, new Timestamp(val.getTime()));
    } else {
      stmt.setNull(position, java.sql.Types.TIMESTAMP);
    }
  }

  @Override
  public void importValueFromSqlObject(Object o) {
    val = (Date) o;
  }

  @Override
  public Date value() {
    return val;
  }

  @Override
  public boolean isArray() {
    return false;
  }

  @Override
  public boolean equals(Object obj) {
    return val.equals(((SqlDate) obj).val);
  }

  @Override
  public int hashCode() {
    return val.hashCode();
  }
}
