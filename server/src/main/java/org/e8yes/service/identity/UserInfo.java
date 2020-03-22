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
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import org.e8yes.constant.DbTableConstants;
import org.e8yes.service.Pagination;
import org.e8yes.sql.SqlQueryBuilder;
import org.e8yes.sql.SqlRunner;
import org.e8yes.sql.connection.ConnectionReservoirInterface;
import org.e8yes.sql.primitive.SqlInt;
import org.e8yes.sql.primitive.SqlLong;
import org.e8yes.sql.primitive.SqlStr;

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
   * @param dbConns Connection to the DemoWeb DB server.
   * @return The user entity if exists.
   * @throws SQLException
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   */
  public static UserEntity retrieveUserEntity(long userId, ConnectionReservoirInterface dbConns)
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
            .withConnectionReservoir(dbConns)
            .withEntity(UserEntityWrapper.class)
            .runQuery(query);
    if (results.isEmpty()) {
      return null;
    }
    assert (results.size() == 1);

    return results.get(0).entity;
  }

  /**
   * Search for user entities on optionally different fields.
   *
   * @param userIdPrefix Filter by the prefix of the userId.
   * @param aliasPrefix Filter by the prefix of the alias.
   * @param pagination Pagination constraint.
   * @param dbConn Connection to the DemoWeb DB server.
   * @return A list of users that satisfy the specified filters.
   * @throws SQLException
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   */
  public static List<UserEntity> searchUserEntity(
      Optional<Long> userIdPrefix,
      Optional<String> aliasPrefix,
      Pagination pagination,
      ConnectionReservoirInterface dbConn)
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException {
    SqlQueryBuilder.Placeholder<String> userIdPrefixPh = new SqlQueryBuilder.Placeholder();
    SqlQueryBuilder.Placeholder<String> aliasPrefixPh = new SqlQueryBuilder.Placeholder();

    SqlQueryBuilder.Placeholder<Integer> limitPh = new SqlQueryBuilder.Placeholder();
    SqlQueryBuilder.Placeholder<Integer> offsetPh = new SqlQueryBuilder.Placeholder();

    SqlQueryBuilder query =
        new SqlQueryBuilder()
            .queryPiece(DbTableConstants.userTable())
            .queryPiece(" entity WHERE TRUE");
    if (userIdPrefix.isPresent()) {
      query.queryPiece(" AND entity.id_str=").placeholder(userIdPrefixPh).queryPiece("%");
      query.setPlaceholderValue(userIdPrefixPh, new SqlStr(Long.toString(userIdPrefix.get())));
    }
    if (aliasPrefix.isPresent()) {
      query.queryPiece(" AND entity.alias=").placeholder(aliasPrefixPh).queryPiece("%");
      query.setPlaceholderValue(aliasPrefixPh, new SqlStr(aliasPrefix.get()));
    }
    query.queryPiece(" LIMIT ").placeholder(limitPh).queryPiece(" OFFSET ").placeholder(offsetPh);
    query.setPlaceholderValue(limitPh, new SqlInt(pagination.getResultPerPage()));
    query.setPlaceholderValue(
        offsetPh, new SqlInt(pagination.getPageNumber() * pagination.getResultPerPage()));

    List<UserEntityWrapper> queryResults =
        new SqlRunner()
            .withConnectionReservoir(dbConn)
            .withEntity(UserEntityWrapper.class)
            .runQuery(query);
    List<UserEntity> users = new ArrayList();
    for (UserEntityWrapper wrapper : queryResults) {
      users.add(wrapper.entity);
    }
    return users;
  }
}
