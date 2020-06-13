import 'package:grpc/grpc_web.dart';

import 'package:demoweb_app/src/authorization_service_interface.dart';
import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/proto_dart/service_user.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_user.pbgrpc.dart';

class AuthorizationServiceImpl implements AuthorizationServiceInterface {
  @override
  Future<AuthorizationResponse> authorize(AuthorizationRequest request) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    AuthorizationResponse res =
        await UserServiceClient(channel).authorize(request);
    demowebServiceConnections.put(channel);
    return res;
  }
}
