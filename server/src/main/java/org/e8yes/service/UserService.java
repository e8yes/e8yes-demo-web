package org.e8yes.service;

import io.grpc.stub.StreamObserver;
import org.drools.core.util.StringUtils;
import org.e8yes.exception.ResourceConflictException;
import org.e8yes.service.identity.User;
import org.e8yes.service.identity.UserGroup;

/** */
public class UserService extends UserServiceGrpc.UserServiceImplBase {

  @Override
  public void register(RegistrationRequest req, StreamObserver<RegistrationReponse> res) {
    RegistrationReponse response;
    try {
      EtUser user =
          User.createBaselineUser(
              req.getUserName(), StringUtils.EMPTY, req.getPassword(), UserGroup.getContext());
      response =
          RegistrationReponse.newBuilder()
              .setErrorType(RegistrationReponse.RegistrationErrorType.RET_NoErr)
              .setGenericErrorType(GenericErrorType.GET_NoErr)
              .setUserId(user.getId())
              .setStatus(200)
              .build();
    } catch (ResourceConflictException ex) {
      response =
          RegistrationReponse.newBuilder()
              .setErrorType(RegistrationReponse.RegistrationErrorType.RET_UserNameConflict)
              .setGenericErrorType(GenericErrorType.UNRECOGNIZED)
              .setStatus(ex.getStatusCode())
              .build();
    }

    res.onNext(response);
    res.onCompleted();
  }
}
