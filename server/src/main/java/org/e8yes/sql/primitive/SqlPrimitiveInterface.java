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

/** Represents SQL primitive type and serves as a bridge between SQL object and Java object */
public interface SqlPrimitiveInterface {

  /**
   * Export value to an SQL statement.
   *
   * @param stmt SQL statement to export to.
   * @param position Position of the placeholder to be assigned a value of.
   * @throws SQLException
   */
  void exportValueToStatement(PreparedStatement stmt, int position, Connection conn)
      throws SQLException;

  /**
   * Import from an SQL object and internally converts to a Java object.
   *
   * @param o SQL object to import from.
   * @throws SQLException
   */
  void importValueFromSqlObject(Object o) throws SQLException;

  /**
   * Retrieve the internally represented Java object.
   *
   * @param <Type>
   * @return The Java object if not null. Otherwise, null.
   */
  <Type> Type value();

  /**
   * Is this an array type?
   *
   * @return If so, true.
   */
  boolean isArray();

  /**
   * Implementation of this method is required.
   *
   * @param o The other side of equivalence.
   * @return True if equivalent.
   */
  @Override
  boolean equals(Object o);
}
