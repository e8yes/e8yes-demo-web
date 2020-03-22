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

import org.e8yes.environment.EnvironmentContextInterface;
import org.e8yes.environment.Initializer;
import org.e8yes.exception.AccessDeniedException;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class JwtAuthorizerTest {

  @BeforeEach
  public void setUp() throws Exception {
    Initializer.init(EnvironmentContextInterface.Environment.Test);
  }

  @AfterEach
  public void tearDown() throws Exception {
    Initializer.cleanUp();
  }

  @Test
  public void successfulSignAndParseTest() throws AccessDeniedException {
    UserEntity user = new UserEntity();
    user.id.assign(-5082222234282491904L);
    user.security_key_hash.assign("$2a$10$FyEIB.LOfYVTm/TckD9QoObpMf0gb21iD9w13Sx6ZX0cPqtVNf6Pq");

    byte[] securityKey = new byte[] {1, 1, 1, 1, 1};
    JwtAuthorizer.AuthorizationToken token =
        JwtAuthorizer.signAuthorizationToken(
            user,
            securityKey,
            Initializer.environmentContext().authorizationJwtProvider().algorithm());
    Identity identity =
        JwtAuthorizer.parseToken(
            token.jwtToken,
            Initializer.environmentContext().authorizationJwtProvider().jwtverifier());

    Assertions.assertEquals(-5082222234282491904L, identity.userId);
  }

  @Test
  public void accessDeniedSignatureTest() {
    UserEntity user = new UserEntity();
    user.id.assign(-5082222234282491904L);
    user.security_key_hash.assign("$2a$10$FyEIB.LOfYVTm/TckD9QoObpMf0gb21iD9w13Sx6ZX0cPqtVNf6Pq");

    byte[] securityKey = new byte[] {1, 1, 1, 1, 2};
    try {
      JwtAuthorizer.signAuthorizationToken(
          user,
          securityKey,
          Initializer.environmentContext().authorizationJwtProvider().algorithm());
      Assertions.fail("Shouldn't sign this token");
    } catch (AccessDeniedException ex) {
      Assertions.assertTrue(true);
    }
  }
}
