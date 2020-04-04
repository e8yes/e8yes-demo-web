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
package org.e8yes.service.file;

import com.auth0.jwt.JWTVerifier;
import com.auth0.jwt.exceptions.JWTVerificationException;
import java.io.IOException;
import java.sql.SQLException;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.fsprovider.FileAccessLocation;
import org.e8yes.fsprovider.FileHandleInterface;
import org.e8yes.fsprovider.FileSystemProviderInterface;
import org.e8yes.service.FileAccessMode;
import org.e8yes.service.FileDescriptor;
import org.e8yes.service.FileTokenAccess;
import org.e8yes.service.identity.Identity;
import org.e8yes.sql.connection.ConnectionReservoirInterface;

/** Module to manage file I/O securely and provides utilities for file streaming */
public class FileIO {

  public static class FileAccessor {
    public FileHandleInterface file;
    public FileAccessLocation location;
  }

  /**
   * Securely open a file by validating the access metadata specified by the file descriptor.
   *
   * @param viewer Viewer of the access location to validate against.
   * @param fileDesc FileDescriptor involving access metadata.
   * @param accessMode Access mode required for the access.
   * @param fsProvider File system provider to perform the actual I/O.
   * @param jwtVerifier For token access validation.
   * @param dbConn Connection to the DB server.
   * @return FileAccessor including the opened file handle.
   * @throws JWTVerificationException
   * @throws AccessDeniedException
   * @throws IOException
   * @throws SQLException
   */
  public static FileAccessor validateAndOpenFile(
      Identity viewer,
      FileDescriptor fileDesc,
      FileAccessMode accessMode,
      FileSystemProviderInterface fsProvider,
      JWTVerifier jwtVerifier,
      ConnectionReservoirInterface dbConn)
      throws JWTVerificationException, AccessDeniedException, IOException, IllegalArgumentException,
          SQLException {
    FileAccessLocation location;
    if (fileDesc.hasFileTokenAccess()) {
      FileTokenAccess tokenAccess = fileDesc.getFileTokenAccess();
      FileAccessValidator.FileAccessToken token = new FileAccessValidator.FileAccessToken();
      token.jwtToken = tokenAccess.getAccessToken().toByteArray();
      location = FileAccessValidator.validateTokenAccess(viewer, accessMode, token, jwtVerifier);
    } else if (fileDesc.hasFileDirectAccess()) {
      location = new FileAccessLocation(fileDesc.getFileDirectAccess().getPath());
      FileAccessValidator.validateDirectAccess(viewer, location, accessMode, dbConn);
    } else {
      throw new AccessDeniedException("Unknown access validation method.");
    }

    if (accessMode == FileAccessMode.FAM_WRITE) {
      fsProvider.createAndOverride(location);
    }
    FileHandleInterface file = fsProvider.open(location);

    FileAccessor info = new FileAccessor();
    info.file = file;
    info.location = location;

    return info;
  }
}
