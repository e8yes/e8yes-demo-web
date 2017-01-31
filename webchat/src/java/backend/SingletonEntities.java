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
public final class SingletonEntities {
        private static DBConnector DB_CONNECTOR;
        private static UserIDGenerator UID_COUNTER;
        private static FriendshipIDGenerator FID_COUNTER;
        private static UserManager USER_MANAGER;
        private static FriendshipManager FRIENDSHIP_MANAGER;
        private static FriendRequestManager FRIEND_REQUEST_MANAGER;
        private static MessageManager MESSAGE_MANAGER;
        private static CommunicationManager COMMUNICATION_MANAGER;
        private static Notifier NOTIFIER;
        
        public static void init() {
                DB_CONNECTOR = new DBConnector();
                UID_COUNTER = new UserIDGenerator(DB_CONNECTOR);
                FID_COUNTER = new FriendshipIDGenerator(DB_CONNECTOR);
                USER_MANAGER = new UserManager(DB_CONNECTOR, UID_COUNTER);
                FRIENDSHIP_MANAGER = new FriendshipManager(DB_CONNECTOR, USER_MANAGER, FID_COUNTER);
                FRIEND_REQUEST_MANAGER = new FriendRequestManager(DB_CONNECTOR, USER_MANAGER);
                MESSAGE_MANAGER = new MessageManager(DB_CONNECTOR, FRIENDSHIP_MANAGER);
                NOTIFIER = new Notifier();
                COMMUNICATION_MANAGER = new CommunicationManager(MESSAGE_MANAGER, NOTIFIER);
        }
	
	public static void destroy() {
		DB_CONNECTOR.destroy();
	}
        
        public static UserManager get_user_manager() {
                return USER_MANAGER;
        }
        
        public static FriendshipManager get_friendship_manager() {
                return FRIENDSHIP_MANAGER;
        }
        
        public static FriendRequestManager get_friend_request_manager() {
                return FRIEND_REQUEST_MANAGER;
        }
        
        public static MessageManager get_message_manager() {
                return MESSAGE_MANAGER;
        }
        
        public static CommunicationManager get_communication_manager() {
                return COMMUNICATION_MANAGER;
        }
        
        public static Notifier get_notifier() {
                return NOTIFIER;
        }
}
