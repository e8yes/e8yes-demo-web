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

  Future<ProcessInvitationResponse> processInvitation(
      ProcessInvitationRequest request, String signature) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    ProcessInvitationResponse res = await SocialNetworkServiceClient(channel)
        .processInvitation(request,
            options: CallOptions(metadata: {'a': signature}));
    demowebServiceConnections.put(channel);
    return res;
  }

  Future<GetRelatedUserListResponse> getRelatedUserList(
      GetRelatedUserListRequest request, String signature) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    GetRelatedUserListResponse res = await SocialNetworkServiceClient(channel)
        .getRelatedUserList(request,
            options: CallOptions(metadata: {'a': signature}));
    demowebServiceConnections.put(channel);
    return res;
  }
}
