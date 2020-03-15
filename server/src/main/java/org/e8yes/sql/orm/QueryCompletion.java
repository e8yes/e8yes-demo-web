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

import java.lang.reflect.Field;

/** Functions to automate SQL query completion via reflection. */
public class QueryCompletion {

  /**
   * It takes in a partial select query and the result class structure and complete the select list
   * forming a valid SQL query.
   *
   * @param query A partial query to be completed.
   * @param resultType Defines the class structure which is used to form the select list.
   * @return A complete select SQL query.
   */
  public static String completeSelectQuery(String query, Class resultType) {
    StringBuilder sb = new StringBuilder("SELECT ");
    for (Field alias : resultType.getDeclaredFields()) {
      Class aliasType;
      if (alias.getType().isArray()) {
        aliasType = alias.getType().getComponentType();
      } else {
        aliasType = alias.getType();
      }
      for (Field field : aliasType.getDeclaredFields()) {
        sb.append(alias.getName());
        sb.append('.');
        sb.append(field.getName());
        sb.append(',');
      }
    }
    sb.setLength(sb.length() - 1);
    sb.append(" FROM ");
    sb.append(query);
    return sb.toString();
  }

  /**
   * It takes in the target table name and entity type and construct an insertion sql query.
   *
   * @param tableName Name of the table to insert the record into..
   * @param entityType Type of the entity to be inserted.
   * @param withUpsert Whether to builder a query which upserts a record.
   * @return A complete insertion SQL query.
   */
  public static String completeInsertQuery(String tableName, Class entityType, boolean withUpsert) {
    Field[] fields = entityType.getDeclaredFields();
    assert (fields.length > 0);

    StringBuilder query = new StringBuilder();
    query.append("INSERT INTO ");
    query.append(tableName);
    query.append("(");
    for (Field field : fields) {
      query.append(field.getName());
      query.append(",");
    }
    query.setLength(query.length() - 1);
    query.append(")VALUES(?");
    for (int i = 1; i < fields.length; i++) {
      query.append(",?");
    }
    query.append(")");

    if (withUpsert) {
      // Update record on conflict.
      query.append("ON CONFLICT DO UPDATE SET ");
      for (Field field : fields) {
        query.append(field.getName());
        query.append("=?,");
      }
      query.setLength(query.length() - 1);
    }
    return query.toString();
  }
}
