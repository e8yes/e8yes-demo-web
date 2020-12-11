import "dart:async";

import '../proto_dart/service_user.pb.dart';

abstract class UserServiceInterface {
  Future<RegistrationResponse> register(RegistrationRequest request);

  Future<GetPublicProfileResponse> getPublicProfile(
      GetPublicProfileRequest request, String signature);

  Future<UpdatePublicProfileResponse> updatePublicProfile(
      UpdatePublicProfileRequest request, String signature);

  Future<SearchUserResponse> search(
      SearchUserRequest request, String signature);

  Future<PrepareNewAvatarResponse> prepareNewAvatar(
      PrepareNewAvatarRequest request, String signature);
}
