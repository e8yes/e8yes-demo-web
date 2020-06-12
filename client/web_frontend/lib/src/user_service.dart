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
    RegistrationResponse res =
        await UserServiceClient(channel).register(request);
    demowebServiceConnections.put(channel);
    return res;
  }

  @override
  Future<AuthorizationResponse> authorize(AuthorizationRequest request) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    AuthorizationResponse res =
        await UserServiceClient(channel).authorize(request);
    demowebServiceConnections.put(channel);
    return res;
  }

  @override
  Future<GetPublicProfileResponse> getPublicProfile(
      GetPublicProfileRequest request) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    GetPublicProfileResponse res =
        await UserServiceClient(channel).getPublicProfile(request);
    demowebServiceConnections.put(channel);
    return res;
  }

  @override
  Future<UpdatePublicProfileResponse> updatePublicProfile(
      UpdatePublicProfileRequest request, List<int> signature) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    UpdatePublicProfileResponse res =
        await UserServiceClient(channel).updatePublicProfile(request);
    demowebServiceConnections.put(channel);
    return res;
  }

  @override
  Future<SearchUserResponse> search(SearchUserRequest request) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    SearchUserResponse res = await UserServiceClient(channel).search(request);
    demowebServiceConnections.put(channel);
    return res;
  }

  @override
  Future<PrepareNewAvatarResponse> prepareNewAvatar(
      PrepareNewAvatarRequest request, List<int> signature) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    PrepareNewAvatarResponse res =
        await UserServiceClient(channel).prepareNewAvatar(request);
    demowebServiceConnections.put(channel);
    return res;
  }
}
