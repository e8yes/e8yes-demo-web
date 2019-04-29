package org.e8yes.srvs;

import io.grpc.stub.StreamObserver;
import org.drools.core.util.StringUtils;
import org.e8yes.srvs.buzlogic.AUserGroupLogic;
import org.e8yes.srvs.buzlogic.AUserLogic;
import org.e8yes.srvs.buzlogic.ctx.AUserGroupContext;

/**
 *
 * @author davis
 */
public class AuthService extends AuthServiceGrpc.AuthServiceImplBase {

        @Override
        public void
                authorize(AuthRequest req, StreamObserver<AuthReply> res) {
                AuthReply reply = AuthReply
                        .newBuilder()
                        .setToken("ABCDEFG")
                        .setErrType(AuthErrType.AET_NoErr)
                        .setGenericErrType(GenericErrType.GET_NoErr)
                        .setStatus(200)
                        .build();
                res.onNext(reply);
                res.onCompleted();
        }

        @Override
        public void
                register(RegRequest req, StreamObserver<RegReply> res) {
                AUserGroupContext ctx = AUserGroupLogic.getContext();
                EtUser user = AUserLogic.createBaselineUser(req.getUserName(),
                                                    StringUtils.EMPTY,
                                                    req.getPassword(),
                                                    ctx);
                RegReply reply = RegReply
                        .newBuilder()
                        .setErrType(RegErrType.RET_NoErr)
                        .setGenericErrType(GenericErrType.GET_NoErr)
                        .setUserId(user.getId())
                        .setStatus(200)
                        .build();
                res.onNext(reply);
                res.onCompleted();
        }
}
