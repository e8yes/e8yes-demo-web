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
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import org.e8yes.constant.DbTableConstants;
import org.e8yes.service.Permission;
import org.e8yes.sql.SqlQueryBuilder;
import org.e8yes.sql.SqlRunner;
import org.e8yes.sql.connection.ConnectionReservoirInterface;
import org.e8yes.sql.primitive.SqlIntArr;
import org.e8yes.sql.primitive.SqlStr;

/** User group management. */
public class UserGroup {

  private static Set<Permission> superUserPerms() {
    Set<Permission> perms = new HashSet();
    perms.add(Permission.PERM_USER_GROUP_CREATE);
    perms.add(Permission.PERM_USER_GROUP_REMOVE);
    perms.add(Permission.PERM_USER_GROUP_VIEW);
    perms.add(Permission.PERM_USER_GROUP_ASSIGN);
    perms.add(Permission.PERM_USER_GROUP_UNASSIGN);
    perms.add(Permission.PERM_USER_SELF_CREATE);
    perms.add(Permission.PERM_USER_SELF_REMOVE);
    perms.add(Permission.PERM_USER_CREATE);
    perms.add(Permission.PERM_USER_REMOVE);
    return perms;
  }

  private static Set<Permission> baselineUserPerms() {
    Set<Permission> perms = new HashSet();
    perms.add(Permission.PERM_USER_GROUP_VIEW);
    perms.add(Permission.PERM_USER_SELF_CREATE);
    perms.add(Permission.PERM_USER_SELF_REMOVE);
    return perms;
  }

  /**
   * Creates and assigns permissions to the internally managed user groups.
   *
   * @param dbConn Connection to the DB server.
   * @throws IllegalAccessException
   * @throws SQLException
   */
  public static void createSystemUserGroups(ConnectionReservoirInterface dbConn)
      throws IllegalAccessException, SQLException {
    createUserGroup(
        SystemUserGroup.SUPER_USER_GROUP.name(), superUserPerms(), /*replace=*/ true, dbConn);
    createUserGroup(
        SystemUserGroup.BASELINE_USER_GROUP.name(), baselineUserPerms(), /*replace=*/ true, dbConn);
  }

  public static class UserGroupEntity {
    public SqlStr group_name = new SqlStr();
    public SqlIntArr permissions = new SqlIntArr();

    @Override
    public int hashCode() {
      return group_name.hashCode();
    }

    @Override
    public boolean equals(Object obj) {
      final UserGroupEntity other = (UserGroupEntity) obj;
      return this.group_name.equals(other.group_name);
    }
  }

  public static class UserGroupEntityWrapper {
    public UserGroupEntity user_group;

    @Override
    public int hashCode() {
      return user_group.hashCode();
    }

    @Override
    public boolean equals(Object obj) {
      final UserGroupEntityWrapper other = (UserGroupEntityWrapper) obj;
      return this.user_group.equals(other.user_group);
    }
  }

  /**
   * Obtain the set of assigned permissions to the user group.
   *
   * @param groupName Name of the user group to read permissions from.
   * @param dbConn Connection to the DB server.
   * @return The set of assigned permissions.
   * @throws SQLException
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   */
  public static Set<Permission> getGroupPermissionSet(
      String groupName, ConnectionReservoirInterface dbConn)
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException {
    SqlQueryBuilder.Placeholder<String> groupNamePh = new SqlQueryBuilder.Placeholder();
    SqlQueryBuilder query =
        new SqlQueryBuilder()
            .queryPiece(DbTableConstants.userGroupTable())
            .queryPiece(" user_group WHERE user_group.group_name=")
            .placeholder(groupNamePh);
    query.setPlaceholderValue(groupNamePh, new SqlStr(groupName));

    List<UserGroupEntityWrapper> groups =
        new SqlRunner()
            .withConnectionReservoir(dbConn)
            .withEntity(UserGroupEntityWrapper.class)
            .runQuery(query);

    if (groups.isEmpty()) {
      return null;
    }

    assert (groups.size() == 1);

    Set<Permission> result = new HashSet();
    Integer[] perms = groups.get(0).user_group.permissions.value();
    if (perms != null) {
      for (Integer p : perms) {
        result.add(Permission.forNumber(p));
      }
    }
    return result;
  }

  /**
   * Like this above, this function allows the checking of internally managed user group.
   *
   * @param group Internally managed user group to read permissions from.
   * @param dbConn Connection to the DB server.
   * @return The set of assigned permissions.
   * @throws SQLException
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   */
  public static Set<Permission> getGroupPermissionSet(
      SystemUserGroup group, ConnectionReservoirInterface dbConn)
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException {
    return UserGroup.getGroupPermissionSet(group.name(), dbConn);
  }

  /**
   * Add permissions to the specified user group.
   *
   * @param groupName Name of the user group to add permissions to.
   * @param perms The set of permissions to add to the user group.
   * @param dbConn Connection to the DB server.
   * @throws SQLException
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   */
  public static void addPermissionSetToGroup(
      String groupName, Set<Permission> perms, ConnectionReservoirInterface dbConn)
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException {
    Set<Permission> toBeUpdated = UserGroup.getGroupPermissionSet(groupName, dbConn);
    toBeUpdated.addAll(perms);
    UserGroup.createUserGroup(groupName, toBeUpdated, /*replace=*/ true, dbConn);
  }

  /**
   * Delete permissions from the specified user group.
   *
   * @param groupName Name of the user group to add permissions to.
   * @param perms The set of permissions to add to the user group.
   * @param dbConn Connection to the DB server.
   * @throws SQLException
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   */
  public static void deletePermissionSetFromGroup(
      String groupName, Set<Permission> perms, ConnectionReservoirInterface dbConn)
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException {
    Set<Permission> toBeUpdated = UserGroup.getGroupPermissionSet(groupName, dbConn);
    toBeUpdated.removeAll(perms);
    UserGroup.createUserGroup(groupName, toBeUpdated, /*replace=*/ true, dbConn);
  }

  /**
   * Create/update (depends on the "replace" argument) a user group.
   *
   * @param groupName Name of the new/existing user group.
   * @param perms Permissions that will be assigned to the user group.
   * @param replace If the replace mode is used, then this function will update an existing user
   *     group if possible. Otherwise, it will attempt to create a new user group.
   * @param dbConn Connection to the DB server.
   * @return If the replace mode is used, it will always return either a new user group or an
   *     existing one. Otherwise, it only returns a new user group if it could be created without
   *     conflict.
   * @throws IllegalAccessException
   * @throws SQLException
   */
  public static UserGroupEntity createUserGroup(
      String groupName, Set<Permission> perms, boolean replace, ConnectionReservoirInterface dbConn)
      throws IllegalAccessException, SQLException {
    SqlRunner runner = new SqlRunner().withConnectionReservoir(dbConn);

    UserGroupEntity userGroup = new UserGroupEntity();
    userGroup.group_name.assign(groupName);
    Integer[] permOrdinals = new Integer[perms.size()];
    int i = 0;
    for (Permission p : perms) {
      permOrdinals[i] = p.getNumber();
      ++i;
    }
    userGroup.permissions.assign(permOrdinals);

    int numRows =
        runner
            .withEntity(UserGroupEntity.class)
            .withOverrideRecord(replace)
            .runUpdate(userGroup, DbTableConstants.userGroupTable());
    if (!replace && numRows != 1) {
      return null;
    }

    return userGroup;
  }

  /**
   * Delete a user group.
   *
   * @param groupName Name of the user group to be deleted.
   * @param dbConn Connection to the DB server.
   * @return true only when a user group is successfully deleted. If the user group doesn't exist,
   *     it returns false.
   * @throws SQLException
   */
  public static boolean deleteUserGroup(String groupName, ConnectionReservoirInterface dbConn)
      throws SQLException {
    SqlQueryBuilder.Placeholder<String> groupNamePh = new SqlQueryBuilder.Placeholder();
    SqlQueryBuilder query =
        new SqlQueryBuilder().queryPiece("WHERE group_name=").placeholder(groupNamePh);
    query.setPlaceholderValue(groupNamePh, new SqlStr(groupName));

    int numRows =
        new SqlRunner()
            .withConnectionReservoir(dbConn)
            .runDelete(DbTableConstants.userGroupTable(), query);

    if (numRows == 0) {
      return false;
    } else {
      assert (numRows == 1);
      return true;
    }
  }

  /**
   * Retrieves all the user group entities.
   *
   * @param dbConn Connection to the DB server.
   * @return List of all user group entities.
   * @throws SQLException
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   */
  public static List<UserGroupEntityWrapper> retrieveAllUserGroups(
      ConnectionReservoirInterface dbConn)
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException {
    return new SqlRunner()
        .withConnectionReservoir(dbConn)
        .withEntity(UserGroupEntityWrapper.class)
        .runQuery(
            new SqlQueryBuilder()
                .queryPiece(DbTableConstants.userGroupTable())
                .queryPiece(" user_group"));
  }
}
