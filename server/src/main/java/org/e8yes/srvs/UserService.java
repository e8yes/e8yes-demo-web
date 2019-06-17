package org.e8yes.srvs;

import io.grpc.stub.StreamObserver;
import org.drools.core.util.StringUtils;
import org.e8yes.srvs.buzlogic.AUserGroupLogic;
import org.e8yes.srvs.buzlogic.AUserLogic;
import org.e8yes.srvs.buzlogic.errs.ResourceConflictException;

/** @author davis */
public class UserService extends UserServiceGrpc.UserServiceImplBase {

  @Override
  public void register(RegRequest req, StreamObserver<RegReply> res) {
    RegReply reply;
    try {
      EtUser user =
          AUserLogic.createBaselineUser(
              req.getUserName(),
              StringUtils.EMPTY,
              req.getPassword(),
              AUserGroupLogic.getContext());
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
              .setStatus(200)
              .build();
    }

    res.onNext(reply);
    res.onCompleted();
  }
}
