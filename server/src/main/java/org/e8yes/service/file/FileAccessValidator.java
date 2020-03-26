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

import com.auth0.jwt.JWT;
import com.auth0.jwt.JWTVerifier;
import com.auth0.jwt.algorithms.Algorithm;
import com.auth0.jwt.exceptions.JWTVerificationException;
import com.auth0.jwt.interfaces.DecodedJWT;
import java.sql.SQLException;
import java.time.Instant;
import java.util.Date;
import org.e8yes.constant.DbTableConstants;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.jwtprovider.JwtUtil;
import org.e8yes.service.FileAccessMode;
import org.e8yes.service.identity.Identity;
import org.e8yes.sql.SqlQueryBuilder;
import org.e8yes.sql.SqlRunner;
import org.e8yes.sql.connection.ConnectionReservoirInterface;
import org.e8yes.sql.primitive.SqlStr;
import org.e8yes.sql.primitive.SqlStrArr;

/** Validates file access. */
public class FileAccessValidator {

  private static final String FILE_VIEWER_ID_KEY = "I";
  private static final String FILE_PATH_KEY = "P";
  private static final String FILE_ACCESS_MODE_KEY = "A";
  private static final long EXPIRY_AFTER_MILLIS = 1000 * 60 * 10;

  public static class FileAccessToken {
    byte[] jwtToken;
  }

  /**
   * Sign a token for a user to allow access to the specified file location.
   *
   * @param viewer Identity of the user who is allowed to use this token.
   * @param location File location this token is valid for.
   * @param accessMode Access mode of the location this token is valid for.
   * @param alg JWT signature algorithm.
   * @return An access token for the specified location.
   */
  public static FileAccessToken signAccessToken(
      Identity viewer, FileAccessLocation location, FileAccessMode accessMode, Algorithm alg) {
    long expiryTimestampMillis = Instant.now().plusMillis(EXPIRY_AFTER_MILLIS).toEpochMilli();
    String base64Token =
        JWT.create()
            .withClaim(FILE_VIEWER_ID_KEY, viewer.userId)
            .withClaim(FILE_PATH_KEY, location.path)
            .withClaim(FILE_ACCESS_MODE_KEY, accessMode.getNumber())
            .withExpiresAt(new Date(expiryTimestampMillis))
            .sign(alg);

    FileAccessToken accessToken = new FileAccessToken();
    accessToken.jwtToken = JwtUtil.convertToByteArray(base64Token);

    return accessToken;
  }

  /**
   * Validate access to a location using a specified access mode through a file access token.
   *
   * @param viewer Identity of the viewer to validate against.
   * @param accessMode Access mode to the file location the user wants to use.
   * @param accessToken The access token the viewer is holding.
   * @param verifier JWT verification algorithm.
   * @return Locate of the file the token permits access to.
   * @throws JWTVerificationException
   * @throws AccessDeniedException
   */
  public static FileAccessLocation validateTokenAccess(
      Identity viewer, FileAccessMode accessMode, FileAccessToken accessToken, JWTVerifier verifier)
      throws JWTVerificationException, AccessDeniedException {
    String jwtToken = JwtUtil.convertFromByteArray(accessToken.jwtToken);
    DecodedJWT decoded = verifier.verify(jwtToken);

    long tokenViewerId = decoded.getClaim(FILE_VIEWER_ID_KEY).asLong();
    if (viewer.userId != tokenViewerId) {
      throw new AccessDeniedException("The access token is for a different viewer.");
    }

    FileAccessMode tokenFileAccessMode =
        FileAccessMode.forNumber(decoded.getClaim(FILE_ACCESS_MODE_KEY).asInt());
    if (tokenFileAccessMode != accessMode && tokenFileAccessMode != FileAccessMode.FAM_READWRITE) {
      throw new AccessDeniedException(
          "Requires accessMode=" + accessMode + ", but got=" + tokenFileAccessMode);
    }

    return new FileAccessLocation(decoded.getClaim(FILE_PATH_KEY).asString());
  }

  /**
   * Check if the viewer has access right to the file location on the specified access mode based on
   * the user groups he is in.
   *
   * @param viewer The user identity to be checked against.
   * @param location Location the user wants to have access to.
   * @param accessMode Access mode of the location the user wants to use on the location.
   * @param dbConn Connection to the DB server.
   * @throws AccessDeniedException
   * @throws IllegalArgumentException
   * @throws SQLException
   */
  public static void validateDirectAccess(
      Identity viewer,
      FileAccessLocation location,
      FileAccessMode accessMode,
      ConnectionReservoirInterface dbConn)
      throws AccessDeniedException, IllegalArgumentException, SQLException {
    if (viewer.groupNames == null || viewer.groupNames.length == 0) {
      throw new AccessDeniedException("The user has participated zero user group.");
    }

    SqlQueryBuilder.Placeholder<SqlStrArr> participatedGroups = new SqlQueryBuilder.Placeholder();
    SqlQueryBuilder.Placeholder<SqlStr> filePath = new SqlQueryBuilder.Placeholder();

    SqlQueryBuilder query =
        new SqlQueryBuilder()
            .queryPiece(DbTableConstants.userGroupHasFileTable())
            .queryPiece(" WHERE file_path=")
            .placeholder(filePath)
            .queryPiece(" AND group_name=ANY(")
            .placeholder(participatedGroups)
            .queryPiece(")");
    switch (accessMode) {
      case FAM_READ:
        query.queryPiece(" AND can_read=TRUE");
        break;
      case FAM_WRITE:
        query.queryPiece(" AND can_write=TRUE");
        break;
      case FAM_READWRITE:
        query.queryPiece(" AND can_read=TRUE AND can_write=TRUE");
        break;
      default:
        throw new IllegalArgumentException("Doesn't expect accessMode=" + accessMode);
    }
    query.setPlaceholderValue(participatedGroups, new SqlStrArr(viewer.groupNames));
    query.setPlaceholderValue(filePath, new SqlStr(location.path));

    boolean hasAccess = SqlRunner.runExists(query, dbConn);
    if (!hasAccess) {
      throw new AccessDeniedException(
          "Access to location="
              + location
              + " on accessMode="
              + accessMode
              + " is not possible for user="
              + viewer);
    }
  }
}
