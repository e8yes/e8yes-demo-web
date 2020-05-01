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
package org.e8yes.jwtprovider;

import com.auth0.jwt.JWTVerifier;
import com.auth0.jwt.algorithms.Algorithm;
import java.io.UnsupportedEncodingException;

/** Provides means of JWT token generation and verification */
public interface JwtAlgorithmProviderInterface {

  /**
   * For internal initialization.
   *
   * @throws UnsupportedEncodingException
   */
  public void init() throws IllegalArgumentException, UnsupportedEncodingException;

  /** Clean up internal resources. */
  public void cleanUp();

  /** @return JWT encryption algorithm. */
  public Algorithm algorithm();

  /** @return JWT token verifier. */
  public JWTVerifier jwtverifier();
}
