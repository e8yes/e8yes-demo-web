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
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @author davis
 */
public class DBConnector {
        private Connection      m_db_conn = null;	// Database connection.
	
	private static final String	MYSQL_DRIVER = "com.mysql.jdbc.Driver";
	private static final String	MYSQL_URL = "jdbc:mysql://localhost:3306/webchat?characterEncoding=utf-8";
	private static final String	USER = "root";
	private static final String	PASSWORD = "password";
	private static final int	MYSQL_CONNECTION_TIMED_OUT_ERROR_CODE = 400;
	
	private static final String	DRIVER = MYSQL_DRIVER;
	private static final String	URL = MYSQL_URL;
	private static final int	CONNECTION_TIMED_OUT_ERROR_CODE = MYSQL_CONNECTION_TIMED_OUT_ERROR_CODE;
	
	private void create_connection() throws SQLException {
		if (m_db_conn != null)
			m_db_conn.close();
                // Connect to database server.
                m_db_conn = DriverManager.getConnection(URL, USER, PASSWORD);
	}
        
        public DBConnector() {
                try {
			// Check for MySQL driver.
			Class.forName(DRIVER);
		
                        create_connection();
                        
                        // Enforce utf8 encoding.
                        m_db_conn.createStatement().executeUpdate(
				"alter database webchat character set utf8 collate utf8_general_ci;");
                } catch (ClassNotFoundException | SQLException ex) {
                        Logger.getLogger(DBConnector.class.getName()).log(Level.SEVERE, null, ex);
                }
        }
	
	private boolean process_exception(SQLException ex) throws SQLException {
//		if (ex.getErrorCode() == CONNECTION_TIMED_OUT_ERROR_CODE) {
//			create_connection();
//			return true;
//		} else {
//			throw ex;
//		}
		// Simply try to re-establish the connection.
		create_connection();
		return true;
	}
        
	public int process_update(Statement statement, String sql) throws SQLException {
		try {
			return statement.executeUpdate(sql);
		} catch (SQLException ex) {
			if (process_exception(ex)) {
				return statement.executeUpdate(sql);
			} else {
				return 0;
			}
		}
	}
	
	public int process_update(PreparedStatement ps) throws SQLException {
		try {
			return ps.executeUpdate();
		} catch (SQLException ex) {
			if (process_exception(ex)) {
				return ps.executeUpdate();
			} else {
				return 0;
			}
		}
	}
	
	public ResultSet process_query(Statement statement, String sql) throws SQLException {
		try {
			return statement.executeQuery(sql);
		} catch (SQLException ex) {
			if (process_exception(ex)) {
				return statement.executeQuery(sql);
			} else {
				return null;
			}
		}
	}
	
	public ResultSet process_query(PreparedStatement ps) throws SQLException {
		try {
			return ps.executeQuery();
		} catch (SQLException ex) {
			if (process_exception(ex)) {
				return ps.executeQuery();
			} else {
				return null;
			}
		}
	}
	
	public Statement get_statement() throws SQLException {
		try {
			return m_db_conn.createStatement();
		} catch (SQLException ex) {
			if (process_exception(ex)) {
				return m_db_conn.createStatement();
			} else {
				return null;
			}
		}
	}
	
	public PreparedStatement get_prepared_statement(String sql) throws SQLException {
		try {
			return m_db_conn.prepareStatement(sql);
		} catch (SQLException ex) {
			if (process_exception(ex)) {
				return m_db_conn.prepareStatement(sql);
			} else {
				return null;
			}
		}
	}
}
