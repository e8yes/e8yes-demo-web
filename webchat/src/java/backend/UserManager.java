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

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author davis
 */
public class UserManager {
        
        private DBConnector m_conn;
        private UserIDGenerator m_uid_counter;
        
        public static String get_entity_name() {
                return "user_manager";
        }
        
        public static String get_key_name() {
                return "(uid)";
        }
        
        public static String get_pk_name() {
                return "uid";
        }
        
        public static String get_alias_name() {
                return "alias";
        }
        
        public UserManager(DBConnector conn, UserIDGenerator cnt) {
                m_conn = conn;
                m_uid_counter = cnt;
                try {
                        Statement s = m_conn.get_connection().createStatement();
                        s.executeUpdate("create table if not exists user_manager("
                                + "uid integer,"
                                + "alias varchar(20),"
                                + "pwd varchar(20),"
                                + "primary key (uid)) default character set=utf8;");
                } catch (SQLException ex) {
                        Logger.getLogger(UserManager.class.getName()).log(Level.SEVERE, null, ex);
                }
        }
        
        public boolean has_user(int user_id) {
                try {
                        Statement s = m_conn.get_connection().createStatement();
                        ResultSet result = s.executeQuery("select 1 from user_manager where uid = " + user_id + ";");
                        return result.next();
                } catch (SQLException ex) {
                        Logger.getLogger(UserManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
        public User get_user(int user_id) {
                try {
                        Statement s = m_conn.get_connection().createStatement();
                        ResultSet result = s.executeQuery("select * from user_manager where uid = " + user_id + ";");
                        if (result.next()) {
                                return new User(result.getInt("uid"), result.getString("alias"), result.getString("pwd"));
                        } else {
                                return null;
                        }
                } catch (SQLException ex) {
                        Logger.getLogger(UserManager.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }
        
        public boolean set_user(User user) {
                try {
                        PreparedStatement ps = m_conn.get_connection().prepareStatement(
                                "update user_manager set pwd = ?, alias = ? where uid = ?;");
                        ps.setString(1, user.get_password());
                        ps.setString(2, user.get_alias());
                        ps.setInt(3, user.get_user_id());
                        return ps.executeUpdate() == 1;
                } catch (SQLException ex) {
                        Logger.getLogger(UserManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
        public Integer create_user(String user_name, String password) {
                Integer uid = m_uid_counter.get_next();
                if (uid == null) return uid;
                try {
                        PreparedStatement ps = m_conn.get_connection().prepareStatement(
                                "insert into user_manager (uid, alias, pwd) "
                                        + " values (" + uid + ",?,?);");
                        ps.setString(1, user_name);
                        ps.setString(2, password);
                        if (1 == ps.executeUpdate())
                                return uid;
                        else
                                return null;
                } catch (SQLException ex) {
                        Logger.getLogger(UserManager.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }
        
        public boolean remove_user(int user_id) {
                 try {
                        Statement s = m_conn.get_connection().createStatement();
                        s.executeUpdate("delete from user_manager where uid = " + user_id + ";");
                        return true;
                } catch (SQLException ex) {
                        Logger.getLogger(UserManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
}
