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
package org.e8yes.service;

import com.auth0.jwt.exceptions.JWTVerificationException;
import com.google.protobuf.ByteString;
import io.grpc.Status;
import io.grpc.stub.StreamObserver;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.nio.ByteBuffer;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.e8yes.constant.GrpcContexts;
import org.e8yes.environment.Initializer;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.exception.ResourceMissingException;
import org.e8yes.fsprovider.FileAccessLocation;
import org.e8yes.fsprovider.FileHandleInterface;
import org.e8yes.fsprovider.FileSystemProviderInterface;
import org.e8yes.jwtprovider.JwtAlgorithmProviderInterface;
import org.e8yes.service.file.FileAccessValidator;
import org.e8yes.service.file.FileAccessValidator.FileAccessToken;
import org.e8yes.service.file.FileEntity;
import org.e8yes.service.file.FileMetaData;
import org.e8yes.service.identity.Identity;

/**
 * Handle file upload. Validate identity then store the data chunk in sequence to the file system.
 */
class UploadStreamObserver implements StreamObserver<UploadFileRequest> {

  private final StreamObserver<UploadFileResponse> res;
  private FileService.FileInfo fileInfo;
  private int fileSize = 0;

  public UploadStreamObserver(StreamObserver<UploadFileResponse> res) {
    this.res = res;
  }

  @Override
  public void onNext(UploadFileRequest request) {
    if (fileInfo == null) {
      try {
        fileInfo =
            FileService.validateIdentityAndOpenFile(
                request.getFileDescriptor(), FileAccessMode.FAM_WRITE);

        FileMetaData.addMetaDataForFile(
            fileInfo.location,
            request.getFileDescriptor().getEncryptionSource(),
            Initializer.environmentContext().demowebDbConnections().connectionReservoir());
      } catch (IllegalArgumentException | SQLException | IllegalAccessException | IOException ex) {
        Logger.getLogger(UploadStreamObserver.class.getName()).log(Level.SEVERE, null, ex);
        res.onError(Status.INTERNAL.withDescription(ex.getMessage()).asException());
        return;
      } catch (AccessDeniedException | JWTVerificationException ex) {
        Logger.getLogger(UploadStreamObserver.class.getName()).log(Level.SEVERE, null, ex);
        res.onError(Status.UNAUTHENTICATED.withDescription(ex.getMessage()).asException());
        return;
      }
    }

    try {
      fileSize +=
          fileInfo.file.writeNext(request.getCurrentChunk().getData().asReadOnlyByteBuffer());
      res.onNext(UploadFileResponse.newBuilder().build());
    } catch (IOException ex) {
      Logger.getLogger(UploadStreamObserver.class.getName()).log(Level.SEVERE, null, ex);
      res.onError(Status.INTERNAL.withDescription(ex.getMessage()).asException());
    }
  }

  @Override
  public void onError(Throwable ex) {
    res.onError(ex);
  }

  @Override
  public void onCompleted() {
    try {
      FileSystemProviderInterface fsProvider =
          Initializer.environmentContext().fileSystemProvider();
      fsProvider.close(fileInfo.file);

      FileMetaData.updateFileSize(
          fileInfo.location,
          fileSize,
          Initializer.environmentContext().demowebDbConnections().connectionReservoir());

      res.onCompleted();
    } catch (SQLException
        | NoSuchMethodException
        | InstantiationException
        | IllegalAccessException
        | IllegalArgumentException
        | InvocationTargetException
        | ResourceMissingException
        | IOException ex) {
      Logger.getLogger(UploadStreamObserver.class.getName()).log(Level.SEVERE, null, ex);
      res.onError(Status.INTERNAL.withDescription(ex.getMessage()).asException());
    }
  }
}

/** Service for file management. */
public class FileService extends FileServiceGrpc.FileServiceImplBase {

  private final int DOWNLOAD_CHUNK_SIZE = 32 * 1000;

  public static class FileInfo {
    public FileHandleInterface file;
    public FileAccessLocation location;
  }

  /**
   * Utility function to securely open a file.
   *
   * @param fileDesc FileDescriptor involving access metadata.
   * @param accessMode Access mode required for the access.
   * @return The opened file handle.
   * @throws JWTVerificationException
   * @throws AccessDeniedException
   * @throws IOException
   */
  public static FileInfo validateIdentityAndOpenFile(
      FileDescriptor fileDesc, FileAccessMode accessMode)
      throws JWTVerificationException, AccessDeniedException, IOException {
    Identity viewer = GrpcContexts.IDENTITY_CONTEXT_KEY.get();

    JwtAlgorithmProviderInterface jwtProvider =
        Initializer.environmentContext().authorizationJwtProvider();

    FileAccessLocation location;
    if (fileDesc.hasFileTokenAccess()) {
      FileTokenAccess tokenAccess = fileDesc.getFileTokenAccess();
      FileAccessToken token = new FileAccessToken();
      token.jwtToken = tokenAccess.getAccessToken().toByteArray();
      location =
          FileAccessValidator.validateTokenAccess(
              viewer, accessMode, token, jwtProvider.jwtverifier());
    } else if (fileDesc.hasFileDirectAccess()) {
      location = new FileAccessLocation(fileDesc.getFileDirectAccess().getPath());
      FileAccessValidator.signAccessToken(viewer, location, accessMode, jwtProvider.algorithm());
    } else {
      throw new AccessDeniedException("Unknown access validation method.");
    }

    FileSystemProviderInterface fsProvider = Initializer.environmentContext().fileSystemProvider();
    if (accessMode == FileAccessMode.FAM_WRITE) {
      fsProvider.createAndOverride(location);
    }
    FileHandleInterface file = fsProvider.open(location);

    FileInfo info = new FileInfo();
    info.file = file;
    info.location = location;

    return info;
  }

  @Override
  public StreamObserver<UploadFileRequest> upload(StreamObserver<UploadFileResponse> res) {
    return new UploadStreamObserver(res);
  }

  @Override
  public void download(DownloadFileRequest request, StreamObserver<DownloadFileResponse> res) {
    try {
      FileInfo info =
          validateIdentityAndOpenFile(request.getFileDescriptor(), FileAccessMode.FAM_READ);

      FileEntity fileMeta =
          FileMetaData.retrieveFileMetadata(
              info.location,
              Initializer.environmentContext().demowebDbConnections().connectionReservoir());

      FileDescriptor.Builder desc = FileMetaData.createFileDescriptorBuilder(fileMeta);
      desc.setFileDirectAccess(request.getFileDescriptor().getFileDirectAccess());
      desc.setFileTokenAccess(request.getFileDescriptor().getFileTokenAccess());

      DownloadFileResponse.Builder respBuilder = DownloadFileResponse.newBuilder();
      respBuilder.setFileDescriptor(desc.build());

      int chunkNumber = -1;
      FileChunk.Builder curChunkBuilder = FileChunk.newBuilder();
      ByteBuffer buf = ByteBuffer.allocate(DOWNLOAD_CHUNK_SIZE);
      while (info.file.readNext(buf) > 0) {
        curChunkBuilder.setChunkNumber(++chunkNumber);
        curChunkBuilder.setData(ByteString.copyFrom(buf));

        respBuilder.setCurrentChunk(curChunkBuilder.build());
        res.onNext(respBuilder.build());

        buf.clear();
      }

      res.onCompleted();
    } catch (AccessDeniedException | JWTVerificationException ex) {
      Logger.getLogger(FileService.class.getName()).log(Level.SEVERE, null, ex);
      res.onError(Status.UNAUTHENTICATED.withDescription(ex.getMessage()).asException());
    } catch (SQLException
        | NoSuchMethodException
        | InstantiationException
        | IllegalAccessException
        | IllegalArgumentException
        | InvocationTargetException
        | IOException ex) {
      Logger.getLogger(FileService.class.getName()).log(Level.SEVERE, null, ex);
      res.onError(Status.INTERNAL.withDescription(ex.getMessage()).asException());
    }
  }
}
