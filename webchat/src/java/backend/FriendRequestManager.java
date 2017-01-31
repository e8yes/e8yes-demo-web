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
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author davis
 */
public class FriendRequestManager {
        
        private DBConnector     m_conn;
        
        public FriendRequestManager(DBConnector conn, UserManager user_mgr) {
                m_conn = conn;
                try {
                        Statement s = m_conn.get_statement();
                        m_conn.process_update(s, 
			          "create table if not exists friend_request_manager("
                                + "uid_a integer,"
                                + "uid_b integer,"
                                + "primary key (uid_a, uid_b),"
                                + "foreign key (uid_a) references " 
                                        + UserManager.get_entity_name() + UserManager.get_key_name() + " on delete cascade,"
                                + "foreign key (uid_b) references "
                                        + UserManager.get_entity_name() + UserManager.get_key_name() + " on delete cascade);");
                } catch (SQLException ex) {
                        Logger.getLogger(FriendRequestManager.class.getName()).log(Level.SEVERE, null, ex);
                }
        }
        
        private boolean has_request(int uid_a, int uid_b) throws SQLException {
                Statement s = m_conn.get_statement();
                ResultSet result = m_conn.process_query(s,
			  "select 1 from friend_request_manager "
                        + "where (uid_a = " + uid_a + ") and (uid_b = " + uid_b + ");");
                return result.next();
        }
        
        public boolean create_friend_request(FriendRequest request) {
                try {
                        if (has_request(request.sender().get_user_id(), 
                                        request.receiver().get_user_id()))
                                return false;
                        Statement s = m_conn.get_statement();
                        // Friend request is anti-symmetric.
                        int r = m_conn.process_update(s,
				  "insert into friend_request_manager "
                                + "(uid_a, uid_b) values (" + request.sender().get_user_id() + "," 
                                                            + request.receiver().get_user_id() + ");");
                        return r == 1;
                } catch (SQLException ex) {
                        Logger.getLogger(FriendRequestManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
        public boolean remove_request(FriendRequest request) {
                try {
                        Statement s = m_conn.get_statement();
                        // Friend request is anti-symmetric.
                        int r = m_conn.process_update(s,
				  "delete from friend_request_manager "
                                + "where (uid_a = " + request.sender().get_user_id() 
                                  + " and uid_b = " + request.receiver().get_user_id() + ");");
                        return r != 0;
                } catch (SQLException ex) {
                        Logger.getLogger(FriendRequestManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
        public ArrayList<User> request_from(Integer uid) {
                try {
                        Statement s = m_conn.get_statement();
                        ResultSet result = m_conn.process_query(s,
                                  " select UM." + UserManager.get_pk_name() + ", UM." + UserManager.get_alias_name() + " from "
                                + " (select FRM.uid_b from friend_request_manager FRM where FRM.uid_a = " + uid + ") as FR, "
                                + UserManager.get_entity_name() + " UM "
                                + " where FR.uid_b = UM." + UserManager.get_pk_name() + ";");
                        ArrayList<User> requests = new ArrayList<>();
                        while (result.next()) {
                                requests.add(new User(result.getInt(UserManager.get_pk_name()), 
                                                      result.getString(UserManager.get_alias_name())));
                        }
                        return requests;
                } catch (SQLException ex) {
                        Logger.getLogger(FriendRequestManager.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }
        
        public ArrayList<User> request_to(Integer uid) {
                try {
                        Statement s = m_conn.get_statement();
                        ResultSet result = m_conn.process_query(s,
                                  " select UM." + UserManager.get_pk_name() + ", UM." + UserManager.get_alias_name() + " from "
                                + " (select FRM.uid_a from friend_request_manager FRM where FRM.uid_b = " + uid + ") as FR, "
                                + UserManager.get_entity_name() + " UM "
                                + " where FR.uid_a = UM." + UserManager.get_pk_name() + ";");
                        ArrayList<User> requests = new ArrayList<>();
                        while (result.next()) {
                                requests.add(new User(result.getInt(UserManager.get_pk_name()), 
                                                      result.getString(UserManager.get_alias_name())));
                        }
                        return requests;
                } catch (SQLException ex) {
                        Logger.getLogger(FriendRequestManager.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }
}
