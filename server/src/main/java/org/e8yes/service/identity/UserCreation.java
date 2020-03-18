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
import org.e8yes.connection.DatabaseConnection;
import org.e8yes.constant.DbTableConstants;
import org.e8yes.service.EtUserGroup;
import org.e8yes.service.identity.ctx.AUserGroupContext;
import org.e8yes.sql.SqlRunner;
import org.e8yes.sql.primitive.SqlDate;
import org.e8yes.sql.primitive.SqlInt;
import org.e8yes.sql.primitive.SqlLong;
import org.e8yes.sql.primitive.SqlStr;
import org.e8yes.sql.primitive.SqlStrArr;
import org.mindrot.jbcrypt.BCrypt;

/** Static class for user creation. */
public class UserCreation {

  public static void init(AUserGroupContext userGroupCtx) {}

  public static class UserEntity {
    public SqlLong id = new SqlLong();
    public SqlStrArr emails = new SqlStrArr();
    public SqlStr alias = new SqlStr();
    public SqlLong avatar_file_id = new SqlLong();
    public SqlStr security_key_hash = new SqlStr();
    public SqlInt group_id = new SqlInt();
    public SqlInt active_level = new SqlInt();
    public SqlDate created_at = new SqlDate();
  }

  private static UserEntity createUser(byte[] securityKey, EtUserGroup userGroup)
      throws SQLException, IllegalAccessException {
    SqlRunner runner = new SqlRunner().withConnectionReservoir(DatabaseConnection.demoweb());

    UserEntity user = new UserEntity();
    user.id.assign(runner.timeId());
    user.security_key_hash.assign(BCrypt.hashpw(Arrays.toString(securityKey), BCrypt.gensalt()));
    user.group_id.assign(userGroup.getId());
    user.active_level.assign(0);
    user.created_at.assign(new Date());

    int numRowUpdated =
        runner.withEntity(UserEntity.class).runUpdate(user, DbTableConstants.userTable());
    assert (numRowUpdated == 1);

    return user;
  }

  public static UserEntity createBaselineUser(byte[] securityKey, AUserGroupContext userGroupCtx)
      throws SQLException, IllegalAccessException {
    return createUser(
        securityKey,
        userGroupCtx.getSystemUserGroup(AUserGroupContext.SystemUserGroup.BASELINE_USER_GROUP));
  }
}
