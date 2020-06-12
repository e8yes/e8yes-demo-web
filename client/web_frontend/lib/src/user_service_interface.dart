import "dart:async";

import "proto_dart/service_user.pb.dart";

abstract class UserServiceInterface {
  Future<RegistrationResponse> register(RegistrationRequest request);
  Future<AuthorizationResponse> authorize(AuthorizationRequest request);
  Future<GetPublicProfileResponse> getPublicProfile(
      GetPublicProfileRequest request);
  Future<UpdatePublicProfileResponse> updatePublicProfile(
      UpdatePublicProfileRequest request, List<int> signature);
  Future<SearchUserResponse> search(SearchUserRequest request);
  Future<PrepareNewAvatarResponse> prepareNewAvatar(
      PrepareNewAvatarRequest request, List<int> signature);
}
