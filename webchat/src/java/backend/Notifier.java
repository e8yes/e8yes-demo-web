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

import java.util.concurrent.ConcurrentHashMap;

/**
 * @author davis
 */
public class Notifier {
        
        private final ConcurrentHashMap<Integer, NotificationListener>  m_channels;
        
        public Notifier() {
                m_channels = new ConcurrentHashMap<>();
        }
        
        public void register_channel(int user_id, NotificationListener listener) {
                m_channels.put(user_id, listener);
        }
        
        public void close_channel(int user_id) {
                m_channels.remove(user_id);
        }
        
        public boolean send_notification(Notification noti) {
                NotificationListener channel = m_channels.get(noti.get_target_user());
                if (channel != null) {
                        return channel.on_receive(noti);
                } else
                        return false;
        }
}
