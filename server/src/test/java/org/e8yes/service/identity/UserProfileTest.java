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

import com.auth0.jwt.exceptions.JWTVerificationException;
import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.Optional;
import org.e8yes.environment.EnvironmentContextInterface;
import org.e8yes.environment.Initializer;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.fsprovider.FileAccessLocation;
import org.e8yes.service.FileAccessMode;
import org.e8yes.service.UserPublicProfile;
import org.e8yes.service.file.FileAccessValidator;
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
  public void extractPublicInfoTest() throws JWTVerificationException, AccessDeniedException {
    UserEntity user = new UserEntity();
    user.id.assign(100L);
    user.alias.assign("user_alias");
    user.avatar_path.assign("/users/100/avatar/selfie.jpg");

    UserPublicProfile profile =
        UserProfile.extractPublicInfo(
            user, Initializer.environmentContext().authorizationJwtProvider().algorithm());
    Assertions.assertEquals(100L, profile.getUserId());
    Assertions.assertEquals("user_alias", profile.getAlias());
    Assertions.assertTrue(profile.hasAvatarReadonlyAccess());

    FileAccessValidator.FileAccessToken accessToken = new FileAccessValidator.FileAccessToken();
    accessToken.jwtToken = profile.getAvatarReadonlyAccess().getAccessToken().toByteArray();
    // Valid access.
    FileAccessLocation loc =
        FileAccessValidator.validateTokenAccess(
            new Identity(user),
            FileAccessMode.FAM_READ,
            accessToken,
            Initializer.environmentContext().authorizationJwtProvider().jwtverifier());
    Assertions.assertEquals("/users/100/avatar/selfie.jpg", loc.path);

    // Invalid write access.
    try {
      FileAccessValidator.validateTokenAccess(
          new Identity(user),
          FileAccessMode.FAM_WRITE,
          accessToken,
          Initializer.environmentContext().authorizationJwtProvider().jwtverifier());
      Assertions.fail();
    } catch (AccessDeniedException ex) {
      Assertions.assertTrue(true);
    }
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

  @Test
  public void setUpNewAvatarTest()
      throws SQLException, IllegalAccessException, JWTVerificationException, AccessDeniedException {
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();

    UserEntity user =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(123L), dbConn);
    UserProfile.AvatarSetup setup =
        UserProfile.setUpNewAvatar(
            user,
            /*fileName=*/ "/home/My Pictures/avatar/selfie.jpg",
            Initializer.environmentContext().authorizationJwtProvider().algorithm(),
            dbConn);

    Assertions.assertNotNull(user.avatar_path.value());

    // Verify avatar file access.
    FileAccessLocation loc =
        FileAccessValidator.validateTokenAccess(
            new Identity(user),
            FileAccessMode.FAM_READ,
            setup.avatarAccessToken,
            Initializer.environmentContext().authorizationJwtProvider().jwtverifier());
    Assertions.assertEquals("/users/123/avatar/selfie.jpg", loc.path);

    loc =
        FileAccessValidator.validateTokenAccess(
            new Identity(user),
            FileAccessMode.FAM_WRITE,
            setup.avatarAccessToken,
            Initializer.environmentContext().authorizationJwtProvider().jwtverifier());
    Assertions.assertEquals("/users/123/avatar/selfie.jpg", loc.path);
  }
}
