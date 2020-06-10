import "dart:async";

import "proto_dart/service_user.pb.dart";

abstract class UserServiceInterface {
  Future<RegistrationResponse> register(RegistrationRequest request);
}