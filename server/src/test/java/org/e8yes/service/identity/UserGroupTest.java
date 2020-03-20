package org.e8yes.service.identity;

import java.sql.SQLException;
import java.util.HashSet;
import java.util.Set;
import org.e8yes.connection.DatabaseConnection;
import org.e8yes.environment.EnvironmentContext;
import org.e8yes.environment.Initializer;
import org.e8yes.service.Permission;
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
  public void setUp() throws IllegalAccessException, SQLException {
    Initializer.init(new EnvironmentContext(EnvironmentContext.Mode.Test));
  }

  @AfterEach
  public void tearDown() throws SQLException {
    DatabaseConnection.deleteAllData();
  }

  @Test
  public void createUserGroupTest() throws IllegalAccessException, SQLException {
    String groupName = "TestUserGroup";
    Set<Permission> perms =
        new HashSet<Permission>() {
          {
            add(Permission.PERM_USER_GROUP_CREATE);
            add(Permission.PERM_USER_GROUP_REMOVE);
          }
        };
    UserGroup.UserGroupEntity group =
        UserGroup.createUserGroup(groupName, perms, /*replace=*/ false);
    Assertions.assertEquals(groupName, group.group_name.value());

    Assertions.assertEquals(2, group.permissions.value().length);
    Set<Integer> expectedPermStrs =
        new HashSet() {
          {
            add(Permission.PERM_USER_GROUP_CREATE.ordinal());
            add(Permission.PERM_USER_GROUP_REMOVE.ordinal());
          }
        };
    Set<Integer> actualPermStrs =
        new HashSet() {
          {
            add(group.permissions.value()[0]);
            add(group.permissions.value()[1]);
          }
        };
    Assertions.assertEquals(expectedPermStrs, actualPermStrs);
  }
}
