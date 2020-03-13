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
package org.e8yes.sql.connection;

import java.util.HashMap;
import java.util.Map;
import org.e8yes.sql.primitive.SqlPrimitiveInterface;
import org.e8yes.sql.resultset.ResultSetInterface;

/** */
public interface ConnectionInterface {

  /** */
  public static class QueryParams {
    Map<Integer, SqlPrimitiveInterface> params = new HashMap();

    public void clear() {
      params.clear();
    }

    public void setparam(int position, SqlPrimitiveInterface val) {
      params.put(position, val);
    }

    public SqlPrimitiveInterface getParam(int position) {
      return params.get(position);
    }
  }

  /**
   * @param query
   * @param params
   * @return
   */
  ResultSetInterface runQuery(String query, QueryParams params);

  /**
   * @param query
   * @param params
   * @return
   */
  long runUpdate(String query, QueryParams params);
}
