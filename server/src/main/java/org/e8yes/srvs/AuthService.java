package org.e8yes.srvs;

import io.grpc.stub.StreamObserver;

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
                RegReply reply = RegReply
                        .newBuilder()
                        .setErrType(RegErrType.RET_NoErr)
                        .setGenericErrType(GenericErrType.GET_NoErr)
                        .setStatus(200)
                        .build();
                res.onNext(reply);
                res.onCompleted();
        }
}
