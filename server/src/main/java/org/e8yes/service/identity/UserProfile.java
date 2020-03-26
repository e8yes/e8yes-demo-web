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

import java.sql.SQLException;
import java.util.Optional;
import org.e8yes.constant.DbTableConstants;
import org.e8yes.service.UserPublicProfile;
import org.e8yes.sql.SqlRunner;
import org.e8yes.sql.connection.ConnectionReservoirInterface;

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

  /**
   * Update a user's profile.See below for what parameters are considered a user's profile.
   *
   * @param user Entity of the user whose profile needs to be updated.
   * @param alias Parameter of user's profile.
   * @param dbConn Connection to the DB server.
   * @return The updated user entity.
   * @throws SQLException
   * @throws IllegalAccessException
   */
  public static UserEntity updateProfile(
      UserEntity user, Optional<String> alias, ConnectionReservoirInterface dbConn)
      throws SQLException, IllegalAccessException {
    if (alias.isPresent()) {
      user.alias.assign(alias.get());
    }

    int numRowsUpdated =
        SqlRunner.runUpdate(
            user, DbTableConstants.userTable(), UserEntity.class, /*override=*/ true, dbConn);
    assert (numRowsUpdated == 1);

    return user;
  }
}
