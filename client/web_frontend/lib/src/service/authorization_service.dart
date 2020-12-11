import 'package:demoweb_app/src/service/authorization_service_interface.dart';
import 'package:demoweb_app/src/basic/context.dart';
import 'package:demoweb_app/src/proto_dart/service_user.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_user.pbgrpc.dart';

class AuthorizationServiceImpl implements AuthorizationServiceInterface {
  @override
  Future<AuthorizationResponse> authorize(AuthorizationRequest request) async {
    return demowebServiceRequester.MakeRequest(request, null,
        (request, call_opts, channel) {
      return UserServiceClient(channel).authorize(request);
    });
  }
}
