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
import java.util.Objects;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author davis
 */
public class FriendshipManager {
        
        private DBConnector             m_conn;
        private FriendshipIDGenerator   m_fid_gen;
        
        public static String get_entity_name() {
                return "friendship_manager";
        }
        
        public static String get_key_name() {
                return "(uid_a,uid_b)";
        }
        
        public FriendshipManager(DBConnector conn, UserManager user_mgr, FriendshipIDGenerator fid_gen) {
                m_conn = conn;
                m_fid_gen = fid_gen;
                try {
                        Statement s = m_conn.get_statement();
                        m_conn.process_update(s,
				"create table if not exists friendship_manager("
                                + "uid_a integer,"
                                + "uid_b integer,"
                                + "fid integer,"
                                + "primary key (uid_a, uid_b),"
                                + "foreign key (uid_a) references " 
                                        + UserManager.get_entity_name() + UserManager.get_key_name() + " on delete cascade,"
                                + "foreign key (uid_b) references "
                                        + UserManager.get_entity_name() + UserManager.get_key_name() + " on delete cascade);");
                } catch (SQLException ex) {
                        Logger.getLogger(FriendshipManager.class.getName()).log(Level.SEVERE, null, ex);
                }
        }
        
        private boolean _has_friendship(Integer uid_a, Integer uid_b) throws SQLException {
                Statement s = m_conn.get_statement();
                ResultSet result = m_conn.process_query(s, 
			  "select 1 from friendship_manager "
                        + "where (uid_a = " + uid_a + ") and (uid_b = " + uid_b + ");");
                return result.next();
        }
        
        public boolean has_friendship(Integer uid_a, Integer uid_b) {
                try {
                        return _has_friendship(uid_a, uid_b);
                } catch (SQLException ex) {
                        Logger.getLogger(FriendshipManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
        public boolean make_friend(Integer uid_a, Integer uid_b) {
                try {
                        if (_has_friendship(uid_a, uid_b))
                                return false;
                        Statement s = m_conn.get_statement();
                        // Friendship is symmetric.
                        int fid = m_fid_gen.get_next();
                        if (!Objects.equals(uid_a, uid_b)) {
                                int r = m_conn.process_update(s, 
				          "insert into friendship_manager "
                                        + "(uid_a, uid_b, fid) values (" + uid_a + "," + uid_b + "," + fid + "),"
                                                                   + "(" + uid_b + "," + uid_a + "," + fid + ");");
                                return r == 2;
                        } else {
                                int r = m_conn.process_update(s,
		                          "insert into friendship_manager "
                                        + "(uid_a, uid_b, fid) values (" + uid_a + "," + uid_b + "," + fid + ");");
                                return r == 1;
                        }
                } catch (SQLException ex) {
                        Logger.getLogger(FriendshipManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
        public boolean end_friendship(Integer uid_a, Integer uid_b) {
                try {
                        Statement s = m_conn.get_statement();
                        // Friendship is symmetric.
                        int r = m_conn.process_update(s,
				  "delete from friendship_manager "
                                + "where (uid_a = " + uid_a + " and uid_b = " + uid_b + ") or "
                                      + "(uid_a = " + uid_b + " and uid_b = " + uid_a + ");");
                        return r != 0;
                } catch (SQLException ex) {
                        Logger.getLogger(FriendshipManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
        public ArrayList<User> friends_of(Integer uid) {
                try {
                        Statement s = m_conn.get_statement();
                        ResultSet result = m_conn.process_query(s,
                                  " select UM." + UserManager.get_pk_name() + ","
                                + "        UM." + UserManager.get_alias_name() + " from "
                                + " (select FM.uid_b from friendship_manager FM where FM.uid_a = " + uid + ") as FS, "
                                + UserManager.get_entity_name() + " UM "
                                + " where FS.uid_b = UM." + UserManager.get_pk_name() + ";");
                        ArrayList<User> friends = new ArrayList<>();
                        while (result.next()) {
                                friends.add(new User(result.getInt(UserManager.get_pk_name()), 
                                                     result.getString(UserManager.get_alias_name())));
                        }
                        return friends;
                } catch (SQLException ex) {
                        Logger.getLogger(FriendshipManager.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }

        public Integer get_friend_id(int uid, int friend) {
                try {
                        Statement s = m_conn.get_statement();
                        ResultSet result = m_conn.process_query(s,
				  "select fid from friendship_manager "
                                + "where uid_a = " + uid + " and uid_b = " + friend + ";");
                        if (result.next()) {
                                return result.getInt("fid");
                        } else {
                                return null;
                        }
                } catch (SQLException ex) {
                        Logger.getLogger(FriendshipManager.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }
}
