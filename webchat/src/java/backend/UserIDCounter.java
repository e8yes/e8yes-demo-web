/*
 * Copyright (C) 2017 davis
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
package backend;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author davis
 */
public class UserIDCounter {
        
        private DBConnector m_conn;
        
        public UserIDCounter(DBConnector conn) {
                m_conn = conn;
                try {
                        // Create the counter table.
                        Statement s = m_conn.get_connection().createStatement();
                        s.executeUpdate("create table if not exists uid_counter("
                                + "bid integer,"
                                + "uid integer,"
                                + "primary key (bid));");
                } catch (SQLException ex) {
                        Logger.getLogger(UserManager.class.getName()).log(Level.SEVERE, null, ex);
                }
        }
        
        public Integer get_next() {
                try {
                        Statement s = m_conn.get_connection().createStatement();
                        int r = s.executeUpdate("update uid_counter "
                                + "set uid = uid + 1 "
                                + "where bid = 0");
                        if (r == 0) {
                                // The row for ClickCounter doesn't exist yet.
                                s.executeUpdate("insert into uid_counter "
                                        + "(bid, uid) "
                                        + "values(0, 10000);");
                        }
                        ResultSet result = s.executeQuery("select uid from uid_counter where bid = 0");
                        if (result.next())
                                return result.getInt("uid");
                        else
                                return null;
                } catch (SQLException ex) {
                        Logger.getLogger(UserIDCounter.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }
}
