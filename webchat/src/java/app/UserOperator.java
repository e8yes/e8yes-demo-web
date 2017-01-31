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

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Objects;

/**
 *
 * @author davis
 */
public class UserOperator {
        
        public static Integer join(String user_name, String password) {
                backend.UserManager um = backend.SingletonEntities.get_user_manager();
                return um.create_user(user_name, password);
        }
        
        private static backend.User get_verified(int uid, String password) {
                backend.UserManager um = backend.SingletonEntities.get_user_manager();
                backend.User user = um.get_user(uid);
                if (user == null) return null;
                if (user.verify(password))
                        return user;
                else
                        return null;
        }
        
        public static backend.User verify(Integer uid, String password) {
                return get_verified(uid, password);
        }
        
        public static boolean change_password(int uid, String password, String new_password) {
                backend.UserManager um = backend.SingletonEntities.get_user_manager();
                backend.User user = get_verified(uid, password);
                if (user == null)
                        return false;
                user.change_password(new_password);
                return um.set_user(user);
        }
        
        public static boolean send_friend_request(backend.User user, int target, PrintWriter error) {
                if (user.get_user_id() == target) {
                        error.println("You wanna befriend yourself :)?");
                }
                backend.UserManager um = backend.SingletonEntities.get_user_manager();
                backend.User target_user = um.get_user(target);
                if (target_user == null) {
                        error.println("User ID " + target + " is not a valid Webchat user.");
                        return false;
                }
                backend.FriendshipManager fm = backend.SingletonEntities.get_friendship_manager();
                if (fm.has_friendship(user.get_user_id(), target)) {
                        error.println("Come on! You guys are friend already!");
                        return false;
                }
                backend.FriendRequestManager frm = backend.SingletonEntities.get_friend_request_manager();
		backend.FriendRequest fr = new backend.FriendRequest(user, target_user);
                if (!frm.create_friend_request(fr)) {
                        error.println("You have sent the request already.");
                        return false;
                }
		backend.Notifier notifier = backend.SingletonEntities.get_notifier();
		notifier.send_notification(fr);
                return true;
        }
        
        public static boolean confirm_friend_request(backend.User user, int target, PrintWriter error) {
                backend.UserManager um = backend.SingletonEntities.get_user_manager();
                backend.User target_user = um.get_user(target);
                if (target_user == null) {
                        error.println("User ID " + target + " is not a valid Webchat user.");
                        return false;
                }
                
                backend.FriendRequestManager frm = backend.SingletonEntities.get_friend_request_manager();
                backend.FriendshipManager fm = backend.SingletonEntities.get_friendship_manager();
                
		backend.FriendRequest fr = new backend.FriendRequest(target_user, user);
                if (frm.remove_request(fr)) {
                        fm.make_friend(user.get_user_id(), target);
			backend.Notifier notifier = backend.SingletonEntities.get_notifier();
			notifier.send_notification(new backend.FriendRequestConfirmation(fr));
                        return true;
                } else {
                        error.println("Good try.");
                        return false;
                }
        }
        
        public static boolean deny_friend_request(backend.User me, int target) {
                backend.UserManager um = backend.SingletonEntities.get_user_manager();
                backend.User target_user = um.get_user(target);
                if (target_user == null) {
                        return false;
                }
                
                backend.FriendRequestManager frm = backend.SingletonEntities.get_friend_request_manager();
                return frm.remove_request(new backend.FriendRequest(target_user, me));
        }
        
        public static ArrayList<backend.User> pull_friend_requests(int uid) {
                backend.FriendRequestManager frm = backend.SingletonEntities.get_friend_request_manager();
                return frm.request_to(uid);
        }
        
        public static ArrayList<backend.User> pull_friends(int uid) {
                backend.FriendshipManager fm = backend.SingletonEntities.get_friendship_manager();
                return fm.friends_of(uid);
        }
        
        public static Integer get_friend_id(int uid, int friend) {
                backend.FriendshipManager fm = backend.SingletonEntities.get_friendship_manager();
                return fm.get_friend_id(uid, friend);
        }
        
        public static boolean end_friendship(int uid, int friend) {
                backend.FriendshipManager fm = backend.SingletonEntities.get_friendship_manager();
                return fm.end_friendship(uid, friend);
        }
}
