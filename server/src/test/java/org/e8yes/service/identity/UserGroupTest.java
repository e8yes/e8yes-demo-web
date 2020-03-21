package org.e8yes.service.identity;

import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import org.e8yes.environment.EnvironmentContextInterface;
import org.e8yes.environment.Initializer;
import org.e8yes.service.Permission;
import org.e8yes.sql.connection.ConnectionReservoirInterface;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class UserGroupTest {

  public UserGroupTest() {}

  @BeforeAll
  public static void setUpClass() {}

  @AfterAll
  public static void tearDownClass() {}

  @BeforeEach
  public void setUp() throws Exception {
    Initializer.init(EnvironmentContextInterface.Environment.Test);
  }

  @AfterEach
  public void tearDown() throws Exception {
    Initializer.cleanUp();
  }

  @Test
  public void createUserGroupAndCheckPermissionsTest()
      throws IllegalAccessException, SQLException, NoSuchMethodException, InstantiationException,
          IllegalArgumentException, InvocationTargetException {
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();

    // Create a user group.
    String groupName = "TestUserGroup";
    Set<Permission> perms =
        new HashSet<Permission>() {
          {
            add(Permission.PERM_USER_GROUP_CREATE);
            add(Permission.PERM_USER_GROUP_REMOVE);
          }
        };
    UserGroup.UserGroupEntity group =
        UserGroup.createUserGroup(groupName, perms, /*replace=*/ false, dbConn);
    Assertions.assertEquals(groupName, group.group_name.value());

    Assertions.assertEquals(2, group.permissions.value().length);
    Set<Integer> expectedPermNos =
        new HashSet() {
          {
            add(Permission.PERM_USER_GROUP_CREATE.getNumber());
            add(Permission.PERM_USER_GROUP_REMOVE.getNumber());
          }
        };
    Set<Integer> actualPermNos =
        new HashSet() {
          {
            add(group.permissions.value()[0]);
            add(group.permissions.value()[1]);
          }
        };
    Assertions.assertEquals(expectedPermNos, actualPermNos);

    // Check if the created group is visible from a separate group retrieval call.
    List<UserGroup.UserGroupEntityWrapper> retrievedGroups =
        UserGroup.retrieveAllUserGroups(dbConn);
    int numInternalGroups = SystemUserGroup.values().length;
    Assertions.assertEquals(numInternalGroups + 1, retrievedGroups.size());
    Assertions.assertTrue(
        retrievedGroups.stream().map(g -> g.user_group).anyMatch(ug -> group.equals(ug)));

    // Check the new group's permission set.
    Set<Permission> actualPerms = UserGroup.getGroupPermissionSet(groupName, dbConn);
    Assertions.assertEquals(perms, actualPerms);
  }

  @Test
  public void deleteUserGroupTest()
      throws IllegalAccessException, SQLException, NoSuchMethodException, InstantiationException,
          IllegalArgumentException, InvocationTargetException {
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();

    UserGroup.UserGroupEntity group =
        UserGroup.createUserGroup(
            /*groupName=*/ "TestUserGroup", /*perms=*/ new HashSet(), /*replace=*/ false, dbConn);
    Assertions.assertNotNull(group);
    List<UserGroup.UserGroupEntityWrapper> retrievedGroups =
        UserGroup.retrieveAllUserGroups(dbConn);
    int numInternalGroups = SystemUserGroup.values().length;
    Assertions.assertEquals(numInternalGroups + 1, retrievedGroups.size());

    boolean deleted = UserGroup.deleteUserGroup(/*groupName=*/ "TestUserGroup", dbConn);
    Assertions.assertTrue(deleted);
    retrievedGroups = UserGroup.retrieveAllUserGroups(dbConn);
    Assertions.assertEquals(numInternalGroups + 0, retrievedGroups.size());
  }

  @Test
  public void changeUserGroupPermissionSetTest()
      throws IllegalAccessException, SQLException, NoSuchMethodException, InstantiationException,
          IllegalArgumentException, InvocationTargetException {
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();

    String groupName = "TestUserGroup";
    UserGroup.createUserGroup(groupName, /*perms=*/ new HashSet(), /*replace=*/ false, dbConn);

    // Should be empty.
    Set<Permission> perms = UserGroup.getGroupPermissionSet(groupName, dbConn);
    Assertions.assertTrue(perms.isEmpty());

    // Add 0 permission.
    UserGroup.addPermissionSetToGroup(groupName, new HashSet(), dbConn);
    perms = UserGroup.getGroupPermissionSet(groupName, dbConn);
    Assertions.assertTrue(perms.isEmpty());

    // Add 1 permission.
    UserGroup.addPermissionSetToGroup(
        groupName,
        new HashSet() {
          {
            add(Permission.PERM_USER_CREATE);
          }
        },
        dbConn);
    perms = UserGroup.getGroupPermissionSet(groupName, dbConn);
    Assertions.assertEquals(1, perms.size());
    Assertions.assertTrue(perms.stream().allMatch(p -> p == Permission.PERM_USER_CREATE));

    // Add 2 permissions with 1 overlaps with the above.
    UserGroup.addPermissionSetToGroup(
        groupName,
        new HashSet() {
          {
            add(Permission.PERM_USER_CREATE);
            add(Permission.PERM_USER_REMOVE);
          }
        },
        dbConn);
    perms = UserGroup.getGroupPermissionSet(groupName, dbConn);
    Assertions.assertEquals(2, perms.size());
    Assertions.assertTrue(perms.stream().anyMatch(p -> p == Permission.PERM_USER_CREATE));
    Assertions.assertTrue(perms.stream().anyMatch(p -> p == Permission.PERM_USER_REMOVE));

    // Remove 0 permission.
    UserGroup.deletePermissionSetFromGroup(groupName, new HashSet(), dbConn);
    perms = UserGroup.getGroupPermissionSet(groupName, dbConn);
    Assertions.assertEquals(2, perms.size());

    // Remove 2 permissions with one the group doesn't have.
    UserGroup.deletePermissionSetFromGroup(
        groupName,
        new HashSet() {
          {
            add(Permission.PERM_USER_CREATE);
            add(Permission.PERM_USER_SELF_CREATE);
          }
        },
        dbConn);
    perms = UserGroup.getGroupPermissionSet(groupName, dbConn);
    Assertions.assertEquals(1, perms.size());
    Assertions.assertTrue(perms.stream().anyMatch(p -> p == Permission.PERM_USER_REMOVE));
  }
}
