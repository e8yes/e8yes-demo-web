package org.e8yes.srvs;

import io.grpc.stub.StreamObserver;
import org.e8yes.service.AcceptFriendshipRReply;
import org.e8yes.service.AcceptFriendshipRRequest;
import org.e8yes.service.FriendListReply;
import org.e8yes.service.FriendListRequest;
import org.e8yes.service.FriendshipRReply;
import org.e8yes.service.FriendshipRRequest;
import org.e8yes.service.FriendshipServiceGrpc;
import org.e8yes.service.RemoveFriendReply;
import org.e8yes.service.RemoveFriendRequest;

/**
 *
 * @author davis
 */
public class FriendshipService extends FriendshipServiceGrpc.FriendshipServiceImplBase {

        @Override
        public void sendFriendshipRequest(FriendshipRRequest req, StreamObserver<FriendshipRReply> res) {
        }

        @Override
        public void acceptFriendRequest(AcceptFriendshipRRequest req, StreamObserver<AcceptFriendshipRReply> res) {
        }

        @Override
        public void removeFriend(RemoveFriendRequest req, StreamObserver<RemoveFriendReply> res) {
        }

        @Override
        public void getFriendList(FriendListRequest req, StreamObserver<FriendListReply> res) {
        }
}
