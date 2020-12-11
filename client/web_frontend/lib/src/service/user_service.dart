import "dart:async";

import 'package:demoweb_app/src/basic/context.dart';
import 'package:demoweb_app/src/service/user_service_interface.dart';
import "package:demoweb_app/src/proto_dart/service_user.pb.dart";
import "package:demoweb_app/src/proto_dart/service_user.pbgrpc.dart";

class UserServiceImpl implements UserServiceInterface {
  @override
  Future<RegistrationResponse> register(RegistrationRequest request) async {
    return demowebServiceRequester.MakeRequest(request, null,
        (request, call_opts, channel) {
      return UserServiceClient(channel).register(request);
    });
  }

  @override
  Future<GetPublicProfileResponse> getPublicProfile(
      GetPublicProfileRequest request, String signature) async {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return UserServiceClient(channel)
          .getPublicProfile(request, options: call_opts);
    });
  }

  @override
  Future<UpdatePublicProfileResponse> updatePublicProfile(
      UpdatePublicProfileRequest request, String signature) async {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return UserServiceClient(channel)
          .updatePublicProfile(request, options: call_opts);
    });
  }

  @override
  Future<SearchUserResponse> search(
      SearchUserRequest request, String signature) async {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return UserServiceClient(channel).search(request, options: call_opts);
    });
  }

  @override
  Future<PrepareNewAvatarResponse> prepareNewAvatar(
      PrepareNewAvatarRequest request, String signature) async {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return UserServiceClient(channel)
          .prepareNewAvatar(request, options: call_opts);
    });
  }
}
