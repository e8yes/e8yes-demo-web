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
package org.e8yes.environment;

import com.auth0.jwt.JWT;
import com.auth0.jwt.JWTVerifier;
import com.auth0.jwt.algorithms.Algorithm;
import java.io.UnsupportedEncodingException;

/** */
public class JwtAlgorithmProvider {

  private static Algorithm encryptAlgorithm;
  private static JWTVerifier jwtVerifier;
  private static final String TEST_ENV_KEY =
      "68be2374ee7d4fb49dc86d2359f1592b3a366632322042509597252d5a1203c5";

  public static void initProvider() throws IllegalArgumentException, UnsupportedEncodingException {
    encryptAlgorithm = Algorithm.HMAC256(TEST_ENV_KEY);
    jwtVerifier = JWT.require(encryptAlgorithm).build();
  }

  public static Algorithm algorithm() {
    return encryptAlgorithm;
  }

  public static JWTVerifier jwtverifier() {
    return jwtVerifier;
  }
}
