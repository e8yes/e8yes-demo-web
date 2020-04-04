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
import org.e8yes.constant.FileStreamConstants;
import org.e8yes.constant.GrpcContexts;
import org.e8yes.environment.Initializer;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.exception.ResourceMissingException;
import org.e8yes.fsprovider.FileSystemProviderInterface;
import org.e8yes.service.file.FileEntity;
import org.e8yes.service.file.FileIO;
import org.e8yes.service.file.FileMetaData;

/**
 * Handle file upload. Validate identity then store the data chunk in sequence to the file system.
 */
class UploadStreamObserver implements StreamObserver<UploadFileRequest> {

  private final StreamObserver<UploadFileResponse> res;
  private FileIO.FileAccessor fileAccessor;
  private int fileSize = 0;

  public UploadStreamObserver(StreamObserver<UploadFileResponse> res) {
    this.res = res;
  }

  @Override
  public void onNext(UploadFileRequest request) {
    if (fileAccessor == null) {
      try {
        fileAccessor =
            FileIO.validateAndOpenFile(
                GrpcContexts.IDENTITY_CONTEXT_KEY.get(),
                request.getFileDescriptor(),
                FileAccessMode.FAM_READ,
                Initializer.environmentContext().fileSystemProvider(),
                Initializer.environmentContext().authorizationJwtProvider().jwtverifier(),
                Initializer.environmentContext().demowebDbConnections().connectionReservoir());

        FileMetaData.addMetaDataForFile(
            fileAccessor.location,
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
          fileAccessor.file.writeNext(request.getCurrentChunk().getData().asReadOnlyByteBuffer());
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
      fsProvider.close(fileAccessor.file);

      FileMetaData.updateFileSize(
          fileAccessor.location,
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

  @Override
  public StreamObserver<UploadFileRequest> upload(StreamObserver<UploadFileResponse> res) {
    return new UploadStreamObserver(res);
  }

  @Override
  public void download(DownloadFileRequest request, StreamObserver<DownloadFileResponse> res) {
    try {
      FileIO.FileAccessor fileAccessor =
          FileIO.validateAndOpenFile(
              GrpcContexts.IDENTITY_CONTEXT_KEY.get(),
              request.getFileDescriptor(),
              FileAccessMode.FAM_READ,
              Initializer.environmentContext().fileSystemProvider(),
              Initializer.environmentContext().authorizationJwtProvider().jwtverifier(),
              Initializer.environmentContext().demowebDbConnections().connectionReservoir());

      FileEntity fileMeta =
          FileMetaData.retrieveFileMetadata(
              fileAccessor.location,
              Initializer.environmentContext().demowebDbConnections().connectionReservoir());
      if (fileMeta == null) {
        throw new ResourceMissingException("File at " + fileAccessor.location + " doesn't exist.");
      }

      FileDescriptor.Builder desc = FileMetaData.createFileDescriptorBuilder(fileMeta);
      desc.setFileDirectAccess(request.getFileDescriptor().getFileDirectAccess());
      desc.setFileTokenAccess(request.getFileDescriptor().getFileTokenAccess());

      DownloadFileResponse.Builder respBuilder = DownloadFileResponse.newBuilder();
      respBuilder.setFileDescriptor(desc.build());

      int chunkNumber = -1;
      FileChunk.Builder curChunkBuilder = FileChunk.newBuilder();
      ByteBuffer buf = ByteBuffer.allocate(FileStreamConstants.CHUNK_SIZE_LIMIT);
      while (fileAccessor.file.readNext(buf) > 0) {
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
    } catch (ResourceMissingException ex) {
      Logger.getLogger(FileService.class.getName()).log(Level.SEVERE, null, ex);
      res.onError(Status.NOT_FOUND.withDescription(ex.getMessage()).asException());
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
