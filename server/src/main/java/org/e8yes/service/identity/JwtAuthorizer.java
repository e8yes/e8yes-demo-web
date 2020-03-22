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

import com.auth0.jwt.JWT;
import com.auth0.jwt.JWTVerifier;
import com.auth0.jwt.algorithms.Algorithm;
import com.auth0.jwt.exceptions.JWTDecodeException;
import com.auth0.jwt.exceptions.JWTVerificationException;
import com.auth0.jwt.interfaces.DecodedJWT;
import java.time.Instant;
import java.util.Arrays;
import java.util.Base64;
import java.util.Date;
import org.e8yes.exception.AccessDeniedException;
import org.mindrot.jbcrypt.BCrypt;

/** Establish and verify user identity via JWT. */
public class JwtAuthorizer {

  private static final String USER_ID_KEY = "I";

  private static final long EXPIRY_AFTER_MILLIS = 1000 * 60 * 5;

  public static class AuthorizationToken {
    public byte[] jwtToken;
  }

  /**
   * @param user
   * @param security_key
   * @param alg
   * @return
   * @throws AccessDeniedException
   */
  public static AuthorizationToken generateAuthorizationToken(
      UserEntity user, byte[] security_key, Algorithm alg) throws AccessDeniedException {
    if (!BCrypt.checkpw(Arrays.toString(security_key), user.security_key_hash.value())) {
      throw new AccessDeniedException();
    }

    long expiryTimestampMillis = Instant.now().plusMillis(EXPIRY_AFTER_MILLIS).toEpochMilli();
    String base64Token =
        JWT.create()
            .withClaim(USER_ID_KEY, user.id.value())
            .withExpiresAt(new Date(expiryTimestampMillis))
            .sign(alg);

    AuthorizationToken authToken = new AuthorizationToken();
    authToken.jwtToken = Base64.getDecoder().decode(base64Token);

    return authToken;
  }

  /**
   * @param jwtToken
   * @param verifier
   * @return
   * @throws JWTDecodeException
   */
  public static Identity parseToken(byte[] jwtToken, JWTVerifier verifier)
      throws JWTVerificationException {
    String base64Token = Base64.getEncoder().encodeToString(jwtToken);
    DecodedJWT decoded = verifier.verify(base64Token);

    long userId = decoded.getClaim(USER_ID_KEY).asLong();
    return new Identity(userId);
  }
}
