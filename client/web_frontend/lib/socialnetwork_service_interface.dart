import "dart:async";

import "package:demoweb_app/src/proto_dart/service_socialnetwork.pb.dart";

abstract class SocialNetworkServiceInterface {
  Future<GetUserRelationResponse> getUserRelation(
      GetUserRelationRequest request);
}
