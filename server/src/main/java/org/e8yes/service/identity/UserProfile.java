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

import com.auth0.jwt.algorithms.Algorithm;
import com.google.protobuf.ByteString;
import java.nio.file.Path;
import java.sql.SQLException;
import java.util.Optional;
import org.e8yes.constant.DbTableConstants;
import org.e8yes.constant.RegexPatterns;
import org.e8yes.fsprovider.FileAccessLocation;
import org.e8yes.service.EncryptionSource;
import org.e8yes.service.FileAccessMode;
import org.e8yes.service.FileTokenAccess;
import org.e8yes.service.UserPublicProfile;
import org.e8yes.service.file.FileAccessValidator;
import org.e8yes.service.file.FileEntity;
import org.e8yes.service.file.FileMetaData;
import org.e8yes.sql.SqlRunner;
import org.e8yes.sql.connection.ConnectionReservoirInterface;

/** Manages user's public profile. */
public class UserProfile {

  /**
   * Extract public profile info from raw database entities.Public profile includes a user's alias
   * and a read only avatar path.
   *
   * @param user Raw database entity.
   * @param jwtAlg Algorithm for signing file access token.
   * @return The public profile.
   */
  public static UserPublicProfile extractPublicInfo(UserEntity user, Algorithm jwtAlg) {
    UserPublicProfile.Builder builder = UserPublicProfile.newBuilder().setUserId(user.id.value());

    if (user.alias.value() != null) {
      builder.setAlias(user.alias.value());
    }

    if (user.avatar_path.value() != null) {
      FileAccessValidator.FileAccessToken avatarAccessToken =
          FileAccessValidator.signAccessToken(
              new Identity(user),
              new FileAccessLocation(user.avatar_path.value()),
              FileAccessMode.FAM_READ,
              jwtAlg);
      FileTokenAccess avatarReadonlyAccess =
          FileTokenAccess.newBuilder()
              .setAccessToken(ByteString.copyFrom(avatarAccessToken.jwtToken))
              .build();
      builder.setAvatarReadonlyAccess(avatarReadonlyAccess);

      if (user.avatar_preview_path.value() != null) {
        FileAccessValidator.FileAccessToken avatarPreviewAccessToken =
            FileAccessValidator.signAccessToken(
                new Identity(user),
                new FileAccessLocation(user.avatar_preview_path.value()),
                FileAccessMode.FAM_READ,
                jwtAlg);
        FileTokenAccess avatarPreviewReadonlyAccess =
            FileTokenAccess.newBuilder()
                .setAccessToken(ByteString.copyFrom(avatarPreviewAccessToken.jwtToken))
                .build();
        builder.setAvatarPreviewReadonlyAccess(avatarPreviewReadonlyAccess);
      } else {
        // Use the avatar path as the preview path when preview doesn't exist.
        builder.setAvatarPreviewReadonlyAccess(avatarReadonlyAccess);
      }
    }

    return builder.build();
  }

  /**
   * Update a user's profile.See below for what parameters are considered a user's profile.
   *
   * @param user Entity of the user whose profile needs to be updated.
   * @param alias Parameter of user's profile.
   * @param dbConn Connection to the DB server.
   * @return The updated user entity.
   * @throws SQLException
   * @throws IllegalAccessException
   */
  public static UserEntity updateProfile(
      UserEntity user, Optional<String> alias, ConnectionReservoirInterface dbConn)
      throws SQLException, IllegalAccessException {
    if (alias.isPresent()) {
      user.alias.assign(alias.get());
    }

    int numRowsUpdated =
        SqlRunner.runUpdate(
            user, DbTableConstants.userTable(), UserEntity.class, /*override=*/ true, dbConn);
    assert (numRowsUpdated == 1);

    return user;
  }

  public static class AvatarSetup {
    public UserEntity updatedUser;
    public FileAccessValidator.FileAccessToken avatarAccessToken;
  }

  /**
   * Set up a systematically uniformed file path for user avatar and update the avatar path to the
   * specified user.
   *
   * @param user User to set avatar for.
   * @param fileName Name of the avatar file. If the supplied file name is a path, it will trim the
   *     path to find the file name
   * @param jwtAlg Algorithm for signing file access token.
   * @param dbConn Connection to the DB server.
   * @return The user with avatar path updated and the avatar's file access token with read/write
   *     access.
   * @throws IllegalArgumentException
   * @throws SQLException
   * @throws IllegalAccessException
   */
  public static AvatarSetup setUpNewAvatar(
      UserEntity user, String fileName, Algorithm jwtAlg, ConnectionReservoirInterface dbConn)
      throws IllegalArgumentException, SQLException, IllegalAccessException {
    // Create file metadata record.
    Path fileNamePath = Path.of(fileName).getFileName();
    if (fileNamePath == null
        || !RegexPatterns.FILE_NAME_PATTERN.matcher(fileNamePath.toString()).matches()) {
      throw new IllegalArgumentException(
          "The supplied path does not contain file name. Path=" + fileName);
    }
    FileAccessLocation location =
        new FileAccessLocation("/users/" + user.id.value() + "/avatar/" + fileNamePath);
    FileEntity avatarFile =
        FileMetaData.addMetaDataForFile(location, EncryptionSource.ESRC_NONE, dbConn);

    // Assign the avatar file record to the user.
    user.avatar_path.assign(avatarFile.path.value());
    int numRowsUpdated =
        SqlRunner.runUpdate(
            user, DbTableConstants.userTable(), UserEntity.class, /*override=*/ true, dbConn);
    assert (numRowsUpdated == 1);

    // Sign an access token.
    FileAccessValidator.FileAccessToken accessToken =
        FileAccessValidator.signAccessToken(
            new Identity(user),
            new FileAccessLocation(user.avatar_path.value()),
            FileAccessMode.FAM_READWRITE,
            jwtAlg);

    AvatarSetup result = new AvatarSetup();
    result.updatedUser = user;
    result.avatarAccessToken = accessToken;

    return result;
  }
}
