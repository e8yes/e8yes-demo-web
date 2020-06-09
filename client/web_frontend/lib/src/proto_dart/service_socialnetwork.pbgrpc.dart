///
//  Generated code. Do not modify.
//  source: service_socialnetwork.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_socialnetwork.pb.dart' as $3;
export 'service_socialnetwork.pb.dart';

class SocialNetworkServiceClient extends $grpc.Client {
  static final _$sendInvitation =
      $grpc.ClientMethod<$3.SendInvitationRequest, $3.SendInvitationResponse>(
          '/e8.SocialNetworkService/SendInvitation',
          ($3.SendInvitationRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $3.SendInvitationResponse.fromBuffer(value));
  static final _$deleteInvitation = $grpc.ClientMethod<
          $3.DeleteInvitationRequest, $3.DeleteInvitationResponse>(
      '/e8.SocialNetworkService/DeleteInvitation',
      ($3.DeleteInvitationRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $3.DeleteInvitationResponse.fromBuffer(value));
  static final _$acceptInvitation = $grpc.ClientMethod<
          $3.AcceptInvitationRequest, $3.AcceptInvitationResponse>(
      '/e8.SocialNetworkService/AcceptInvitation',
      ($3.AcceptInvitationRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $3.AcceptInvitationResponse.fromBuffer(value));
  static final _$rejectInvitation = $grpc.ClientMethod<
          $3.RejectInvitationRequest, $3.RejectInvitationResponse>(
      '/e8.SocialNetworkService/RejectInvitation',
      ($3.RejectInvitationRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $3.RejectInvitationResponse.fromBuffer(value));
  static final _$getInvitationList = $grpc.ClientMethod<
          $3.GetInvitationListRequest, $3.GetInvitationListResponse>(
      '/e8.SocialNetworkService/GetInvitationList',
      ($3.GetInvitationListRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $3.GetInvitationListResponse.fromBuffer(value));
  static final _$deleteFriend =
      $grpc.ClientMethod<$3.DeleteFriendRequest, $3.DeleteFriendResponse>(
          '/e8.SocialNetworkService/DeleteFriend',
          ($3.DeleteFriendRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $3.DeleteFriendResponse.fromBuffer(value));
  static final _$getFriendList =
      $grpc.ClientMethod<$3.GetFriendListRequest, $3.GetFriendListResponse>(
          '/e8.SocialNetworkService/GetFriendList',
          ($3.GetFriendListRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $3.GetFriendListResponse.fromBuffer(value));

  SocialNetworkServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$3.SendInvitationResponse> sendInvitation(
      $3.SendInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$sendInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$3.DeleteInvitationResponse> deleteInvitation(
      $3.DeleteInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$deleteInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$3.AcceptInvitationResponse> acceptInvitation(
      $3.AcceptInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$acceptInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$3.RejectInvitationResponse> rejectInvitation(
      $3.RejectInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$rejectInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$3.GetInvitationListResponse> getInvitationList(
      $3.GetInvitationListRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getInvitationList, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$3.DeleteFriendResponse> deleteFriend(
      $3.DeleteFriendRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$deleteFriend, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$3.GetFriendListResponse> getFriendList(
      $3.GetFriendListRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getFriendList, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }
}

abstract class SocialNetworkServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.SocialNetworkService';

  SocialNetworkServiceBase() {
    $addMethod($grpc.ServiceMethod<$3.SendInvitationRequest,
            $3.SendInvitationResponse>(
        'SendInvitation',
        sendInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $3.SendInvitationRequest.fromBuffer(value),
        ($3.SendInvitationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$3.DeleteInvitationRequest,
            $3.DeleteInvitationResponse>(
        'DeleteInvitation',
        deleteInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $3.DeleteInvitationRequest.fromBuffer(value),
        ($3.DeleteInvitationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$3.AcceptInvitationRequest,
            $3.AcceptInvitationResponse>(
        'AcceptInvitation',
        acceptInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $3.AcceptInvitationRequest.fromBuffer(value),
        ($3.AcceptInvitationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$3.RejectInvitationRequest,
            $3.RejectInvitationResponse>(
        'RejectInvitation',
        rejectInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $3.RejectInvitationRequest.fromBuffer(value),
        ($3.RejectInvitationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$3.GetInvitationListRequest,
            $3.GetInvitationListResponse>(
        'GetInvitationList',
        getInvitationList_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $3.GetInvitationListRequest.fromBuffer(value),
        ($3.GetInvitationListResponse value) => value.writeToBuffer()));
    $addMethod(
        $grpc.ServiceMethod<$3.DeleteFriendRequest, $3.DeleteFriendResponse>(
            'DeleteFriend',
            deleteFriend_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $3.DeleteFriendRequest.fromBuffer(value),
            ($3.DeleteFriendResponse value) => value.writeToBuffer()));
    $addMethod(
        $grpc.ServiceMethod<$3.GetFriendListRequest, $3.GetFriendListResponse>(
            'GetFriendList',
            getFriendList_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $3.GetFriendListRequest.fromBuffer(value),
            ($3.GetFriendListResponse value) => value.writeToBuffer()));
  }

  $async.Future<$3.SendInvitationResponse> sendInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.SendInvitationRequest> request) async {
    return sendInvitation(call, await request);
  }

  $async.Future<$3.DeleteInvitationResponse> deleteInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.DeleteInvitationRequest> request) async {
    return deleteInvitation(call, await request);
  }

  $async.Future<$3.AcceptInvitationResponse> acceptInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.AcceptInvitationRequest> request) async {
    return acceptInvitation(call, await request);
  }

  $async.Future<$3.RejectInvitationResponse> rejectInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.RejectInvitationRequest> request) async {
    return rejectInvitation(call, await request);
  }

  $async.Future<$3.GetInvitationListResponse> getInvitationList_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.GetInvitationListRequest> request) async {
    return getInvitationList(call, await request);
  }

  $async.Future<$3.DeleteFriendResponse> deleteFriend_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.DeleteFriendRequest> request) async {
    return deleteFriend(call, await request);
  }

  $async.Future<$3.GetFriendListResponse> getFriendList_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.GetFriendListRequest> request) async {
    return getFriendList(call, await request);
  }

  $async.Future<$3.SendInvitationResponse> sendInvitation(
      $grpc.ServiceCall call, $3.SendInvitationRequest request);
  $async.Future<$3.DeleteInvitationResponse> deleteInvitation(
      $grpc.ServiceCall call, $3.DeleteInvitationRequest request);
  $async.Future<$3.AcceptInvitationResponse> acceptInvitation(
      $grpc.ServiceCall call, $3.AcceptInvitationRequest request);
  $async.Future<$3.RejectInvitationResponse> rejectInvitation(
      $grpc.ServiceCall call, $3.RejectInvitationRequest request);
  $async.Future<$3.GetInvitationListResponse> getInvitationList(
      $grpc.ServiceCall call, $3.GetInvitationListRequest request);
  $async.Future<$3.DeleteFriendResponse> deleteFriend(
      $grpc.ServiceCall call, $3.DeleteFriendRequest request);
  $async.Future<$3.GetFriendListResponse> getFriendList(
      $grpc.ServiceCall call, $3.GetFriendListRequest request);
}
