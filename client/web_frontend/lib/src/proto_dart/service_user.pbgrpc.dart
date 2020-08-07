///
//  Generated code. Do not modify.
//  source: service_user.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_user.pb.dart' as $2;
export 'service_user.pb.dart';

class UserServiceClient extends $grpc.Client {
  static final _$register =
      $grpc.ClientMethod<$2.RegistrationRequest, $2.RegistrationResponse>(
          '/e8.UserService/Register',
          ($2.RegistrationRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $2.RegistrationResponse.fromBuffer(value));
  static final _$authorize =
      $grpc.ClientMethod<$2.AuthorizationRequest, $2.AuthorizationResponse>(
          '/e8.UserService/Authorize',
          ($2.AuthorizationRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $2.AuthorizationResponse.fromBuffer(value));
  static final _$getPublicProfile = $grpc.ClientMethod<
          $2.GetPublicProfileRequest, $2.GetPublicProfileResponse>(
      '/e8.UserService/GetPublicProfile',
      ($2.GetPublicProfileRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $2.GetPublicProfileResponse.fromBuffer(value));
  static final _$updatePublicProfile = $grpc.ClientMethod<
          $2.UpdatePublicProfileRequest, $2.UpdatePublicProfileResponse>(
      '/e8.UserService/UpdatePublicProfile',
      ($2.UpdatePublicProfileRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $2.UpdatePublicProfileResponse.fromBuffer(value));
  static final _$search =
      $grpc.ClientMethod<$2.SearchUserRequest, $2.SearchUserResponse>(
          '/e8.UserService/Search',
          ($2.SearchUserRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $2.SearchUserResponse.fromBuffer(value));
  static final _$prepareNewAvatar = $grpc.ClientMethod<
          $2.PrepareNewAvatarRequest, $2.PrepareNewAvatarResponse>(
      '/e8.UserService/PrepareNewAvatar',
      ($2.PrepareNewAvatarRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $2.PrepareNewAvatarResponse.fromBuffer(value));

  UserServiceClient($grpc.ClientChannel channel, {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$2.RegistrationResponse> register(
      $2.RegistrationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$register, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$2.AuthorizationResponse> authorize(
      $2.AuthorizationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$authorize, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$2.GetPublicProfileResponse> getPublicProfile(
      $2.GetPublicProfileRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getPublicProfile, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$2.UpdatePublicProfileResponse> updatePublicProfile(
      $2.UpdatePublicProfileRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$updatePublicProfile, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$2.SearchUserResponse> search(
      $2.SearchUserRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$search, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$2.PrepareNewAvatarResponse> prepareNewAvatar(
      $2.PrepareNewAvatarRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$prepareNewAvatar, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }
}

abstract class UserServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.UserService';

  UserServiceBase() {
    $addMethod(
        $grpc.ServiceMethod<$2.RegistrationRequest, $2.RegistrationResponse>(
            'Register',
            register_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $2.RegistrationRequest.fromBuffer(value),
            ($2.RegistrationResponse value) => value.writeToBuffer()));
    $addMethod(
        $grpc.ServiceMethod<$2.AuthorizationRequest, $2.AuthorizationResponse>(
            'Authorize',
            authorize_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $2.AuthorizationRequest.fromBuffer(value),
            ($2.AuthorizationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$2.GetPublicProfileRequest,
            $2.GetPublicProfileResponse>(
        'GetPublicProfile',
        getPublicProfile_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $2.GetPublicProfileRequest.fromBuffer(value),
        ($2.GetPublicProfileResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$2.UpdatePublicProfileRequest,
            $2.UpdatePublicProfileResponse>(
        'UpdatePublicProfile',
        updatePublicProfile_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $2.UpdatePublicProfileRequest.fromBuffer(value),
        ($2.UpdatePublicProfileResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$2.SearchUserRequest, $2.SearchUserResponse>(
        'Search',
        search_Pre,
        false,
        false,
        ($core.List<$core.int> value) => $2.SearchUserRequest.fromBuffer(value),
        ($2.SearchUserResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$2.PrepareNewAvatarRequest,
            $2.PrepareNewAvatarResponse>(
        'PrepareNewAvatar',
        prepareNewAvatar_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $2.PrepareNewAvatarRequest.fromBuffer(value),
        ($2.PrepareNewAvatarResponse value) => value.writeToBuffer()));
  }

  $async.Future<$2.RegistrationResponse> register_Pre($grpc.ServiceCall call,
      $async.Future<$2.RegistrationRequest> request) async {
    return register(call, await request);
  }

  $async.Future<$2.AuthorizationResponse> authorize_Pre($grpc.ServiceCall call,
      $async.Future<$2.AuthorizationRequest> request) async {
    return authorize(call, await request);
  }

  $async.Future<$2.GetPublicProfileResponse> getPublicProfile_Pre(
      $grpc.ServiceCall call,
      $async.Future<$2.GetPublicProfileRequest> request) async {
    return getPublicProfile(call, await request);
  }

  $async.Future<$2.UpdatePublicProfileResponse> updatePublicProfile_Pre(
      $grpc.ServiceCall call,
      $async.Future<$2.UpdatePublicProfileRequest> request) async {
    return updatePublicProfile(call, await request);
  }

  $async.Future<$2.SearchUserResponse> search_Pre($grpc.ServiceCall call,
      $async.Future<$2.SearchUserRequest> request) async {
    return search(call, await request);
  }

  $async.Future<$2.PrepareNewAvatarResponse> prepareNewAvatar_Pre(
      $grpc.ServiceCall call,
      $async.Future<$2.PrepareNewAvatarRequest> request) async {
    return prepareNewAvatar(call, await request);
  }

  $async.Future<$2.RegistrationResponse> register(
      $grpc.ServiceCall call, $2.RegistrationRequest request);
  $async.Future<$2.AuthorizationResponse> authorize(
      $grpc.ServiceCall call, $2.AuthorizationRequest request);
  $async.Future<$2.GetPublicProfileResponse> getPublicProfile(
      $grpc.ServiceCall call, $2.GetPublicProfileRequest request);
  $async.Future<$2.UpdatePublicProfileResponse> updatePublicProfile(
      $grpc.ServiceCall call, $2.UpdatePublicProfileRequest request);
  $async.Future<$2.SearchUserResponse> search(
      $grpc.ServiceCall call, $2.SearchUserRequest request);
  $async.Future<$2.PrepareNewAvatarResponse> prepareNewAvatar(
      $grpc.ServiceCall call, $2.PrepareNewAvatarRequest request);
}
