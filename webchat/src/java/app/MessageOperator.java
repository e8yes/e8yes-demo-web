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
package app;

import java.util.ArrayList;

/**
 *
 * @author davis
 */
public class MessageOperator {
        
        public static boolean send_message(Integer sender, Integer receiver, String message) {
                backend.CommunicationManager cm = backend.SingletonEntities.get_communication_manager();
                return cm.send_message(new backend.Message(sender, receiver, message));
        }
        
        public static void create_realtime_channel(Integer uid, backend.MessageListener listener) {
                backend.CommunicationManager cm = backend.SingletonEntities.get_communication_manager();
                cm.register_channel(uid, listener);
        }
        
        public static void close_realtime_channel(Integer uid) {
                backend.CommunicationManager cm = backend.SingletonEntities.get_communication_manager();
                cm.close_channel(uid);
        }
        
        public static ArrayList<backend.Message> get_chat_history(Integer uid_a, Integer uid_b, Integer n) {
                backend.MessageManager mm = backend.SingletonEntities.get_message_manager();
                return mm.get_messages(uid_a, uid_b, n);
        }
}
