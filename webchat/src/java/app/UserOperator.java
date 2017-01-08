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
public class UserOperator {
        
        public static Integer join(String password) {
                backend.UserManager um = backend.SingletonEntities.get_user_manager();
                return um.create_user(password);
        }
        
        private static backend.User get_verified(Integer uid, String password) {
                backend.UserManager um = backend.SingletonEntities.get_user_manager();
                backend.User user = um.get_user(uid);
                if (user == null) return null;
                if (user.verify(password))
                        return user;
                else
                        return null;
        }
        
        public static boolean verify(Integer uid, String password) {
                return get_verified(uid, password) != null;
        }
        
        public static boolean change_password(Integer uid, String password, String new_password) {
                backend.UserManager um = backend.SingletonEntities.get_user_manager();
                backend.User user = get_verified(uid, password);
                if (user == null)
                        return false;
                user.change_password(new_password);
                return um.set_user(user);
        }
        
        public static boolean send_friend_request(Integer uid, Integer target) {
                backend.FriendRequestManager frm = backend.SingletonEntities.get_friend_request_manager();
                return frm.create_friend_request(uid, target);
        }
        
        public static boolean confirm_friend_request(Integer uid, Integer target) {
                backend.FriendRequestManager frm = backend.SingletonEntities.get_friend_request_manager();
                backend.FriendshipManager fm = backend.SingletonEntities.get_friendship_manager();
                
                if (frm.remove_request(target, uid)) {
                        fm.make_friend(uid, target);
                        return true;
                } else {
                        return false;
                }
        }
        
        public static boolean deny_friend_request(Integer me, Integer target) {
                backend.FriendRequestManager frm = backend.SingletonEntities.get_friend_request_manager();
                return frm.remove_request(target, me);
        }
        
        public static ArrayList<Integer> pull_friend_requests(Integer uid) {
                backend.FriendRequestManager frm = backend.SingletonEntities.get_friend_request_manager();
                return frm.request_to(uid);
        }
        
        public static ArrayList<Integer> pull_friends(Integer uid) {
                backend.FriendshipManager fm = backend.SingletonEntities.get_friendship_manager();
                return fm.friends_of(uid);
        }
        
        public static boolean end_friendship(Integer uid, Integer friend) {
                backend.FriendshipManager fm = backend.SingletonEntities.get_friendship_manager();
                return fm.end_friendship(uid, friend);
        }
}
