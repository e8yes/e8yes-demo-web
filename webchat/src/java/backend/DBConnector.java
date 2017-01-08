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

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author davis
 */
public class DBConnector {
        private Connection      m_db_conn;      // Database connection.
        
        public DBConnector() {
                try {
                        // Check for MySQL driver.
                        Class.forName("com.mysql.jdbc.Driver");
                        
                        // Connect to MySQL server with utf8-encoded connection.
                        m_db_conn = DriverManager.getConnection(
                                "jdbc:mysql://localhost:3306/webchat?characterEncoding=utf-8",
                                "root", "password");
                        
                        // Enforce utf8 encoding.
                        m_db_conn.createStatement().executeUpdate("alter database webchat character set utf8 collate utf8_general_ci;");
                } catch (ClassNotFoundException | SQLException ex) {
                        Logger.getLogger(DBConnector.class.getName()).log(Level.SEVERE, null, ex);
                }
        }
        
        public Connection get_connection() {
                return m_db_conn;
        }
}
