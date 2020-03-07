/**
 * e8yes demo web server.
 *
 * Copyright (C) 2020 Yichen Ma {yichenm2@uci.edu}, Chifeng Wen {daviesx66@gmail.com}
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.e8yes.sql;

import java.lang.reflect.Field;

/**
 * Functions to automate SQL query completion via reflection.
 *
 * @author davis
 */
public class SqlCompletion {

    /**
     * It takes in a partial select query and the result class structure and
     * complete the select list forming a valid SQL query.
     *
     * @param query A partial query to be completed.
     * @param resultType Defines the class structure which is used to form the
     * select list.
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
}
