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

import com.sun.corba.se.impl.orbutil.concurrent.Mutex;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author davis
 */
public class UUIDGenerator {
        
        private final DBConnector       m_conn;
        private final String            m_entity_name;
        private final int               m_min;
        private final Mutex             m_mutex;
        
        public UUIDGenerator(DBConnector conn, String entity_name, int min) {
                m_conn = conn;
                m_entity_name = entity_name;
                m_min = min;
                m_mutex = new Mutex();
                try {
                        // Create the counter table.
                        Statement s = m_conn.get_connection().createStatement();
                        s.executeUpdate("create table if not exists " + m_entity_name + "("
                                + "bid integer,"
                                + "uid integer,"
                                + "primary key (bid));");
                } catch (SQLException ex) {
                        Logger.getLogger(UserManager.class.getName()).log(Level.SEVERE, null, ex);
                }
        }
        
        public Integer get_next() {
                try {
                        m_mutex.acquire();
                        
                        Statement s = m_conn.get_connection().createStatement();
                        int r = s.executeUpdate("update " + m_entity_name
                                + " set uid = uid + 1 "
                                + " where bid = 0");
                        if (r == 0) {
                                // The row for ClickCounter doesn't exist yet.
                                s.executeUpdate("insert into " + m_entity_name
                                        + " (bid, uid) "
                                        + " values(0, " + m_min + ");");
                        }
                        ResultSet result = s.executeQuery("select uid from " + m_entity_name + " where bid = 0");
                        
                        m_mutex.release();
                        
                        if (result.next())
                                return result.getInt("uid");
                        else
                                return null;
                } catch (SQLException ex) {
                        Logger.getLogger(UUIDGenerator.class.getName()).log(Level.SEVERE, null, ex);
                        m_mutex.release();
                        return null;
                } catch (InterruptedException ex) {
                        Logger.getLogger(UUIDGenerator.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }
}
