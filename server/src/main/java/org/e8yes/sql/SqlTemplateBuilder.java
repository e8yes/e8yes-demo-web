/**
 * e8yes demo web server.
 * 
 * Copyright (C) 2020 Yichen Ma Yichen Ma {yichenm2@uci.edu}, Chifeng Wen {daviesx66@gmail.com}
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
import java.lang.reflect.InvocationTargetException;
import java.util.List;


/**
 *
 * @author zjz20
 */
public class SqlTemplateBuilder {
        private Class resultType;
        private String query;
        
        /**
         * Example: query = "FROM AUser user 
         * JOIN CreditCard card ON card.userId = user.id WHERE user.joinDate > '2020-1-1'"
         * @param query
         * @return 
         */
        public SqlTemplateBuilder withSelect(String query) {
                this.query = query;
                return this;
        }
        
        public SqlTemplateBuilder withResultType(Class resultType) {
                this.resultType = resultType;
                return this;
        }
        
        public <ReturnType> List<ReturnType> run(Class classType) throws NoSuchMethodException, 
                                                                         InstantiationException, 
                                                                         IllegalAccessException, 
                                                                         IllegalArgumentException, 
                                                                         InvocationTargetException {
                 throw new UnsupportedOperationException();
        }
        
        public static String completeSelectQuery(String query, Class resultType){
                StringBuilder sb = new StringBuilder("select ");
                for (Field alias : resultType.getDeclaredFields()){
                        Class aliasType;
                        if (alias.getType().isArray()) {
                                aliasType = alias.getType().getComponentType();
                        } else {
                                aliasType = alias.getType();
                        }
                        for (Field field : aliasType.getDeclaredFields()){
                               sb.append(alias.getName());
                               sb.append('.');
                               sb.append(field.getName());
                               sb.append(',');
                        }
                }
                sb.setLength(sb.length() - 1);
                sb.append(' ');
                sb.append(query);
                return sb.toString();
        }
}
