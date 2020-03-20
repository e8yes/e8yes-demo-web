package org.e8yes.service.identity.ctx;

import com.auth0.jwt.JWT;
import com.auth0.jwt.algorithms.Algorithm;
import com.auth0.jwt.interfaces.DecodedJWT;
import com.google.protobuf.InvalidProtocolBufferException;
import java.io.UnsupportedEncodingException;

/**
 * Defines the identity context of a user -- Who is that and what can it do?
 *
 * @author davis
 */
public class IdentityContext {

  public long userId;

  public IdentityContext(long userId) {
    this.userId = userId;
  }

  private static Integer[] toIntegerArray(byte[] bytes) {
    Integer[] res = new Integer[bytes.length];
    for (int i = 0; i < bytes.length; i++) {
      res[i] = (int) bytes[i];
    }
    return res;
  }

  private static byte[] toBytes(Integer[] integerArr) {
    byte[] res = new byte[integerArr.length];
    for (int i = 0; i < integerArr.length; i++) {
      res[i] = (byte) (int) integerArr[i];
    }
    return res;
  }

  public String sign(String secret) throws IllegalArgumentException, UnsupportedEncodingException {
    return JWT.create().withClaim("U", userId).sign(Algorithm.HMAC256(secret));
  }

  public static IdentityContext decode(String token) throws InvalidProtocolBufferException {
    DecodedJWT decoded = JWT.decode(token);
    return new IdentityContext(decoded.getClaim("U").asLong());
  }
}
