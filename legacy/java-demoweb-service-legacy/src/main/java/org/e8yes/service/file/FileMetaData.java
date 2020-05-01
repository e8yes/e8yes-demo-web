package org.e8yes.service.file;

import java.lang.reflect.InvocationTargetException;
import java.nio.file.Path;
import java.sql.SQLException;
import java.util.Date;
import java.util.List;
import org.e8yes.constant.DbTableConstants;
import org.e8yes.exception.ResourceMissingException;
import org.e8yes.fsprovider.FileAccessLocation;
import org.e8yes.service.EncryptionSource;
import org.e8yes.service.FileAccessMode;
import org.e8yes.service.FileDescriptor;
import org.e8yes.service.FileFormat;
import org.e8yes.service.identity.UserGroupEntity;
import org.e8yes.sql.SqlQueryBuilder;
import org.e8yes.sql.SqlRunner;
import org.e8yes.sql.connection.ConnectionReservoirInterface;
import org.e8yes.sql.primitive.SqlStr;

/**
 * Manages file metadata including access rights, format and size, etc. associated with each file
 * location
 */
public class FileMetaData {

  public static FileFormat fileFormatFromPath(String path) throws IllegalArgumentException {
    path = path.toLowerCase();
    if (path.endsWith(".jpeg") || path.endsWith(".jpg")) {
      return FileFormat.FFMT_IMAGE_JPEG;
    } else if (path.endsWith(".png")) {
      return FileFormat.FFMT_IMAGE_PNG;
    } else if (path.endsWith(".mp4")) {
      return FileFormat.FFMT_VIDEO_MPEG4;
    } else if (path.endsWith(".gif")) {
      return FileFormat.FFMT_VIDEO_GIF;
    } else if (path.endsWith(".ogv")) {
      return FileFormat.FFMT_VIDEO_OGV;
    } else if (path.endsWith(".mp3")) {
      return FileFormat.FFMT_AUDIO_MP3;
    } else if (path.endsWith(".ogg")) {
      return FileFormat.FFMT_AUDIO_OGG;
    } else {
      return FileFormat.FFMT_UNKNOWN;
    }
  }

  /**
   * Attach new metadata for the file (it overrides the old metadata).
   *
   * @param location Location of the file to be attached new metadata.
   * @param encryptionSource Encryption key source used to encrypt the file.
   * @param dbConn Connection to the DB server.
   * @return The newly attached metadata.
   * @throws IllegalArgumentException
   * @throws SQLException
   * @throws IllegalAccessException
   */
  public static FileEntity addMetaDataForFile(
      FileAccessLocation location,
      EncryptionSource encryptionSource,
      ConnectionReservoirInterface dbConn)
      throws IllegalArgumentException, SQLException, IllegalAccessException {
    FileEntity file = new FileEntity();
    file.path.assign(location.path);
    file.storage_size.assign(0L);
    FileFormat format = fileFormatFromPath(location.path);
    file.format.assign(format.getNumber());
    file.encryption_key_source.assign(encryptionSource.getNumber());
    Date curTimeStamp = new Date();
    file.created_at.assign(curTimeStamp);
    file.last_modified_at.assign(curTimeStamp);

    int numRows =
        SqlRunner.runUpdate(
            file,
            DbTableConstants.fileMetadataTable(),
            FileEntity.class,
            /*override=*/ true,
            dbConn);
    assert (numRows == 1);

    return file;
  }

  public static class FileEntityWrapper {
    public FileEntity file_meta;
  }

  /**
   * Retrieves the metadata associated with the file.
   *
   * @param location Location of the file whose metadata to be retrieved.
   * @param dbConn Connection to the DB server.
   * @return File metadata retrieved.
   * @throws SQLException
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   */
  public static FileEntity retrieveFileMetadata(
      FileAccessLocation location, ConnectionReservoirInterface dbConn)
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException {
    SqlQueryBuilder.Placeholder<SqlStr> path = new SqlQueryBuilder.Placeholder();
    SqlQueryBuilder query =
        new SqlQueryBuilder()
            .queryPiece(DbTableConstants.fileMetadataTable())
            .queryPiece(" file_meta WHERE file_meta.path=")
            .placeholder(path);
    query.setPlaceholderValue(path, new SqlStr(location.path));

    List<FileEntityWrapper> result = SqlRunner.runQuery(query, FileEntityWrapper.class, dbConn);
    if (result.isEmpty()) {
      return null;
    }

    assert (result.size() == 1);
    return result.get(0).file_meta;
  }

  /**
   * Creates file descriptor builder and pre-fill values from entity. Access methods are ignored.
   *
   * @param entity File entity used to pre-fill the file descriptor builder.
   * @return The pre-filled builder.
   */
  public static FileDescriptor.Builder createFileDescriptorBuilder(FileEntity entity) {
    return FileDescriptor.newBuilder()
        .setFileName(Path.of(entity.path.value()).getFileName().toString())
        .setFileFormat(FileFormat.forNumber(entity.format.value()))
        .setEncryptionSource(EncryptionSource.forNumber(entity.encryption_key_source.value()))
        .setCreatedAtTimestamp(entity.created_at.value().getTime())
        .setLastModifiedAtTimestamp(entity.last_modified_at.value().getTime())
        .setStorageSize(entity.storage_size.value());
  }

  /**
   * Updates the file size as well as the last modified date.
   *
   * @param location Location of the file to be modified.
   * @param storageSize New storage size of the file.
   * @param dbConn Connection to the DB server.
   * @return
   * @throws SQLException
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   * @throws ResourceMissingException
   */
  public static FileEntity updateFileSize(
      FileAccessLocation location, long storageSize, ConnectionReservoirInterface dbConn)
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException, ResourceMissingException {
    FileEntity file = retrieveFileMetadata(location, dbConn);
    if (file == null) {
      throw new ResourceMissingException("file with path=" + location.path + " doesn't exist.");
    }

    file.storage_size.assign(storageSize);
    file.last_modified_at.assign(new Date());

    int numRows =
        SqlRunner.runUpdate(
            file,
            DbTableConstants.fileMetadataTable(),
            FileEntity.class,
            /*override=*/ true,
            dbConn);
    assert (numRows == 1);

    return file;
  }

  /**
   * Assigns a file location to a user group which allows users of that group to have the specified
   * access to the file location.
   *
   * @param location location of the file to assign.
   * @param userGroup user group to be assigned the file location.
   * @param accessMode the type of access mode this assignment allows.
   * @param dbConn Connection to the DB server.
   * @throws SQLException
   * @throws IllegalAccessException
   */
  public static void addFileLocationToGroup(
      FileAccessLocation location,
      UserGroupEntity userGroup,
      FileAccessMode accessMode,
      ConnectionReservoirInterface dbConn)
      throws SQLException, IllegalAccessException {
    UserGroupHasFileEntity fileGroup = new UserGroupHasFileEntity();
    fileGroup.group_name.assign(userGroup.group_name.value());
    fileGroup.file_path.assign(location.path);
    switch (accessMode) {
      case FAM_READ:
        fileGroup.can_read.assign(true);
        fileGroup.can_write.assign(false);
        break;
      case FAM_WRITE:
        fileGroup.can_write.assign(true);
        fileGroup.can_read.assign(false);
        break;
      case FAM_READWRITE:
        fileGroup.can_read.assign(true);
        fileGroup.can_write.assign(true);
        break;
      default:
        throw new IllegalArgumentException("Unknown accessMode=" + accessMode);
    }

    int numRows =
        SqlRunner.runUpdate(
            fileGroup,
            DbTableConstants.userGroupHasFileTable(),
            UserGroupHasFileEntity.class,
            /*override=*/ true,
            dbConn);
    assert (numRows == 1);
  }
}
