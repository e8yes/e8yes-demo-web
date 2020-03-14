package org.e8yes.service.identity;

import java.sql.SQLException;
import java.util.HashSet;
import java.util.Set;
import org.apache.ibatis.session.SqlSession;
import org.e8yes.connection.DatabaseConnection;
import org.e8yes.environment.EnvironmentContext;
import org.e8yes.environment.Initializer;
import org.e8yes.service.EtUserGroup;
import org.e8yes.service.Permission;
import org.e8yes.service.identity.dao.mappers.AUserGroupMapperEx;
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
  public void setUp() {
    Initializer.init(new EnvironmentContext(EnvironmentContext.Mode.Test));
  }

  @AfterEach
  public void tearDown() throws SQLException {
    DatabaseConnection.deleteAllData();
  }

  @Test
  public void createUserGroupTest() {
    String groupName = "TestUserGroup";
    Set<Permission> perms =
        new HashSet<Permission>() {
          {
            add(Permission.PERM_USER_GROUP_CREATE);
            add(Permission.PERM_USER_GROUP_REMOVE);
          }
        };
    EtUserGroup group = UserGroup.createUserGroup(groupName, perms);
    Assertions.assertTrue(group.getId() != 0);
    Assertions.assertEquals(groupName, group.getDescription());
    Assertions.assertArrayEquals(perms.toArray(), group.getPermissionsList().toArray());

    SqlSession sess = DatabaseConnection.openSession();
    EtUserGroup loadedGroup = AUserGroupMapperEx.loadById(sess, group.getId());
    Assertions.assertNotNull(loadedGroup);
    Assertions.assertEquals(group.getId(), loadedGroup.getId());
    Assertions.assertEquals(groupName, loadedGroup.getDescription());

    DatabaseConnection.closeSession(sess);
  }
}
