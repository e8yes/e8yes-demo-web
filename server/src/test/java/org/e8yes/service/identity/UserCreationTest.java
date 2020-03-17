/**
 * e8yes demo web server.
 *
 * <p>Copyright (C) 2020 Yichen Ma {yichenm2@uci.edu}, Chifeng Wen {daviesx66@gmail.com}
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
import org.apache.ibatis.session.SqlSession;
import org.e8yes.connection.DatabaseConnection;
import org.e8yes.environment.EnvironmentContext;
import org.e8yes.environment.Initializer;
import org.e8yes.exception.ResourceConflictException;
import org.e8yes.service.EtUser;
import org.e8yes.service.identity.dao.mappers.AUserMapper;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class UserTest {

  public UserTest() {}

  @BeforeAll
  public static void setUpClass() {}

  @AfterAll
  public static void tearDownClass() {}

  @BeforeEach
  public void setUp() throws SQLException {
    Initializer.init(new EnvironmentContext(EnvironmentContext.Mode.Test));
  }

  @AfterEach
  public void tearDown() throws SQLException {
    DatabaseConnection.deleteAllData();
  }

  @Test
  public void createUserTest() throws ResourceConflictException {
    String user0Name = "USER0";
    String user0Alias = "";
    String user0PassWord = "PASS";
    EtUser user =
        User.createBaselineUser(user0Name, user0Alias, user0PassWord, UserGroup.getContext());
    user.getUserName();
    Assertions.assertNotEquals(0, user.getId());
    Assertions.assertEquals(user0Name, user.getUserName());
    Assertions.assertEquals(user0Alias, user.getAlias());
    Assertions.assertNotNull(user.getPasscode());

    SqlSession sess = DatabaseConnection.openSession();
    AUserMapper mapper = sess.getMapper(AUserMapper.class);
    EtUser loadedUser = mapper.loadByIdOrUserName(user.getId(), null);
    Assertions.assertNotNull(loadedUser);
    Assertions.assertEquals(loadedUser, user);

    Assertions.assertTrue(User.userNameExists(user0Name));

    DatabaseConnection.closeSession(sess);
  }

  @Test
  public void createRepeatedUserTest() throws ResourceConflictException {
    String user0Name = "USER0";
    String user0Alias = "";
    String user0PassWord = "PASS";
    User.createBaselineUser(user0Name, user0Alias, user0PassWord, UserGroup.getContext());
    Assertions.assertThrows(
        ResourceConflictException.class,
        () -> {
          User.createBaselineUser(user0Name, user0Alias, user0PassWord, UserGroup.getContext());
        });
  }
}
