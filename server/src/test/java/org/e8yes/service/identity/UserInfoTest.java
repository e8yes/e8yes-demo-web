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
import org.e8yes.connection.DatabaseConnection;
import org.e8yes.environment.EnvironmentContext;
import org.e8yes.environment.Initializer;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class UserInfoTest {

  @BeforeEach
  public void setUp() throws SQLException, IllegalAccessException {
    Initializer.init(new EnvironmentContext(EnvironmentContext.Mode.Test));
  }

  @AfterEach
  public void tearDown() throws SQLException {
    DatabaseConnection.deleteAllData();
  }

  @Test
  public void retrieveUserEntityTest()
      throws SQLException, IllegalAccessException, NoSuchMethodException, InstantiationException,
          IllegalArgumentException, InvocationTargetException {
    UserEntity user = UserCreation.createBaselineUser("PASS".getBytes());
    Assertions.assertNotNull(user);

    UserEntity retrieved = UserInfo.retrieveUserEntity(user.id.value());
    Assertions.assertEquals(user, retrieved);
  }
}
