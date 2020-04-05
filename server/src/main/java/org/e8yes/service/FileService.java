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
import org.e8yes.service.file.FileEntity;
import org.e8yes.service.file.FileIO;
import org.e8yes.service.file.FileIO.FileChunkWriter;
import org.e8yes.service.file.FileMetaData;
import org.e8yes.sql.connection.ConnectionReservoirInterface;

/**
 * Handle file upload. Validate identity then store the data chunk in sequence to the file system.
 */

/** Service for file transfer. */
public class FileService extends FileServiceGrpc.FileServiceImplBase {

  @Override
  public StreamObserver<UploadFileRequest> upload(StreamObserver<UploadFileResponse> res) {
    return new StreamObserver<UploadFileRequest>() {
      private final FileChunkWriter fileWriter =
          new FileChunkWriter(
              Initializer.environmentContext().fileSystemProvider(),
              Initializer.environmentContext().authorizationJwtProvider().jwtverifier(),
              Initializer.environmentContext().demowebDbConnections().connectionReservoir());

      @Override
      public void onNext(UploadFileRequest request) {
        try {
          fileWriter.writeNextChunk(
              GrpcContexts.IDENTITY_CONTEXT_KEY.get(),
              request.getFileDescriptor(),
              request.getCurrentChunk().getData().asReadOnlyByteBuffer());
        } catch (IOException | IllegalArgumentException | SQLException ex) {
          Logger.getLogger(FileService.class.getName()).log(Level.SEVERE, null, ex);
          res.onError(Status.INTERNAL.withDescription(ex.getMessage()).asException());
        } catch (JWTVerificationException | AccessDeniedException ex) {
          Logger.getLogger(FileService.class.getName()).log(Level.SEVERE, null, ex);
          res.onError(Status.PERMISSION_DENIED.withDescription(ex.getMessage()).asException());
        } catch (IllegalAccessException ex) {
          Logger.getLogger(FileService.class.getName()).log(Level.SEVERE, null, ex);
          res.onError(Status.INVALID_ARGUMENT.withDescription(ex.getMessage()).asException());
        }
      }

      @Override
      public void onError(Throwable ex) {
        res.onError(ex);
      }

      @Override
      public void onCompleted() {
        try {
          fileWriter.close();
          res.onCompleted();
        } catch (IOException
            | SQLException
            | NoSuchMethodException
            | InstantiationException
            | IllegalAccessException
            | IllegalArgumentException
            | InvocationTargetException ex) {
          Logger.getLogger(FileService.class.getName()).log(Level.SEVERE, null, ex);
          res.onError(Status.INTERNAL.withDescription(ex.getMessage()).asException());
        } catch (ResourceMissingException ex) {
          Logger.getLogger(FileService.class.getName()).log(Level.SEVERE, null, ex);
          res.onError(Status.NOT_FOUND.withDescription(ex.getMessage()).asException());
        }
      }
    };
  }

  @Override
  public void download(DownloadFileRequest request, StreamObserver<DownloadFileResponse> res) {
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();

    try {
      FileIO.readFile(
          GrpcContexts.IDENTITY_CONTEXT_KEY.get(),
          request.getFileDescriptor(),
          Initializer.environmentContext().fileSystemProvider(),
          Initializer.environmentContext().authorizationJwtProvider().jwtverifier(),
          dbConn,
          new FileIO.FileChunkListener() {
            private DownloadFileResponse.Builder respBuilder;
            private FileChunk.Builder chunkBuilder;

            @Override
            public void onReady(FileEntity metadata) {
              FileDescriptor.Builder desc = FileMetaData.createFileDescriptorBuilder(metadata);
              respBuilder = DownloadFileResponse.newBuilder();
              respBuilder.setFileDescriptor(desc.build());
            }

            @Override
            public void onComplete() {
              res.onCompleted();
            }

            @Override
            public void processNextChunk(ByteBuffer chunkData, int chunkNumber) {
              chunkBuilder.setChunkNumber(chunkNumber);
              chunkBuilder.setData(ByteString.copyFrom(chunkData));
              respBuilder.setCurrentChunk(chunkBuilder.build());
              res.onNext(respBuilder.build());
            }
          });
    } catch (AccessDeniedException | JWTVerificationException ex) {
      Logger.getLogger(FileService.class.getName()).log(Level.SEVERE, null, ex);
      res.onError(Status.PERMISSION_DENIED.withDescription(ex.getMessage()).asException());
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
