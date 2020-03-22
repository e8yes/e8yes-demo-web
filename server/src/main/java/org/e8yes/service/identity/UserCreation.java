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
import java.util.Arrays;
import java.util.Date;
import java.util.HashSet;
import java.util.Set;
import org.e8yes.constant.DbTableConstants;
import org.e8yes.sql.SqlRunner;
import org.e8yes.sql.connection.ConnectionReservoirInterface;
import org.mindrot.jbcrypt.BCrypt;

/** Static class for user creation. */
public class UserCreation {

  /**
   * Create a root user who has all the permissions so it can bootstrap the permission configuration
   * process.
   *
   * @param dbConn Connection to the DB server.
   */
  public static void createRootUser(ConnectionReservoirInterface dbConn) {}

  /**
   * Create a user of arbitrary group.
   *
   * @param securityKey Security key associated with new user.
   * @param userGroupNames Name of the groups the user will be in.
   * @param dbConn Connection to the DB server.
   * @return A newly created user with its associated unique ID.
   * @throws SQLException
   * @throws IllegalAccessException
   */
  public static UserEntity createUser(
      byte[] securityKey, Set<String> userGroupNames, ConnectionReservoirInterface dbConn)
      throws SQLException, IllegalAccessException {
    SqlRunner runner = new SqlRunner().withConnectionReservoir(dbConn);

    UserEntity user = new UserEntity();
    user.id.assign(runner.timeId());
    user.id_str.assign(Long.toString(user.id.value()));
    // Stores a irreversibly hashed security key.
    user.security_key_hash.assign(BCrypt.hashpw(Arrays.toString(securityKey), BCrypt.gensalt()));
    user.group_names.assign(userGroupNames.toArray(new String[userGroupNames.size()]));
    user.active_level.assign(0);
    user.created_at.assign(new Date());

    int numRowUpdated =
        runner.withEntity(UserEntity.class).runUpdate(user, DbTableConstants.userTable());
    assert (numRowUpdated == 1);

    return user;
  }

  /**
   * Similar to the above, but the new user will be in the system's baseline user group.
   *
   * @param securityKey Security key associated with new user.
   * @return A newly created user with its associated unique ID.
   * @param dbConn Connection to the DB server.
   * @throws SQLException
   * @throws IllegalAccessException
   */
  public static UserEntity createBaselineUser(
      byte[] securityKey, ConnectionReservoirInterface dbConn)
      throws SQLException, IllegalAccessException {
    return createUser(
        securityKey,
        new HashSet() {
          {
            add(SystemUserGroup.BASELINE_USER_GROUP.name());
          }
        },
        dbConn);
  }
}
