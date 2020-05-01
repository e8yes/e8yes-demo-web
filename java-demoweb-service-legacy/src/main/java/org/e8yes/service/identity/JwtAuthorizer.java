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
import java.util.Date;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.jwtprovider.JwtUtil;
import org.mindrot.jbcrypt.BCrypt;

/** Establish and verify user identity via JWT. */
public class JwtAuthorizer {

  private static final String USER_ID_KEY = "I";
  private static final String USER_GROUP_KEY = "G";

  private static final long EXPIRY_AFTER_MILLIS = 1000 * 60 * 10;

  public static class AuthorizationToken {
    public byte[] jwtToken;
  }

  /**
   * Verify user's security key and sign an authorization token representing the user's identity in
   * byte array format.
   *
   * @param user The user to verify against.
   * @param security_key The security key used to verify the agent.
   * @param alg JWT signature algorithm.
   * @return An authorization token representing the user's identity.
   * @throws AccessDeniedException
   */
  public static AuthorizationToken signAuthorizationToken(
      UserEntity user, byte[] security_key, Algorithm alg) throws AccessDeniedException {
    if (!BCrypt.checkpw(Arrays.toString(security_key), user.security_key_hash.value())) {
      throw new AccessDeniedException();
    }

    long expiryTimestampMillis = Instant.now().plusMillis(EXPIRY_AFTER_MILLIS).toEpochMilli();
    String base64Token =
        JWT.create()
            .withClaim(USER_ID_KEY, user.id.value())
            .withArrayClaim(USER_GROUP_KEY, user.group_names.value())
            .withExpiresAt(new Date(expiryTimestampMillis))
            .sign(alg);

    AuthorizationToken authToken = new AuthorizationToken();
    authToken.jwtToken = JwtUtil.convertToByteArray(base64Token);

    return authToken;
  }

  /**
   * Parse and verify a JWT token in byte array format and establish an identity for the agent.
   *
   * @param jwtToken token to be verify.
   * @param verifier JWT verification algorithm.
   * @return The agent's user identity.
   * @throws JWTDecodeException
   */
  public static Identity parseToken(byte[] jwtToken, JWTVerifier verifier)
      throws JWTVerificationException {
    String base64Token = JwtUtil.convertFromByteArray(jwtToken);
    DecodedJWT decoded = verifier.verify(base64Token);

    long userId = decoded.getClaim(USER_ID_KEY).asLong();
    String[] groupNames = decoded.getClaim(USER_GROUP_KEY).asArray(String.class);
    return new Identity(userId, groupNames);
  }
}
