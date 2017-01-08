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

/**
 *
 * @author davis
 */
public class Message {
        private final int       m_user_a;
        private final int       m_user_b;
        private final int       m_fid;
        private boolean         m_has_read;
        private final long      m_timestamp;
        private final String    m_content;
        
        public Message(int sender, int receiver, int fid, long timestamp, boolean has_read, String content) {
                m_user_a = sender;
                m_user_b = receiver;
                m_fid = fid;
                m_content = content;
                m_has_read = has_read;
                m_timestamp = timestamp;
        }
                
        public Message(int sender, int receiver, int fid, String content) {
                m_user_a = sender;
                m_user_b = receiver;
                m_fid = fid;
                m_has_read = false;
                m_content = content;
                m_timestamp = System.currentTimeMillis();
        }
        
        public int sender() {
                return m_user_a;
        }
        
        public int receiver() {
                return m_user_b;
        }
        
        public int get_fid() {
                return m_fid;
        }
        
        public String get_content() {
                return m_content;
        }
        
        public long get_timestamp() {
                return m_timestamp;
        }
        
        public boolean has_read() {
                return m_has_read;
        }
        
        public void read() {
                m_has_read = true;
        }
        
        @Override
        public boolean equals(Object o) {
                Message other = (Message) o;
                return m_user_a == other.m_user_a && m_user_b == other.m_user_b && m_timestamp == other.m_timestamp;
        }

        @Override
        public int hashCode() {
                int hash = 7;
                hash = 53 * hash + this.m_user_a;
                hash = 53 * hash + this.m_user_b;
                hash = 53 * hash + (int) (this.m_timestamp ^ (this.m_timestamp >>> 32));
                return hash;
        }
        
        @Override
        public String toString() {
                return "Message = [" + m_user_a + "," + m_user_b + "," + m_timestamp + "," + m_content + "]";
        }
}
