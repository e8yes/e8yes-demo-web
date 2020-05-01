package org.e8yes.service;

import io.grpc.stub.StreamObserver;

public class SystemService extends SystemServiceGrpc.SystemServiceImplBase {

  @Override
  public void version(Empty request, StreamObserver<VersionResponse> responseObserver) {
    VersionResponse response = VersionResponse.newBuilder().setVersion("0.1.1").build();
    responseObserver.onNext(response);
    responseObserver.onCompleted();
  }
}
