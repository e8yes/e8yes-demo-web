import 'dart:async';

import 'package:demoweb_app/src/proto_dart/service_socialnetwork.pbgrpc.dart';

abstract class SocialNetworkServiceInterface {
  Future<GetUserRelationsResponse> getUserRelations(
      GetUserRelationsRequest request, String signature);

  Future<SendInvitationResponse> sendInvitation(
      SendInvitationRequest request, String signature);

  Future<ProcessInvitationResponse> processInvitation(
      ProcessInvitationRequest request, String signature);

  Future<GetRelatedUserListResponse> getRelatedUserList(
      GetRelatedUserListRequest request, String signature);
}
