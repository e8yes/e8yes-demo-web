import "dart:async";

import "package:demoweb_app/src/proto_dart/service_user.pb.dart";

abstract class AuthorizationServiceInterface {
  Future<AuthorizationResponse> authorize(AuthorizationRequest request);
}
