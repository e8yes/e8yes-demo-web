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
import java.lang.reflect.InvocationTargetException;
import java.nio.ByteBuffer;
import java.sql.SQLException;
import org.e8yes.constant.FileStreamConstants;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.exception.ResourceMissingException;
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

  private static class FileAccessor {
    public FileHandleInterface file;
    public FileAccessLocation location;
  }

  private static FileAccessor validateAndOpenFile(
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

  /** File are read small chunk by small chunk to then be processed. */
  public static interface FileChunkListener {

    /**
     * Allows the listener to be ready for processing.
     *
     * @param metadata Metadata of the file to be processed.
     */
    public void onReady(FileEntity metadata);

    /** Telling the listener to finalize since the processing is complete. */
    public void onComplete();

    /**
     * The next chunk of data has arrived for processing.
     *
     * @param chunkData The data chunk to be processed.
     * @param chunkNumber Zero-offset chunk number identifying which chunk this data is in.
     */
    public void processNextChunk(ByteBuffer chunkData, int chunkNumber);
  }

  private static void readFile(
      FileAccessor accessor, ConnectionReservoirInterface dbConn, FileChunkListener listener)
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException, ResourceMissingException,
          IOException {
    FileEntity fileMeta = FileMetaData.retrieveFileMetadata(accessor.location, dbConn);
    if (fileMeta == null) {
      throw new ResourceMissingException("File at " + accessor.location + " doesn't exist.");
    }

    listener.onReady(fileMeta);

    int chunkNumber = 0;
    ByteBuffer buf = ByteBuffer.allocate(FileStreamConstants.CHUNK_SIZE_LIMIT);
    while (accessor.file.readNext(buf) > 0) {
      listener.processNextChunk(buf, chunkNumber);
      buf.clear();
      ++chunkNumber;
    }

    listener.onComplete();
  }

  /**
   * It bundles the file open operation with the read operation. Securely opens a file by validating
   * the access metadata specified by the file descriptor, then reads the file by chunk and pass the
   * chunk data to the listener for processing.
   *
   * @param viewer Viewer of the access location to validate against.
   * @param fileDesc FileDescriptor involving access metadata including access methods and access
   *     location.
   * @param fsProvider File system provider to perform the actual I/O.
   * @param jwtVerifier For token access validation.
   * @param dbConn Connection to the DB server.
   * @param listener Listen on the file metadata and chunk data.
   * @throws SQLException
   * @throws NoSuchMethodException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws IllegalArgumentException
   * @throws InvocationTargetException
   * @throws ResourceMissingException
   * @throws IOException
   * @throws JWTVerificationException
   * @throws AccessDeniedException
   */
  public static void readFile(
      Identity viewer,
      FileDescriptor fileDesc,
      FileSystemProviderInterface fsProvider,
      JWTVerifier jwtVerifier,
      ConnectionReservoirInterface dbConn,
      FileChunkListener listener)
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException, ResourceMissingException,
          IOException, JWTVerificationException, AccessDeniedException {
    FileIO.FileAccessor accessor =
        FileIO.validateAndOpenFile(
            viewer, fileDesc, FileAccessMode.FAM_READ, fsProvider, jwtVerifier, dbConn);

    readFile(accessor, dbConn, listener);

    fsProvider.close(accessor.file);
  }

  /** It takes in small chunks of data sequentially and write to the file system. */
  public static class FileChunkWriter {

    private final FileSystemProviderInterface fsProvider;
    private final JWTVerifier jwtVerifier;
    private final ConnectionReservoirInterface dbConn;

    private FileIO.FileAccessor accessor;
    private int bytesWritten = 0;

    private Identity lastViewer;
    private FileDescriptor lastFileDesc;

    /**
     * Initialize the writer with low-level interfaces.
     *
     * @param fsProvider File system provider to perform the actual I/O.
     * @param jwtVerifier For token access validation.
     * @param dbConn Connection to the DB server.
     */
    public FileChunkWriter(
        FileSystemProviderInterface fsProvider,
        JWTVerifier jwtVerifier,
        ConnectionReservoirInterface dbConn) {
      this.fsProvider = fsProvider;
      this.jwtVerifier = jwtVerifier;
      this.dbConn = dbConn;
    }

    /**
     * On the first call, it securely opens a file by validating the access metadata specified by
     * the file descriptor. Then, it appends the chunk data to the file. Note that, the viewer and
     * file descriptor must be the same as those appears in the first call.
     *
     * @param viewer Viewer of the access location to validate against.
     * @param desc FileDescriptor involving access metadata including access methods and access
     *     location.
     * @param chunkData The data chunk to be appended to the file specified by the file descriptor.
     * @throws JWTVerificationException
     * @throws AccessDeniedException
     * @throws IOException
     * @throws IllegalArgumentException
     * @throws SQLException
     * @throws IllegalAccessException
     */
    public void writeNextChunk(Identity viewer, FileDescriptor desc, ByteBuffer chunkData)
        throws JWTVerificationException, AccessDeniedException, IOException,
            IllegalArgumentException, SQLException, IllegalAccessException {
      if (accessor == null) {
        accessor =
            FileIO.validateAndOpenFile(
                viewer, desc, FileAccessMode.FAM_WRITE, fsProvider, jwtVerifier, dbConn);
        FileMetaData.addMetaDataForFile(accessor.location, desc.getEncryptionSource(), dbConn);

        lastViewer = viewer;
        lastFileDesc = desc;
      } else {
        if (lastViewer != viewer || lastFileDesc != desc) {
          throw new AccessDeniedException(
              "Viewer or file descriptor is different from the previous call.");
        }
      }
      bytesWritten += accessor.file.writeNext(chunkData);
    }

    /**
     * Closing active file handles and update the file's storage size metadata with the actual
     * number of bytes written.
     *
     * @throws IOException
     * @throws SQLException
     * @throws NoSuchMethodException
     * @throws InstantiationException
     * @throws IllegalAccessException
     * @throws IllegalArgumentException
     * @throws InvocationTargetException
     * @throws ResourceMissingException
     */
    public void close()
        throws IOException, SQLException, NoSuchMethodException, InstantiationException,
            IllegalAccessException, IllegalArgumentException, InvocationTargetException,
            ResourceMissingException {
      fsProvider.close(accessor.file);

      FileMetaData.updateFileSize(accessor.location, bytesWritten, dbConn);
    }
  }
}
