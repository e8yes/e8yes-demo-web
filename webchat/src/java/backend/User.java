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
public class User {
        private final int       m_user_id;
        private String          m_password;
        
        public User(int user_id, String password) {
                m_user_id = user_id;
                m_password = password;
        }
        
        public boolean verify(String password) {
                return m_password.equals(password);
        }
        
        public void change_password(String password) {
                m_password = password;
        }
        
        public int get_user_id() {
                return m_user_id;
        }
        
        public String get_password() {
                return m_password;
        }
        
        @Override
        public boolean equals(Object o) {
                User other = (User) o;
                return m_user_id == other.m_user_id;
        }

        @Override
        public int hashCode() {
                int hash = 3;
                hash = 59 * hash + this.m_user_id;
                return hash;
        }
        
        @Override
        public String toString() {
                return "User = [" + m_user_id + "," + m_password + "]";
        }
}
