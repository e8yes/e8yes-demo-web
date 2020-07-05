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
  static final _$getUserRelations = $grpc.ClientMethod<
          $3.GetUserRelationsRequest, $3.GetUserRelationsResponse>(
      '/e8.SocialNetworkService/GetUserRelations',
      ($3.GetUserRelationsRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $3.GetUserRelationsResponse.fromBuffer(value));
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
  static final _$processInvitation = $grpc.ClientMethod<
          $3.ProcessInvitationRequest, $3.ProcessInvitationResponse>(
      '/e8.SocialNetworkService/ProcessInvitation',
      ($3.ProcessInvitationRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $3.ProcessInvitationResponse.fromBuffer(value));
  static final _$deleteContact =
      $grpc.ClientMethod<$3.DeleteContactRequest, $3.DeleteContactResponse>(
          '/e8.SocialNetworkService/DeleteContact',
          ($3.DeleteContactRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $3.DeleteContactResponse.fromBuffer(value));
  static final _$getRelatedUserList = $grpc.ClientMethod<
          $3.GetRelatedUserListRequest, $3.GetRelatedUserListResponse>(
      '/e8.SocialNetworkService/GetRelatedUserList',
      ($3.GetRelatedUserListRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $3.GetRelatedUserListResponse.fromBuffer(value));

  SocialNetworkServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$3.GetUserRelationsResponse> getUserRelations(
      $3.GetUserRelationsRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getUserRelations, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

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

  $grpc.ResponseFuture<$3.ProcessInvitationResponse> processInvitation(
      $3.ProcessInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$processInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$3.DeleteContactResponse> deleteContact(
      $3.DeleteContactRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$deleteContact, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$3.GetRelatedUserListResponse> getRelatedUserList(
      $3.GetRelatedUserListRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getRelatedUserList, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }
}

abstract class SocialNetworkServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.SocialNetworkService';

  SocialNetworkServiceBase() {
    $addMethod($grpc.ServiceMethod<$3.GetUserRelationsRequest,
            $3.GetUserRelationsResponse>(
        'GetUserRelations',
        getUserRelations_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $3.GetUserRelationsRequest.fromBuffer(value),
        ($3.GetUserRelationsResponse value) => value.writeToBuffer()));
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
    $addMethod($grpc.ServiceMethod<$3.ProcessInvitationRequest,
            $3.ProcessInvitationResponse>(
        'ProcessInvitation',
        processInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $3.ProcessInvitationRequest.fromBuffer(value),
        ($3.ProcessInvitationResponse value) => value.writeToBuffer()));
    $addMethod(
        $grpc.ServiceMethod<$3.DeleteContactRequest, $3.DeleteContactResponse>(
            'DeleteContact',
            deleteContact_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $3.DeleteContactRequest.fromBuffer(value),
            ($3.DeleteContactResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$3.GetRelatedUserListRequest,
            $3.GetRelatedUserListResponse>(
        'GetRelatedUserList',
        getRelatedUserList_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $3.GetRelatedUserListRequest.fromBuffer(value),
        ($3.GetRelatedUserListResponse value) => value.writeToBuffer()));
  }

  $async.Future<$3.GetUserRelationsResponse> getUserRelations_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.GetUserRelationsRequest> request) async {
    return getUserRelations(call, await request);
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

  $async.Future<$3.ProcessInvitationResponse> processInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.ProcessInvitationRequest> request) async {
    return processInvitation(call, await request);
  }

  $async.Future<$3.DeleteContactResponse> deleteContact_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.DeleteContactRequest> request) async {
    return deleteContact(call, await request);
  }

  $async.Future<$3.GetRelatedUserListResponse> getRelatedUserList_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.GetRelatedUserListRequest> request) async {
    return getRelatedUserList(call, await request);
  }

  $async.Future<$3.GetUserRelationsResponse> getUserRelations(
      $grpc.ServiceCall call, $3.GetUserRelationsRequest request);
  $async.Future<$3.SendInvitationResponse> sendInvitation(
      $grpc.ServiceCall call, $3.SendInvitationRequest request);
  $async.Future<$3.DeleteInvitationResponse> deleteInvitation(
      $grpc.ServiceCall call, $3.DeleteInvitationRequest request);
  $async.Future<$3.ProcessInvitationResponse> processInvitation(
      $grpc.ServiceCall call, $3.ProcessInvitationRequest request);
  $async.Future<$3.DeleteContactResponse> deleteContact(
      $grpc.ServiceCall call, $3.DeleteContactRequest request);
  $async.Future<$3.GetRelatedUserListResponse> getRelatedUserList(
      $grpc.ServiceCall call, $3.GetRelatedUserListRequest request);
}
