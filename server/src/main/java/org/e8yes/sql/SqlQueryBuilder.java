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
package org.e8yes.sql;

import java.util.ArrayList;
import java.util.List;
import org.e8yes.sql.connection.ConnectionInterface;
import org.e8yes.sql.primitive.SqlPrimitiveInterface;

/**
 * Builder class to construct a parameterized SQL query from query string pieces and variable
 * placeholders
 */
public class SqlQueryBuilder {

  private final ConnectionInterface.QueryParams params = new ConnectionInterface.QueryParams();
  private int curPlaceHolderId = 0;
  private final StringBuilder queryPieces = new StringBuilder();

  private int nextPlaceHolderId() {
    return ++curPlaceHolderId;
  }

  /**
   * Append a query string piece.
   *
   * @param piece Query string piece
   * @return The current builder.
   */
  public SqlQueryBuilder queryPiece(String piece) {
    queryPieces.append(piece);
    return this;
  }

  /**
   * Represents a variable placeholder
   *
   * @param <Type>
   */
  public static class Placeholder<Type> {
    private final List<Integer> slots = new ArrayList();
  }

  /**
   * Append a variable placeholder.
   *
   * @param holder Variable placeholder
   * @return The current builder.
   */
  public SqlQueryBuilder placeholder(Placeholder holder) {
    holder.slots.add(this.nextPlaceHolderId());
    queryPieces.append("?");
    return this;
  }

  /**
   * Collectively assigns the value to the holder to all the query positions.
   *
   * @param <Type>
   * @param holder Placeholder to be assigned a value.
   * @param val The value to be assigned.
   */
  public <Type> void setPlaceholderValue(Placeholder<Type> holder, SqlPrimitiveInterface val) {
    for (int slot : holder.slots) {
      params.setparam(slot, val);
    }
  }

  /**
   * Export a JDBC-compliant query from the previously appended pieces.
   *
   * @return JDBC-compliant query
   */
  public String jdbcQuery() {
    return queryPieces.toString();
  }

  /**
   * Export the assigned query parameters.
   *
   * @return The assigned query parameters
   */
  public ConnectionInterface.QueryParams queryParams() {
    return params;
  }
}
