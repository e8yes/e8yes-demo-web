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
public final class DBConnector {

	private Connection m_db_conn = null;	// Database connection.

	private static final String MYSQL_DRIVER = "com.mysql.jdbc.Driver";
	private static final String MYSQL_URL = "jdbc:mysql://localhost:3306/webchat?useUnicode=true&characterEncoding=utf8";
	private static final String MYSQL_USER = "root";
	private static final String MYSQL_PASSWORD = "password";
	private static final int MYSQL_CONNECTION_TIMED_OUT_ERROR_CODE = 400;

	private static final String H2_DRIVER = "org.h2.Driver";
	private static final String H2_URL = "jdbc:h2:file:~/webchat;MODE=MySQL;IGNORECASE=TRUE";
	//private static final String	H2_URL = "jdbc:h2:mem:MODE=MySQL;IGNORECASE=TRUE";
	private static final String H2_USER = "sa";
	private static final String H2_PASSWORD = "";
	private static final int H2_CONNECTION_TIMED_OUT_ERROR_CODE = 400;

	private static final String DRIVER = H2_DRIVER;
	private static final String URL = H2_URL;
	private static final String USER = H2_USER;
	private static final String PASSWORD = H2_PASSWORD;
	private static final int CONNECTION_TIMED_OUT_ERROR_CODE = H2_CONNECTION_TIMED_OUT_ERROR_CODE;

	private void create_connection() throws SQLException {
		try {
			if (m_db_conn != null && !m_db_conn.isClosed()) {
				m_db_conn.close();
			}

			// Load database driver.
			Class.forName(DRIVER);

			// Connect to database server.
			m_db_conn = DriverManager.getConnection(URL, USER, PASSWORD);
		} catch (ClassNotFoundException ex) {
			Logger.getLogger(DBConnector.class.getName()).log(Level.SEVERE, null, ex);
		}
	}

	public DBConnector() {
		try {
			create_connection();
			set_database_utf8_character_set("webchat");
		} catch (SQLException ex) {
			Logger.getLogger(DBConnector.class.getName()).log(Level.SEVERE, null, ex);
		}
	}
	
	public void destroy() {
		try {
			if (!m_db_conn.isClosed()) {
				m_db_conn.close();
			}
		} catch (SQLException ex) {
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
		try {
			create_connection();
		} catch (SQLException ex2) {
			Logger.getLogger(DBConnector.class.getName()).log(Level.SEVERE, null, ex2);
			throw ex;
		}
		return true;
	}

	public void set_database_utf8_character_set(String database_name) throws SQLException {
		if (DRIVER.equals(MYSQL_DRIVER)) {
			// Enforce utf8 encoding.
			m_db_conn.createStatement().executeUpdate(
				"alter database " + database_name + " character set utf8 collate utf8_general_ci;");
		}
	}

	public void set_table_utf8_character_set(String table_name) throws SQLException {
		if (DRIVER.equals(MYSQL_DRIVER)) {
			// Enforce utf8 encoding.
			get_statement().executeUpdate(
				"alter table " + table_name + " convert to character set utf8;");
		}
	}

	public int process_update(Statement statement, String sql) throws SQLException {
		try {
			return statement.executeUpdate(sql);
		} catch (SQLException ex) {
			if (process_exception(ex)) {
				return get_statement().executeUpdate(sql);
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
				return get_statement().executeQuery(sql);
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
