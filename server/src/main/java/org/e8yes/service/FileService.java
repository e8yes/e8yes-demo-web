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

import io.grpc.stub.StreamObserver;

class UploadStreamObserver implements StreamObserver<UploadFileRequest> {

  private final StreamObserver<UploadFileResponse> res;

  public UploadStreamObserver(StreamObserver<UploadFileResponse> res) {
    this.res = res;
  }

  @Override
  public void onNext(UploadFileRequest v) {
    throw new UnsupportedOperationException(
        "Not supported yet."); // To change body of generated methods, choose Tools | Templates.
  }

  @Override
  public void onError(Throwable thrwbl) {
    throw new UnsupportedOperationException(
        "Not supported yet."); // To change body of generated methods, choose Tools | Templates.
  }

  @Override
  public void onCompleted() {
    throw new UnsupportedOperationException(
        "Not supported yet."); // To change body of generated methods, choose Tools | Templates.
  }
}

/** Service for file management. */
public class FileService extends FileServiceGrpc.FileServiceImplBase {

  @Override
  public StreamObserver<UploadFileRequest> upload(StreamObserver<UploadFileResponse> res) {
    return new UploadStreamObserver(res);
  }
}
