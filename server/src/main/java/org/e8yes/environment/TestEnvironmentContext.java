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
import org.e8yes.dbconnection.DemowebTestDatabaseConnection;
import org.e8yes.fsprovider.FileSystemProviderInterface;
import org.e8yes.fsprovider.LocalFileSystemProvider;
import org.e8yes.jwtprovider.FixedKeyJwtAlgorithmProvider;
import org.e8yes.jwtprovider.JwtAlgorithmProviderInterface;

/** Manages global objects for the test deployment environment. */
public class TestEnvironmentContext implements EnvironmentContextInterface {

  private final DatabaseConnectionInterface demowebDbConn = new DemowebTestDatabaseConnection();
  private final JwtAlgorithmProviderInterface jwtProvider = new FixedKeyJwtAlgorithmProvider();
  private final FileSystemProviderInterface fsProvider =
      new LocalFileSystemProvider(/*root=*/ "./demowebtestfs");

  @Override
  public void init() throws Exception {
    demowebDbConn.testConnection();
    demowebDbConn.clearAllTables();
    jwtProvider.init();
    fsProvider.init();
  }

  @Override
  public void cleanUp() throws Exception {
    demowebDbConn.clearAllTables();
    demowebDbConn.cleanUp();
    jwtProvider.cleanUp();
    fsProvider.cleanUp();
  }

  @Override
  public Environment environmentType() {
    return Environment.Test;
  }

  @Override
  public DatabaseConnectionInterface demowebDbConnections() {
    return demowebDbConn;
  }

  @Override
  public JwtAlgorithmProviderInterface authorizationJwtProvider() {
    return jwtProvider;
  }

  @Override
  public FileSystemProviderInterface fileSystemProvider() {
    return fsProvider;
  }
}
