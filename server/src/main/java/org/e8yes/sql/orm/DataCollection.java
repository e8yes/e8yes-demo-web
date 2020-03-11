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
package org.e8yes.sql.orm;

import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.e8yes.sql.resultset.ResultSetInterface;

/** Collects data from SQL ResultSet and transform the data into regular Java objects. */
public class DataCollection {

  private static class CellNumberIterator {
    private int i = 0;

    public int next() {
      return ++i;
    }
  }

  private static Object retrieveRowDataForTable(
      Class tableType, ResultSetInterface rs, CellNumberIterator cellNumIterator)
      throws NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException, SQLException {
    Object record = tableType.getConstructor().newInstance();

    // Assign values to the table's fields.
    for (Field field : tableType.getDeclaredFields()) {
      rs.setCellValueToField(cellNumIterator.next(), field, record);
    }

    return record;
  }

  private static class TempRecordStorage<ReturnType> {
    public ReturnType mainRecord;
    public Map<Field, List<Object>> mergedOneToManyRowData;
    public int recordPosition;

    public TempRecordStorage(
        ReturnType mainRecord, Map<Field, Object> oneToManyRowData, int recordPosition) {
      this.mainRecord = mainRecord;
      mergedOneToManyRowData = new HashMap();
      for (Map.Entry<Field, Object> row : oneToManyRowData.entrySet()) {
        mergedOneToManyRowData.put(
            row.getKey(),
            new ArrayList<Object>() {
              {
                add(row.getValue());
              }
            });
      }
      this.recordPosition = recordPosition;
    }

    public void mergeOneToManyRelation(Map<Field, Object> oneToManyRowData) {
      for (Map.Entry<Field, Object> row : oneToManyRowData.entrySet()) {
        mergedOneToManyRowData.get(row.getKey()).add(row.getValue());
      }
    }

    public void assignOneToManyRowDataToMainRecord()
        throws IllegalArgumentException, IllegalAccessException {
      for (Map.Entry<Field, List<Object>> row : mergedOneToManyRowData.entrySet()) {
        Field field = row.getKey();
        List<Object> arrVal = row.getValue();
        Object arr = Array.newInstance(field.getType().getComponentType(), arrVal.size());
        for (int i = 0; i < arrVal.size(); i++) {
          Array.set(arr, i, arrVal.get(i));
        }
        field.set(mainRecord, arr);
      }
    }
  }

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
    Map<ReturnType, TempRecordStorage> records = new HashMap();
    int recordPosition = 0;
    for (; rs.hasNext(); rs.next()) {
      ReturnType rowRecord = (ReturnType) resultType.getConstructor().newInstance();

      // Constructs Java instance for each table and set the instances which belongs to the
      // one-to-one tables to the record wrapper class resultType. One-to-many tables are only
      // cached, instead of assigning to the wrapper class, because we want to merge the current row
      // with all other rows which share the same keys defined in the one-to-one tables.
      Map<Field, Object> oneToManyRowData = new HashMap();
      CellNumberIterator cellNumIterator = new CellNumberIterator();
      for (Field table : resultType.getDeclaredFields()) {
        if (!table.getType().isArray()) {
          Object rowDataForTable =
              DataCollection.retrieveRowDataForTable(table.getType(), rs, cellNumIterator);
          table.set(rowRecord, rowDataForTable);
        } else {
          Object rowDataForTable =
              DataCollection.retrieveRowDataForTable(
                  table.getType().getComponentType(), rs, cellNumIterator);
          oneToManyRowData.put(table, rowDataForTable);
        }
      }

      // Create new temp storage for unseen main record, or merge one-to-many relations.
      TempRecordStorage<ReturnType> storage = records.get(rowRecord);
      if (storage == null) {
        records.put(rowRecord, new TempRecordStorage(rowRecord, oneToManyRowData, recordPosition));
      } else {
        storage.mergeOneToManyRelation(oneToManyRowData);
      }

      recordPosition++;
    }

    // Convert TempRecordStorage to ReturnType.
    ArrayList<ReturnType> results = new ArrayList(records.size());
    for (int i = 0; i < records.size(); i++) {
      results.add(null);
    }
    for (TempRecordStorage<ReturnType> record : records.values()) {
      record.assignOneToManyRowDataToMainRecord();
      results.set(record.recordPosition, record.mainRecord);
    }
    return results;
  }
}
