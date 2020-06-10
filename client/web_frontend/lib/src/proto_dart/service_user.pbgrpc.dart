///
//  Generated code. Do not modify.
//  source: service_user.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_user.pb.dart' as $0;
export 'service_user.pb.dart';

class UserServiceClient extends $grpc.Client {
  static final _$register =
      $grpc.ClientMethod<$0.RegistrationRequest, $0.RegistrationResponse>(
          '/e8.UserService/Register',
          ($0.RegistrationRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $0.RegistrationResponse.fromBuffer(value));
  static final _$authorize =
      $grpc.ClientMethod<$0.AuthorizationRequest, $0.AuthorizationResponse>(
          '/e8.UserService/Authorize',
          ($0.AuthorizationRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $0.AuthorizationResponse.fromBuffer(value));
  static final _$getPublicProfile = $grpc.ClientMethod<
          $0.GetPublicProfileRequest, $0.GetPublicProfileResponse>(
      '/e8.UserService/GetPublicProfile',
      ($0.GetPublicProfileRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $0.GetPublicProfileResponse.fromBuffer(value));
  static final _$updatePublicProfile = $grpc.ClientMethod<
          $0.UpdatePublicProfileRequest, $0.UpdatePublicProfileResponse>(
      '/e8.UserService/UpdatePublicProfile',
      ($0.UpdatePublicProfileRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $0.UpdatePublicProfileResponse.fromBuffer(value));
  static final _$search =
      $grpc.ClientMethod<$0.SearchUserRequest, $0.SearchUserResponse>(
          '/e8.UserService/Search',
          ($0.SearchUserRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $0.SearchUserResponse.fromBuffer(value));
  static final _$prepareNewAvatar = $grpc.ClientMethod<
          $0.PrepareNewAvatarRequest, $0.PrepareNewAvatarResponse>(
      '/e8.UserService/PrepareNewAvatar',
      ($0.PrepareNewAvatarRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $0.PrepareNewAvatarResponse.fromBuffer(value));

  UserServiceClient($grpc.ClientChannel channel, {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$0.RegistrationResponse> register(
      $0.RegistrationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$register, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$0.AuthorizationResponse> authorize(
      $0.AuthorizationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$authorize, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$0.GetPublicProfileResponse> getPublicProfile(
      $0.GetPublicProfileRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getPublicProfile, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$0.UpdatePublicProfileResponse> updatePublicProfile(
      $0.UpdatePublicProfileRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$updatePublicProfile, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$0.SearchUserResponse> search(
      $0.SearchUserRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$search, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$0.PrepareNewAvatarResponse> prepareNewAvatar(
      $0.PrepareNewAvatarRequest request,
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
        $grpc.ServiceMethod<$0.RegistrationRequest, $0.RegistrationResponse>(
            'Register',
            register_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $0.RegistrationRequest.fromBuffer(value),
            ($0.RegistrationResponse value) => value.writeToBuffer()));
    $addMethod(
        $grpc.ServiceMethod<$0.AuthorizationRequest, $0.AuthorizationResponse>(
            'Authorize',
            authorize_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $0.AuthorizationRequest.fromBuffer(value),
            ($0.AuthorizationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$0.GetPublicProfileRequest,
            $0.GetPublicProfileResponse>(
        'GetPublicProfile',
        getPublicProfile_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $0.GetPublicProfileRequest.fromBuffer(value),
        ($0.GetPublicProfileResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$0.UpdatePublicProfileRequest,
            $0.UpdatePublicProfileResponse>(
        'UpdatePublicProfile',
        updatePublicProfile_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $0.UpdatePublicProfileRequest.fromBuffer(value),
        ($0.UpdatePublicProfileResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$0.SearchUserRequest, $0.SearchUserResponse>(
        'Search',
        search_Pre,
        false,
        false,
        ($core.List<$core.int> value) => $0.SearchUserRequest.fromBuffer(value),
        ($0.SearchUserResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$0.PrepareNewAvatarRequest,
            $0.PrepareNewAvatarResponse>(
        'PrepareNewAvatar',
        prepareNewAvatar_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $0.PrepareNewAvatarRequest.fromBuffer(value),
        ($0.PrepareNewAvatarResponse value) => value.writeToBuffer()));
  }

  $async.Future<$0.RegistrationResponse> register_Pre($grpc.ServiceCall call,
      $async.Future<$0.RegistrationRequest> request) async {
    return register(call, await request);
  }

  $async.Future<$0.AuthorizationResponse> authorize_Pre($grpc.ServiceCall call,
      $async.Future<$0.AuthorizationRequest> request) async {
    return authorize(call, await request);
  }

  $async.Future<$0.GetPublicProfileResponse> getPublicProfile_Pre(
      $grpc.ServiceCall call,
      $async.Future<$0.GetPublicProfileRequest> request) async {
    return getPublicProfile(call, await request);
  }

  $async.Future<$0.UpdatePublicProfileResponse> updatePublicProfile_Pre(
      $grpc.ServiceCall call,
      $async.Future<$0.UpdatePublicProfileRequest> request) async {
    return updatePublicProfile(call, await request);
  }

  $async.Future<$0.SearchUserResponse> search_Pre($grpc.ServiceCall call,
      $async.Future<$0.SearchUserRequest> request) async {
    return search(call, await request);
  }

  $async.Future<$0.PrepareNewAvatarResponse> prepareNewAvatar_Pre(
      $grpc.ServiceCall call,
      $async.Future<$0.PrepareNewAvatarRequest> request) async {
    return prepareNewAvatar(call, await request);
  }

  $async.Future<$0.RegistrationResponse> register(
      $grpc.ServiceCall call, $0.RegistrationRequest request);
  $async.Future<$0.AuthorizationResponse> authorize(
      $grpc.ServiceCall call, $0.AuthorizationRequest request);
  $async.Future<$0.GetPublicProfileResponse> getPublicProfile(
      $grpc.ServiceCall call, $0.GetPublicProfileRequest request);
  $async.Future<$0.UpdatePublicProfileResponse> updatePublicProfile(
      $grpc.ServiceCall call, $0.UpdatePublicProfileRequest request);
  $async.Future<$0.SearchUserResponse> search(
      $grpc.ServiceCall call, $0.SearchUserRequest request);
  $async.Future<$0.PrepareNewAvatarResponse> prepareNewAvatar(
      $grpc.ServiceCall call, $0.PrepareNewAvatarRequest request);
}
