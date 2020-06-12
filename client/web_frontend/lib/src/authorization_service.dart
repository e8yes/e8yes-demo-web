import 'package:grpc/grpc_web.dart';

import 'package:demoweb_app/src/authorization_service_interface.dart';

class AuthorizationServiceImpl implements AuthorizationService {
  @override
  Future<AuthorizationResponse> authorize(AuthorizationRequest request) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    AuthorizationResponse res =
        await UserServiceClient(channel).authorize(request);
    demowebServiceConnections.put(channel);
    return res;
  }
}
