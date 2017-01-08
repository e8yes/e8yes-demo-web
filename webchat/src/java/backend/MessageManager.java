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
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author davis
 */
public class MessageManager {
        
        private DBConnector     m_conn;
        
        public MessageManager(DBConnector conn, FriendshipManager fs_mgr) {
                m_conn = conn;
                try {
                        Statement s = m_conn.get_connection().createStatement();
                        s.executeUpdate("create table if not exists message_manager("
                                + "uid_a integer,"
                                + "uid_b integer,"
                                + "t datetime,"
                                + "has_read boolean,"
                                + "msg text,"
                                + "primary key (uid_a, uid_b, t),"
                                + "foreign key (uid_a, uid_b) references " 
                                        + FriendshipManager.get_entity_name() + FriendshipManager.get_key_name() 
                                        + " on delete cascade);");
                } catch (SQLException ex) {
                        Logger.getLogger(MessageManager.class.getName()).log(Level.SEVERE, null, ex);
                }
        }
        
        public boolean new_message(Message msg) {
                Timestamp t = new Timestamp(msg.get_timestamp());
                try {
                        PreparedStatement ps = m_conn.get_connection().prepareStatement(
                                "insert into message_manager (uid_a, uid_b, t, has_read, msg) "
                                + " values (" + msg.sender() + "," 
                                              + msg.receiver() + "," 
                                              + "?," 
                                              + msg.has_read() + ","
                                              + "\"" + msg.get_content() + "\");");
                        ps.setTimestamp(1, t);
                        int r = ps.executeUpdate();
                        return r == 1;
                } catch (SQLException ex) {
                        Logger.getLogger(MessageManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
        public boolean clear_unread_state(int receiver) {
                try {
                        Statement s = m_conn.get_connection().createStatement();
                        int q = s.executeUpdate("update message_manager set has_read = true "
                                + " where has_read = false and uid_b = " + receiver);
                        return true;
                } catch (SQLException ex) {
                        Logger.getLogger(MessageManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
        public ArrayList<Message> get_messages(int uid_a, int uid_b, int n) {
                try {
                        Statement s = m_conn.get_connection().createStatement();
                        ResultSet result = s.executeQuery("select * from message_manager "
                                + "where (uid_a = " + uid_a + " and uid_b = " + uid_b + ") or "
                                      + "(uid_a = " + uid_b + " and uid_b = " + uid_a + ")"
                                + "order by t asc "
                                + "limit " + n + ";");
                        ArrayList<Message> msgs = new ArrayList<>();
                        while (result.next()) {
                                int sender = result.getInt("uid_a");
                                int receiver = result.getInt("uid_b");
                                long timestamp = result.getTimestamp("t").getTime();
                                boolean has_read = result.getBoolean("has_read");
                                String content = result.getString("msg");
                                msgs.add(new Message(sender, receiver, timestamp, has_read, content));
                        }
                        return msgs;
                } catch (SQLException ex) {
                        Logger.getLogger(MessageManager.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }
        
        public ArrayList<Message> get_received_unread_messages(int uid_receiver) {
                try {
                        Statement s = m_conn.get_connection().createStatement();
                        ResultSet result = s.executeQuery("select * from message_manager "
                                + " where has_read = false and uid_b = " + uid_receiver
                                + " order by t asc;");
                        ArrayList<Message> msgs = new ArrayList<>();
                        while (result.next()) {
                                int sender = result.getInt("uid_a");
                                int receiver = result.getInt("uid_b");
                                long timestamp = result.getTimestamp("t").getTime();
                                boolean has_read = result.getBoolean("has_read");
                                String content = result.getString("msg");
                                msgs.add(new Message(sender, receiver, timestamp, has_read, content));
                        }
                        return msgs;
                } catch (SQLException ex) {
                        Logger.getLogger(MessageManager.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }
}
