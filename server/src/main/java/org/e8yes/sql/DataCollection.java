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

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import org.e8yes.sql.resultset.ResultSetInterface;

/** Collects data from SQL ResultSet and transform the data into regular Java objects. */
public class DataCollection {

  /**
   * Achieves object relational mapping. In particular, it collects each of row of data from the
   * table-format ResultSetInterface and assigns the cells' value to fields in a new Java class
   * instance.
   *
   * @param <ReturnType> The type of Java object each row is going to convert into.
   * @param rs Query result.
   * @param resultType Class type of the Java object the result set is going to convert into.
   * @return List of records constructable from the result set.
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   * @throws SQLException
   */
  public static <ReturnType> List<ReturnType> collect(ResultSetInterface rs, Class resultType)
      throws NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException, SQLException {
    List<ReturnType> results = new ArrayList();
    for (; rs.hasNext(); rs.next()) {
      ReturnType record = (ReturnType) resultType.getConstructor().newInstance();

      // Construct table data and set the instance to the table field.
      int i = 1;
      for (Field tableField : resultType.getDeclaredFields()) {
        Class tableType;
        if (tableField.getType().isArray()) {
          // TODO: Doesn't support one-to-many relation for now.
          // tableType = tableField.getType().getComponentType();
          continue;
        } else {
          tableType = tableField.getType();
        }
        Object tableData = tableType.getConstructor().newInstance();

        // Assign values to the table's fields.
        for (Field field : tableType.getDeclaredFields()) {
          rs.setCellValueToField(i, field, tableData);
          i++;
        }
        tableField.set(record, tableData);
      }

      results.add(record);
    }
    return results;
  }
}
