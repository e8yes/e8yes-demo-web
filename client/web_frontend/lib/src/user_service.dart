import "dart:async";

import 'package:demoweb_app/src/context.dart';
import 'package:grpc/grpc_web.dart';

import "user_service_interface.dart";
import "proto_dart/service_user.pb.dart";
import "proto_dart/service_user.pbgrpc.dart";

class UserServiceImpl implements UserServiceInterface {
  @override
  Future<RegistrationResponse> register(RegistrationRequest request) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    RegistrationResponse res = await UserServiceClient(channel).register(request);
    demowebServiceConnections.put(channel);
    return res;
  }
}