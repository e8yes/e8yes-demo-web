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

import org.e8yes.dbconnection.DatabaseConnectionInterface;
import org.e8yes.jwtprovider.JwtAlgorithmProviderInterface;

/** Returns global objects the runs in a deployment environment. */
public interface EnvironmentContextInterface {

  public enum Environment {
    Prod,
    Test
  }

  /**
   * The type of deployment environment it implements.
   *
   * @return Environment type.
   */
  public Environment environmentType();

  /**
   * Initialize the internal global objects.
   *
   * @throws Exception
   */
  public void init() throws Exception;

  /**
   * Clean up the internal global objects.
   *
   * @throws Exception
   */
  public void cleanUp() throws Exception;

  /**
   * Connections to the DemoWeb database server.
   *
   * @return the connections.
   */
  public DatabaseConnectionInterface demowebDbConnections();

  /**
   * JWT algorithm provider for the authorization.
   *
   * @return the provider.
   */
  public JwtAlgorithmProviderInterface authorizationJwtProvider();
}
