import "dart:async";

import "package:demoweb_app/src/context.dart";
import 'package:demoweb_app/src/proto_dart/service_socialnetwork.pbgrpc.dart';
import "package:demoweb_app/src/socialnetwork_service_interface.dart";
import 'package:grpc/grpc_web.dart';
import 'package:grpc/src/client/web_channel.dart';

class SocialNetworkServiceImpl implements SocialNetworkServiceInterface {
  @override
  Future<GetUserRelationsResponse> getUserRelations(
      GetUserRelationsRequest request, String signature) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    GetUserRelationsResponse res = await SocialNetworkServiceClient(channel)
        .getUserRelations(request,
            options: CallOptions(metadata: {'a': signature}));
    demowebServiceConnections.put(channel);
    return res;
  }

  Future<SendInvitationResponse> sendInvitation(
      SendInvitationRequest request, String signature) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    SendInvitationResponse res = await SocialNetworkServiceClient(channel)
        .sendInvitation(request,
            options: CallOptions(metadata: {'a': signature}));
    demowebServiceConnections.put(channel);
    return res;
  }

  Future<GetInvitationListResponse> getInvitationList(
      GetInvitationListRequest request, String signature) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    GetInvitationListResponse res = await SocialNetworkServiceClient(channel)
        .getInvitationList(request,
            options: CallOptions(metadata: {'a': signature}));
    demowebServiceConnections.put(channel);
    return res;
  }
}
