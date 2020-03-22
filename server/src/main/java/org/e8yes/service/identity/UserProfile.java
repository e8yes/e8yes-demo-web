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
package org.e8yes.service.identity;

import org.e8yes.service.UserPublicProfile;

/** Manages user's public profile. */
public class UserProfile {

  /**
   * Extract public profile info from raw database entities.
   *
   * @param user Raw database entity.
   * @return The public profile.
   */
  public static UserPublicProfile extractPublicInfo(UserEntity user) {
    UserPublicProfile.Builder builder = UserPublicProfile.newBuilder().setUserId(user.id.value());

    if (user.alias.value() != null) {
      builder.setAlias(user.alias.value());
    }

    return builder.build();
  }
}
