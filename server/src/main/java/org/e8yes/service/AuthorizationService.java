package org.e8yes.service;

import com.google.protobuf.ByteString;
import io.grpc.stub.StreamObserver;
import java.util.Optional;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.service.identity.AccessControl;

/** */
public class AuthorizationService extends AuthServiceGrpc.AuthServiceImplBase {

  @Override
  public void authorize(AuthorizationRequest req, StreamObserver<AuthorizationResponse> res) {
    AuthorizationResponse response;
    try {
      Optional<String> token =
          AccessControl.genTokenFromCredentialPair(req.getUserName(), req.getPassword());
      if (token.isPresent()) {
        response =
            AuthorizationResponse.newBuilder()
                .setToken(ByteString.copyFrom(token.get().getBytes()))
                .setErrorType(AuthorizationResponse.AuthorizationErrorType.AET_NoErr)
                .setGenericErrorType(GenericErrorType.GET_NoErr)
                .setStatus(200)
                .build();
      } else {
        response =
            AuthorizationResponse.newBuilder()
                .setErrorType(AuthorizationResponse.AuthorizationErrorType.UNRECOGNIZED)
                .setGenericErrorType(GenericErrorType.UNRECOGNIZED)
                .setStatus(500)
                .build();
      }
    } catch (AccessDeniedException ex) {
      response =
          AuthorizationResponse.newBuilder()
              .setErrorType(AuthorizationResponse.AuthorizationErrorType.AET_IncorrectCredential)
              .setGenericErrorType(GenericErrorType.GET_NoErr)
              .setStatus(ex.getStatusCode())
              .build();
    }

    res.onNext(response);
    res.onCompleted();
  }
}
