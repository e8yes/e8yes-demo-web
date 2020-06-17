import "dart:async";
import 'package:grpc/grpc_web.dart';

import 'package:demoweb_app/src/context.dart';
import "package:demoweb_app/src/user_service_interface.dart";
import "package:demoweb_app/src/proto_dart/service_user.pb.dart";
import "package:demoweb_app/src/proto_dart/service_user.pbgrpc.dart";

class UserServiceImpl implements UserServiceInterface {
  @override
  Future<RegistrationResponse> register(RegistrationRequest request) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    RegistrationResponse res =
        await UserServiceClient(channel).register(request);
    demowebServiceConnections.put(channel);
    return res;
  }

  @override
  Future<GetPublicProfileResponse> getPublicProfile(
      GetPublicProfileRequest request, String signature) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    GetPublicProfileResponse res = await UserServiceClient(channel)
        .getPublicProfile(request,
            options: CallOptions(metadata: {'a': signature}));
    demowebServiceConnections.put(channel);
    return res;
  }

  @override
  Future<UpdatePublicProfileResponse> updatePublicProfile(
      UpdatePublicProfileRequest request, String signature) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    UpdatePublicProfileResponse res = await UserServiceClient(channel)
        .updatePublicProfile(request,
            options: CallOptions(metadata: {'a': signature}));
    demowebServiceConnections.put(channel);
    return res;
  }

  @override
  Future<SearchUserResponse> search(
      SearchUserRequest request, String signature) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    SearchUserResponse res = await UserServiceClient(channel)
        .search(request, options: CallOptions(metadata: {'a': signature}));
    demowebServiceConnections.put(channel);
    return res;
  }

  @override
  Future<PrepareNewAvatarResponse> prepareNewAvatar(
      PrepareNewAvatarRequest request, String signature) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    PrepareNewAvatarResponse res = await UserServiceClient(channel)
        .prepareNewAvatar(request,
            options: CallOptions(metadata: {'a': signature}));
    demowebServiceConnections.put(channel);
    return res;
  }
}
