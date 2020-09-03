import "dart:async";

import "package:demoweb_app/src/context.dart";
import 'package:demoweb_app/src/proto_dart/service_socialnetwork.pbgrpc.dart';
import "package:demoweb_app/src/socialnetwork_service_interface.dart";

class SocialNetworkServiceImpl implements SocialNetworkServiceInterface {
  @override
  Future<GetUserRelationsResponse> getUserRelations(
      GetUserRelationsRequest request, String signature) async {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return SocialNetworkServiceClient(channel)
          .getUserRelations(request, options: call_opts);
    });
  }

  Future<SendInvitationResponse> sendInvitation(
      SendInvitationRequest request, String signature) async {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return SocialNetworkServiceClient(channel)
          .sendInvitation(request, options: call_opts);
    });
  }

  Future<ProcessInvitationResponse> processInvitation(
      ProcessInvitationRequest request, String signature) async {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return SocialNetworkServiceClient(channel)
          .processInvitation(request, options: call_opts);
    });
  }

  Future<GetRelatedUserListResponse> getRelatedUserList(
      GetRelatedUserListRequest request, String signature) async {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      SocialNetworkServiceClient(channel)
          .getRelatedUserList(request, options: call_opts);
    });
  }
}
