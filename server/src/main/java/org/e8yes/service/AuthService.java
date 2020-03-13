package org.e8yes.service;

import io.grpc.stub.StreamObserver;
import java.util.Optional;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.service.identity.AccessControlLogic;

/** @author davis */
public class AuthService extends AuthServiceGrpc.AuthServiceImplBase {

  @Override
  public void authorize(AuthRequest req, StreamObserver<AuthReply> res) {
    AuthReply reply;
    try {
      Optional<String> token =
          AccessControlLogic.genTokenFromCredentialPair(req.getUserName(), req.getPassword());
      if (token.isPresent()) {
        reply =
            AuthReply.newBuilder()
                .setToken(token.get())
                .setErrType(AuthErrType.AET_NoErr)
                .setGenericErrType(GenericErrType.GET_NoErr)
                .setStatus(200)
                .build();
      } else {
        reply =
            AuthReply.newBuilder()
                .setErrType(AuthErrType.UNRECOGNIZED)
                .setGenericErrType(GenericErrType.UNRECOGNIZED)
                .setStatus(500)
                .build();
      }
    } catch (AccessDeniedException ex) {
      reply =
          AuthReply.newBuilder()
              .setErrType(AuthErrType.AET_IncorrectCredential)
              .setGenericErrType(GenericErrType.GET_NoErr)
              .setStatus(ex.getStatusCode())
              .build();
    }

    res.onNext(reply);
    res.onCompleted();
  }
}
