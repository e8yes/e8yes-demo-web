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
import java.util.Optional;
import org.e8yes.environment.EnvironmentContextInterface;
import org.e8yes.environment.Initializer;
import org.e8yes.service.UserPublicProfile;
import org.e8yes.sql.connection.ConnectionReservoirInterface;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class UserProfileTest {

  @BeforeEach
  public void setUp() throws Exception {
    Initializer.init(EnvironmentContextInterface.Environment.Test);
  }

  @AfterEach
  public void tearDown() throws Exception {
    Initializer.cleanUp();
  }

  @Test
  public void extractPublicInfoTest() {
    UserEntity user = new UserEntity();
    user.id.assign(100L);
    user.alias.assign("user_alias");

    UserPublicProfile profile = UserProfile.extractPublicInfo(user);
    Assertions.assertEquals(100L, profile.getUserId());
    Assertions.assertEquals("user_alias", profile.getAlias());
  }

  @Test
  public void updateProfileTest()
      throws SQLException, IllegalAccessException, NoSuchMethodException, InstantiationException,
          IllegalArgumentException, InvocationTargetException {
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();

    UserEntity user =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(123L), dbConn);
    Assertions.assertNotNull(user);
    Assertions.assertNull(user.alias.value());

    user = UserProfile.updateProfile(user, /*alias=*/ Optional.of("new_alias"), dbConn);
    Assertions.assertNotNull(user.alias.value());
    Assertions.assertEquals("new_alias", user.alias.value());

    UserEntity retrievedUser = UserRetrieval.retrieveUserEntity(user.id.value(), dbConn);
    Assertions.assertNotNull(retrievedUser.alias.value());
    Assertions.assertEquals(user.alias, retrievedUser.alias);
  }
}
