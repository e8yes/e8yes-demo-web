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

  private static final long EXPIRY_AFTER_MILLIS = 1000 * 60 * 10;

  /**
   * This is done to reduce the size of the JWT token, since byte array takes less space than base64
   * string.
   *
   * @param jwtToken JWT token encoded in base64 format.
   * @return JWT byte array.
   */
  private static byte[] convertToByteArray(String jwtToken) {
    // Split into header, payload and signature.
    String[] parts = jwtToken.split("\\.");
    assert (parts.length == 3);
    byte[] header = Base64.getUrlDecoder().decode(parts[0]);
    byte[] payload = Base64.getUrlDecoder().decode(parts[1]);
    byte[] signature = Base64.getUrlDecoder().decode(parts[2]);
    int headerLength = header.length;
    int payloadLength = payload.length;
    assert (headerLength < (1 << 8));
    assert (payloadLength < (1 << 8));
    byte[] finalToken = new byte[2 + header.length + payload.length + signature.length];
    // Write the segment length information into the first 2 bytes in little-endian format.
    finalToken[0] = (byte) headerLength;
    finalToken[1] = (byte) payloadLength;
    System.arraycopy(header, 0, finalToken, 2, headerLength);
    System.arraycopy(payload, 0, finalToken, 2 + headerLength, payloadLength);
    System.arraycopy(signature, 0, finalToken, 2 + headerLength + payloadLength, signature.length);
    return finalToken;
  }

  /**
   * JWT verifier has to ingest the standard base64 string format. It converts from a compact byte
   * array back to the standard base64 format.
   *
   * @param jwtBytes Compact byte array.
   * @return standard base64 format.
   */
  private static String convertFromByteArray(byte[] jwtBytes) {
    int headerLength = jwtBytes[0];
    int payloadLength = jwtBytes[1];
    int signatureLength = jwtBytes.length - headerLength - payloadLength - 2;

    byte[] header = new byte[headerLength];
    byte[] payload = new byte[payloadLength];
    byte[] signature = new byte[signatureLength];
    System.arraycopy(jwtBytes, 2, header, 0, headerLength);
    System.arraycopy(jwtBytes, 2 + headerLength, payload, 0, payloadLength);
    System.arraycopy(jwtBytes, 2 + headerLength + payloadLength, signature, 0, signature.length);

    String b64Header = Base64.getUrlEncoder().withoutPadding().encodeToString(header);
    String b64Payload = Base64.getUrlEncoder().withoutPadding().encodeToString(payload);
    String b64Signature = Base64.getUrlEncoder().withoutPadding().encodeToString(signature);

    return String.format("%s.%s.%s", b64Header, b64Payload, b64Signature);
  }

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
            .withExpiresAt(new Date(expiryTimestampMillis))
            .sign(alg);

    AuthorizationToken authToken = new AuthorizationToken();
    authToken.jwtToken = convertToByteArray(base64Token);

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
    String base64Token = convertFromByteArray(jwtToken);
    DecodedJWT decoded = verifier.verify(base64Token);

    long userId = decoded.getClaim(USER_ID_KEY).asLong();
    return new Identity(userId);
  }
}
