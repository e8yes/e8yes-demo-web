/**
 * e8yes demo web server.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */
package org.e8yes.constant;

public class DbTableConstants {

  public static String emailSetTable() {
    return "email_set";
  }

  public static String fileMetadataTable() {
    return "file_metadata";
  }

  public static String friendTable() {
    return "friend";
  }

  public static String messageTable() {
    return "message";
  }

  public static String messageIdSeq() {
    return "message_id_seq";
  }

  public static String messagingChannelTable() {
    return "messaging_channel";
  }

  public static String messagingChannelHasUserTable() {
    return "messaging_channel_has_user";
  }

  public static String userGroupTable() {
    return "user_group";
  }

  public static String userGroupHasFileTable() {
    return "user_group_has_file";
  }

  public static String userTable() {
    return "auser";
  }

  public static String userIdSeq() {
    return "auser_id_seq";
  }
}
