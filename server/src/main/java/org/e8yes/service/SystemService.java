package org.e8yes.service;

import io.grpc.stub.StreamObserver;

public class SystemService extends SystemServiceGrpc.SystemServiceImplBase {

  @Override
  public void version(Empty request, StreamObserver<VersionReply> responseObserver) {
    VersionReply versionReply = VersionReply.newBuilder().setVersion("0.1.1").build();
    responseObserver.onNext(versionReply);
    responseObserver.onCompleted();
  }
}
