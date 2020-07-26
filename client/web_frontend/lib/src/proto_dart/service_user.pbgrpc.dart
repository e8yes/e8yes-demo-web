///
//  Generated code. Do not modify.
//  source: service_user.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_user.pb.dart' as $1;
export 'service_user.pb.dart';

class UserServiceClient extends $grpc.Client {
  static final _$register =
      $grpc.ClientMethod<$1.RegistrationRequest, $1.RegistrationResponse>(
          '/e8.UserService/Register',
          ($1.RegistrationRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $1.RegistrationResponse.fromBuffer(value));
  static final _$authorize =
      $grpc.ClientMethod<$1.AuthorizationRequest, $1.AuthorizationResponse>(
          '/e8.UserService/Authorize',
          ($1.AuthorizationRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $1.AuthorizationResponse.fromBuffer(value));
  static final _$getPublicProfile = $grpc.ClientMethod<
          $1.GetPublicProfileRequest, $1.GetPublicProfileResponse>(
      '/e8.UserService/GetPublicProfile',
      ($1.GetPublicProfileRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $1.GetPublicProfileResponse.fromBuffer(value));
  static final _$updatePublicProfile = $grpc.ClientMethod<
          $1.UpdatePublicProfileRequest, $1.UpdatePublicProfileResponse>(
      '/e8.UserService/UpdatePublicProfile',
      ($1.UpdatePublicProfileRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $1.UpdatePublicProfileResponse.fromBuffer(value));
  static final _$search =
      $grpc.ClientMethod<$1.SearchUserRequest, $1.SearchUserResponse>(
          '/e8.UserService/Search',
          ($1.SearchUserRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $1.SearchUserResponse.fromBuffer(value));
  static final _$prepareNewAvatar = $grpc.ClientMethod<
          $1.PrepareNewAvatarRequest, $1.PrepareNewAvatarResponse>(
      '/e8.UserService/PrepareNewAvatar',
      ($1.PrepareNewAvatarRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $1.PrepareNewAvatarResponse.fromBuffer(value));

  UserServiceClient($grpc.ClientChannel channel, {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$1.RegistrationResponse> register(
      $1.RegistrationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$register, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$1.AuthorizationResponse> authorize(
      $1.AuthorizationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$authorize, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$1.GetPublicProfileResponse> getPublicProfile(
      $1.GetPublicProfileRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getPublicProfile, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$1.UpdatePublicProfileResponse> updatePublicProfile(
      $1.UpdatePublicProfileRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$updatePublicProfile, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$1.SearchUserResponse> search(
      $1.SearchUserRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$search, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$1.PrepareNewAvatarResponse> prepareNewAvatar(
      $1.PrepareNewAvatarRequest request,
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
        $grpc.ServiceMethod<$1.RegistrationRequest, $1.RegistrationResponse>(
            'Register',
            register_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $1.RegistrationRequest.fromBuffer(value),
            ($1.RegistrationResponse value) => value.writeToBuffer()));
    $addMethod(
        $grpc.ServiceMethod<$1.AuthorizationRequest, $1.AuthorizationResponse>(
            'Authorize',
            authorize_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $1.AuthorizationRequest.fromBuffer(value),
            ($1.AuthorizationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$1.GetPublicProfileRequest,
            $1.GetPublicProfileResponse>(
        'GetPublicProfile',
        getPublicProfile_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $1.GetPublicProfileRequest.fromBuffer(value),
        ($1.GetPublicProfileResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$1.UpdatePublicProfileRequest,
            $1.UpdatePublicProfileResponse>(
        'UpdatePublicProfile',
        updatePublicProfile_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $1.UpdatePublicProfileRequest.fromBuffer(value),
        ($1.UpdatePublicProfileResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$1.SearchUserRequest, $1.SearchUserResponse>(
        'Search',
        search_Pre,
        false,
        false,
        ($core.List<$core.int> value) => $1.SearchUserRequest.fromBuffer(value),
        ($1.SearchUserResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$1.PrepareNewAvatarRequest,
            $1.PrepareNewAvatarResponse>(
        'PrepareNewAvatar',
        prepareNewAvatar_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $1.PrepareNewAvatarRequest.fromBuffer(value),
        ($1.PrepareNewAvatarResponse value) => value.writeToBuffer()));
  }

  $async.Future<$1.RegistrationResponse> register_Pre($grpc.ServiceCall call,
      $async.Future<$1.RegistrationRequest> request) async {
    return register(call, await request);
  }

  $async.Future<$1.AuthorizationResponse> authorize_Pre($grpc.ServiceCall call,
      $async.Future<$1.AuthorizationRequest> request) async {
    return authorize(call, await request);
  }

  $async.Future<$1.GetPublicProfileResponse> getPublicProfile_Pre(
      $grpc.ServiceCall call,
      $async.Future<$1.GetPublicProfileRequest> request) async {
    return getPublicProfile(call, await request);
  }

  $async.Future<$1.UpdatePublicProfileResponse> updatePublicProfile_Pre(
      $grpc.ServiceCall call,
      $async.Future<$1.UpdatePublicProfileRequest> request) async {
    return updatePublicProfile(call, await request);
  }

  $async.Future<$1.SearchUserResponse> search_Pre($grpc.ServiceCall call,
      $async.Future<$1.SearchUserRequest> request) async {
    return search(call, await request);
  }

  $async.Future<$1.PrepareNewAvatarResponse> prepareNewAvatar_Pre(
      $grpc.ServiceCall call,
      $async.Future<$1.PrepareNewAvatarRequest> request) async {
    return prepareNewAvatar(call, await request);
  }

  $async.Future<$1.RegistrationResponse> register(
      $grpc.ServiceCall call, $1.RegistrationRequest request);
  $async.Future<$1.AuthorizationResponse> authorize(
      $grpc.ServiceCall call, $1.AuthorizationRequest request);
  $async.Future<$1.GetPublicProfileResponse> getPublicProfile(
      $grpc.ServiceCall call, $1.GetPublicProfileRequest request);
  $async.Future<$1.UpdatePublicProfileResponse> updatePublicProfile(
      $grpc.ServiceCall call, $1.UpdatePublicProfileRequest request);
  $async.Future<$1.SearchUserResponse> search(
      $grpc.ServiceCall call, $1.SearchUserRequest request);
  $async.Future<$1.PrepareNewAvatarResponse> prepareNewAvatar(
      $grpc.ServiceCall call, $1.PrepareNewAvatarRequest request);
}
