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

import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;

/**
 *
 * @author davis
 */
public class CommunicationManager {
        
        private final ConcurrentHashMap<Integer, MessageListener>       m_channels;
        private final MessageManager                                    m_mgr;
        
        public CommunicationManager(MessageManager mgr) {
                m_channels = new ConcurrentHashMap<>();
                m_mgr = mgr;
        }
        
        public void register_channel(int user_id, MessageListener listener) {
                m_channels.put(user_id, listener);

                // Check any unread messages.
                ArrayList<Message> msgs = m_mgr.pull_unread_messages(user_id);
                msgs.stream().forEach(listener::on_receive);
                listener.start();
        }
        
        public void close_channel(int user_id) {
                m_channels.remove(user_id);
        }
        
        public boolean send_message(Message msg) {
                MessageListener channel = m_channels.get(msg.receiver());
                if (channel != null) {
                        if (channel.on_receive(msg)) {
                                msg.read();
                        }
                }
                return m_mgr.new_message(msg);
        }
}
