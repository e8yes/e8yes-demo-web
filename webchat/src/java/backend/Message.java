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
        private final int     m_user_a;
        private final int     m_user_b;
        private final String  m_content;
        
        public Message(int sender, int receiver, String content) {
                m_user_a = sender;
                m_user_b = receiver;
                m_content = content;
        }
        
        public int sender() {
                return m_user_a;
        }
        
        public int receiver() {
                return m_user_b;
        }
        
        public String get_content() {
                return m_content;
        }
}
