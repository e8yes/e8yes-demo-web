package org.e8yes.service;

import io.grpc.stub.StreamObserver;
import org.drools.core.util.StringUtils;
import org.e8yes.exception.ResourceConflictException;
import org.e8yes.service.identity.User;
import org.e8yes.service.identity.UserGroup;

/** @author davis */
public class UserService extends UserServiceGrpc.UserServiceImplBase {

  @Override
  public void register(RegRequest req, StreamObserver<RegReply> res) {
    RegReply reply;
    try {
      EtUser user =
          User.createBaselineUser(
              req.getUserName(), StringUtils.EMPTY, req.getPassword(), UserGroup.getContext());
      reply =
          RegReply.newBuilder()
              .setErrType(RegErrType.RET_NoErr)
              .setGenericErrType(GenericErrType.GET_NoErr)
              .setUserId(user.getId())
              .setStatus(200)
              .build();
    } catch (ResourceConflictException ex) {
      reply =
          RegReply.newBuilder()
              .setErrType(RegErrType.RET_UserNameConflict)
              .setGenericErrType(GenericErrType.UNRECOGNIZED)
              .setStatus(ex.getStatusCode())
              .build();
    }

    res.onNext(reply);
    res.onCompleted();
  }
}
