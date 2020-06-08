///
//  Generated code. Do not modify.
//  source: service_user.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'package:protobuf/protobuf.dart' as $pb;

import 'dart:core' as $core;
import 'service_user.pb.dart' as $6;
import 'service_user.pbjson.dart';

export 'service_user.pb.dart';

abstract class UserServiceBase extends $pb.GeneratedService {
  $async.Future<$6.RegistrationReponse> register($pb.ServerContext ctx, $6.RegistrationRequest request);
  $async.Future<$6.AuthorizationResponse> authorize($pb.ServerContext ctx, $6.AuthorizationRequest request);
  $async.Future<$6.GetPublicProfileResponse> getPublicProfile($pb.ServerContext ctx, $6.GetPublicProfileRequest request);
  $async.Future<$6.UpdatePublicProfileResponse> updatePublicProfile($pb.ServerContext ctx, $6.UpdatePublicProfileRequest request);
  $async.Future<$6.SearchUserResponse> search($pb.ServerContext ctx, $6.SearchUserRequest request);
  $async.Future<$6.PrepareNewAvatarResponse> prepareNewAvatar($pb.ServerContext ctx, $6.PrepareNewAvatarRequest request);

  $pb.GeneratedMessage createRequest($core.String method) {
    switch (method) {
      case 'Register': return $6.RegistrationRequest();
      case 'Authorize': return $6.AuthorizationRequest();
      case 'GetPublicProfile': return $6.GetPublicProfileRequest();
      case 'UpdatePublicProfile': return $6.UpdatePublicProfileRequest();
      case 'Search': return $6.SearchUserRequest();
      case 'PrepareNewAvatar': return $6.PrepareNewAvatarRequest();
      default: throw $core.ArgumentError('Unknown method: $method');
    }
  }

  $async.Future<$pb.GeneratedMessage> handleCall($pb.ServerContext ctx, $core.String method, $pb.GeneratedMessage request) {
    switch (method) {
      case 'Register': return this.register(ctx, request);
      case 'Authorize': return this.authorize(ctx, request);
      case 'GetPublicProfile': return this.getPublicProfile(ctx, request);
      case 'UpdatePublicProfile': return this.updatePublicProfile(ctx, request);
      case 'Search': return this.search(ctx, request);
      case 'PrepareNewAvatar': return this.prepareNewAvatar(ctx, request);
      default: throw $core.ArgumentError('Unknown method: $method');
    }
  }

  $core.Map<$core.String, $core.dynamic> get $json => UserServiceBase$json;
  $core.Map<$core.String, $core.Map<$core.String, $core.dynamic>> get $messageJson => UserServiceBase$messageJson;
}

