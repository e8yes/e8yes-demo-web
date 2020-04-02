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
import io.grpc.Status;
import io.grpc.stub.StreamObserver;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.e8yes.constant.GrpcContexts;
import org.e8yes.environment.Initializer;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.fsprovider.FileAccessLocation;
import org.e8yes.fsprovider.FileHandleInterface;
import org.e8yes.fsprovider.FileSystemProviderInterface;
import org.e8yes.jwtprovider.JwtAlgorithmProviderInterface;
import org.e8yes.service.file.FileAccessValidator;
import org.e8yes.service.file.FileAccessValidator.FileAccessToken;
import org.e8yes.service.identity.Identity;

/**
 * Handle file upload. Validate identity then store the data chunk in sequence to the file system.
 */
class UploadStreamObserver implements StreamObserver<UploadFileRequest> {

  private final StreamObserver<UploadFileResponse> res;
  private FileHandleInterface file;

  public UploadStreamObserver(StreamObserver<UploadFileResponse> res) {
    this.res = res;
  }

  private static FileHandleInterface validateIdentityAndOpenFile(
      FileDescriptor fileDesc, Identity viewer)
      throws JWTVerificationException, AccessDeniedException, IOException {
    JwtAlgorithmProviderInterface jwtProvider =
        Initializer.environmentContext().authorizationJwtProvider();

    FileAccessLocation location;
    if (fileDesc.hasFileTokenAccess()) {
      FileTokenAccess tokenAccess = fileDesc.getFileTokenAccess();
      FileAccessToken token = new FileAccessToken();
      token.jwtToken = tokenAccess.getAccessToken().toByteArray();
      location =
          FileAccessValidator.validateTokenAccess(
              viewer, FileAccessMode.FAM_WRITE, token, jwtProvider.jwtverifier());
    } else if (fileDesc.hasFileDirectAccess()) {
      location = new FileAccessLocation(fileDesc.getFileDirectAccess().getPath());
      FileAccessValidator.signAccessToken(
          viewer, location, FileAccessMode.FAM_WRITE, jwtProvider.algorithm());
    } else {
      throw new AccessDeniedException("Unknown access validation method.");
    }

    FileSystemProviderInterface fsProvider = Initializer.environmentContext().fileSystemProvider();
    fsProvider.createAndOverride(location);
    return fsProvider.open(location);
  }

  @Override
  public void onNext(UploadFileRequest request) {
    if (file == null) {
      try {
        file =
            validateIdentityAndOpenFile(
                request.getFileDescriptor(), GrpcContexts.IDENTITY_CONTEXT_KEY.get());
      } catch (IOException ex) {
        Logger.getLogger(UploadStreamObserver.class.getName()).log(Level.SEVERE, null, ex);
        res.onError(Status.INTERNAL.withDescription(ex.getMessage()).asException());
      } catch (AccessDeniedException | JWTVerificationException ex) {
        Logger.getLogger(UploadStreamObserver.class.getName()).log(Level.SEVERE, null, ex);
        res.onError(Status.UNAUTHENTICATED.withDescription(ex.getMessage()).asException());
      }
    }

    try {
      file.writeNext(request.getCurrentChunk().getData().asReadOnlyByteBuffer());
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
      fsProvider.close(file);
      res.onCompleted();
    } catch (IOException ex) {
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
}
