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

import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.List;
import org.e8yes.connection.DatabaseConnection;
import org.e8yes.constant.DbTableConstants;
import org.e8yes.sql.SqlQueryBuilder;
import org.e8yes.sql.SqlRunner;
import org.e8yes.sql.primitive.SqlLong;

/** Module to retrieve and manage information about a user. */
public class UserInfo {

  public static class UserEntityWrapper {
    public UserEntity entity;

    @Override
    public int hashCode() {
      return entity.hashCode();
    }

    @Override
    public boolean equals(Object obj) {
      final UserEntityWrapper other = (UserEntityWrapper) obj;
      return this.entity.equals(other.entity);
    }
  }

  /**
   * Retrieve user entity by user ID.
   *
   * @param userId ID of the user to be retrieved.
   * @return The user entity if exists.
   * @throws SQLException
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   */
  public static UserEntity retrieveUserEntity(long userId)
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException {
    SqlQueryBuilder.Placeholder<Long> userIdPh = new SqlQueryBuilder.Placeholder();

    SqlQueryBuilder query =
        new SqlQueryBuilder()
            .queryPiece(DbTableConstants.userTable())
            .queryPiece(" entity WHERE entity.id=")
            .placeholder(userIdPh);
    query.setPlaceholderValue(userIdPh, new SqlLong(userId));

    List<UserEntityWrapper> results =
        new SqlRunner()
            .withConnectionReservoir(DatabaseConnection.demoweb())
            .withEntity(UserEntityWrapper.class)
            .runQuery(query);
    if (results.isEmpty()) {
      return null;
    }
    assert (results.size() == 1);

    return results.get(0).entity;
  }
}
