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
import java.util.Collections;
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
                                + "fid integer,"
                                + "t timestamp(0) default current_timestamp,"
                                + "uid_b integer not null,"
                                + "uid_a integer not null,"
                                + "msg text,"
                                + "unique key (uid_b, uid_a, t),"
                                + "primary key (fid, t),"
                                + "foreign key (uid_a, uid_b) references " 
                                        + FriendshipManager.get_entity_name() + FriendshipManager.get_key_name() 
                                        + " on delete cascade) "
                                + " default character set=utf8;");
                        s.executeUpdate("create table if not exists unread_message_manager like message_manager;");
                        s.executeUpdate("alter table unread_message_manager add index (uid_b) using btree;");
                } catch (SQLException ex) {
                        Logger.getLogger(MessageManager.class.getName()).log(Level.SEVERE, null, ex);
                }
        }
        
        private boolean insert_message(Message msg, String entity) {
                Timestamp t = new Timestamp(msg.get_timestamp());
                try {
                        PreparedStatement ps = m_conn.get_connection().prepareStatement(
                                "insert into " + entity + " (uid_a, uid_b, fid, t, msg) "
                                + " values (" + msg.sender() + "," 
                                              + msg.receiver() + ","
                                              + msg.get_fid() + "," 
                                              + "?," 
                                              + "?);");
                        ps.setTimestamp(1, t);
                        ps.setString(2, msg.get_content());
                        int r = ps.executeUpdate();
                        return r == 1;
                } catch (SQLException ex) {
                        Logger.getLogger(MessageManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
        public boolean new_message(Message msg) {
                if (!msg.has_read())
                        return insert_message(msg, "unread_message_manager") &&
                               insert_message(msg, "message_manager");
                else
                        return insert_message(msg, "message_manager");
        }
        
        private boolean clear_unread_messages(int receiver, long timestamp) {
                Timestamp t = new Timestamp(timestamp);
                try {
                        PreparedStatement ps = m_conn.get_connection().prepareStatement(
                                "delete from unread_message_manager "
                                + " where uid_b = " + receiver + " and t <= ?;");
                        ps.setTimestamp(1, t);
                        int q = ps.executeUpdate();
                        return true;
                } catch (SQLException ex) {
                        Logger.getLogger(MessageManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
        private boolean clear_unread_messages(int uid) {
                try {
                        Statement s = m_conn.get_connection().createStatement();
                        s.executeUpdate("delete from unread_message_manager "
                                + " where uid_b = " + uid + ";");
                        return true;
                } catch (SQLException ex) {
                        Logger.getLogger(MessageManager.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
        public ArrayList<Message> pull_messages(int fid, int uid, int n) {
                try {
                        Statement s = m_conn.get_connection().createStatement();
                        ResultSet result = s.executeQuery("select * from message_manager "
                                + "where fid = " + fid
                                + " order by t desc "
                                + " limit " + n + ";");
                        if (!clear_unread_messages(uid))
                                return null;
                        ArrayList<Message> msgs = new ArrayList<>();
                        while (result.next()) {
                                int sender = result.getInt("uid_a");
                                int receiver = result.getInt("uid_b");
                                int afid = result.getInt("fid");
                                long timestamp = result.getTimestamp("t").getTime();
                                String content = result.getString("msg");
                                msgs.add(new Message(sender, receiver, afid, timestamp, true, content));
                        }
                        Collections.reverse(msgs);
                        return msgs;
                } catch (SQLException ex) {
                        Logger.getLogger(MessageManager.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }
        
        public ArrayList<Message> pull_unread_messages(int uid_receiver) {
                try {
                        Statement s = m_conn.get_connection().createStatement();
                        ResultSet result = s.executeQuery("select * from unread_message_manager "
                                + " where uid_b = " + uid_receiver
                                + " order by t asc;");
                        ArrayList<Message> msgs = new ArrayList<>();
                        while (result.next()) {
                                int sender = result.getInt("uid_a");
                                int receiver = result.getInt("uid_b");
                                int fid = result.getInt("fid");
                                long timestamp = result.getTimestamp("t").getTime();
                                String content = result.getString("msg");
                                msgs.add(new Message(sender, receiver, fid, timestamp, false, content));
                        }
                        if (!msgs.isEmpty())
                                if (!clear_unread_messages(uid_receiver, msgs.get(msgs.size() - 1).get_timestamp()))
                                        return null;
                        return msgs;
                } catch (SQLException ex) {
                        Logger.getLogger(MessageManager.class.getName()).log(Level.SEVERE, null, ex);
                        return null;
                }
        }
}
