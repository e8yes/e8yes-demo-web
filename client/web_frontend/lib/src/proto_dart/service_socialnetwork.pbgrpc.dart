///
//  Generated code. Do not modify.
//  source: service_socialnetwork.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_socialnetwork.pb.dart' as $7;
export 'service_socialnetwork.pb.dart';

class SocialNetworkServiceClient extends $grpc.Client {
  static final _$getUserRelations = $grpc.ClientMethod<
          $7.GetUserRelationsRequest, $7.GetUserRelationsResponse>(
      '/e8.SocialNetworkService/GetUserRelations',
      ($7.GetUserRelationsRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $7.GetUserRelationsResponse.fromBuffer(value));
  static final _$sendInvitation =
      $grpc.ClientMethod<$7.SendInvitationRequest, $7.SendInvitationResponse>(
          '/e8.SocialNetworkService/SendInvitation',
          ($7.SendInvitationRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $7.SendInvitationResponse.fromBuffer(value));
  static final _$deleteInvitation = $grpc.ClientMethod<
          $7.DeleteInvitationRequest, $7.DeleteInvitationResponse>(
      '/e8.SocialNetworkService/DeleteInvitation',
      ($7.DeleteInvitationRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $7.DeleteInvitationResponse.fromBuffer(value));
  static final _$processInvitation = $grpc.ClientMethod<
          $7.ProcessInvitationRequest, $7.ProcessInvitationResponse>(
      '/e8.SocialNetworkService/ProcessInvitation',
      ($7.ProcessInvitationRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $7.ProcessInvitationResponse.fromBuffer(value));
  static final _$deleteContact =
      $grpc.ClientMethod<$7.DeleteContactRequest, $7.DeleteContactResponse>(
          '/e8.SocialNetworkService/DeleteContact',
          ($7.DeleteContactRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $7.DeleteContactResponse.fromBuffer(value));
  static final _$getRelatedUserList = $grpc.ClientMethod<
          $7.GetRelatedUserListRequest, $7.GetRelatedUserListResponse>(
      '/e8.SocialNetworkService/GetRelatedUserList',
      ($7.GetRelatedUserListRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $7.GetRelatedUserListResponse.fromBuffer(value));

  SocialNetworkServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$7.GetUserRelationsResponse> getUserRelations(
      $7.GetUserRelationsRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getUserRelations, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$7.SendInvitationResponse> sendInvitation(
      $7.SendInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$sendInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$7.DeleteInvitationResponse> deleteInvitation(
      $7.DeleteInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$deleteInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$7.ProcessInvitationResponse> processInvitation(
      $7.ProcessInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$processInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$7.DeleteContactResponse> deleteContact(
      $7.DeleteContactRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$deleteContact, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$7.GetRelatedUserListResponse> getRelatedUserList(
      $7.GetRelatedUserListRequest request,
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
    $addMethod($grpc.ServiceMethod<$7.GetUserRelationsRequest,
            $7.GetUserRelationsResponse>(
        'GetUserRelations',
        getUserRelations_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $7.GetUserRelationsRequest.fromBuffer(value),
        ($7.GetUserRelationsResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$7.SendInvitationRequest,
            $7.SendInvitationResponse>(
        'SendInvitation',
        sendInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $7.SendInvitationRequest.fromBuffer(value),
        ($7.SendInvitationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$7.DeleteInvitationRequest,
            $7.DeleteInvitationResponse>(
        'DeleteInvitation',
        deleteInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $7.DeleteInvitationRequest.fromBuffer(value),
        ($7.DeleteInvitationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$7.ProcessInvitationRequest,
            $7.ProcessInvitationResponse>(
        'ProcessInvitation',
        processInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $7.ProcessInvitationRequest.fromBuffer(value),
        ($7.ProcessInvitationResponse value) => value.writeToBuffer()));
    $addMethod(
        $grpc.ServiceMethod<$7.DeleteContactRequest, $7.DeleteContactResponse>(
            'DeleteContact',
            deleteContact_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $7.DeleteContactRequest.fromBuffer(value),
            ($7.DeleteContactResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$7.GetRelatedUserListRequest,
            $7.GetRelatedUserListResponse>(
        'GetRelatedUserList',
        getRelatedUserList_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $7.GetRelatedUserListRequest.fromBuffer(value),
        ($7.GetRelatedUserListResponse value) => value.writeToBuffer()));
  }

  $async.Future<$7.GetUserRelationsResponse> getUserRelations_Pre(
      $grpc.ServiceCall call,
      $async.Future<$7.GetUserRelationsRequest> request) async {
    return getUserRelations(call, await request);
  }

  $async.Future<$7.SendInvitationResponse> sendInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$7.SendInvitationRequest> request) async {
    return sendInvitation(call, await request);
  }

  $async.Future<$7.DeleteInvitationResponse> deleteInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$7.DeleteInvitationRequest> request) async {
    return deleteInvitation(call, await request);
  }

  $async.Future<$7.ProcessInvitationResponse> processInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$7.ProcessInvitationRequest> request) async {
    return processInvitation(call, await request);
  }

  $async.Future<$7.DeleteContactResponse> deleteContact_Pre(
      $grpc.ServiceCall call,
      $async.Future<$7.DeleteContactRequest> request) async {
    return deleteContact(call, await request);
  }

  $async.Future<$7.GetRelatedUserListResponse> getRelatedUserList_Pre(
      $grpc.ServiceCall call,
      $async.Future<$7.GetRelatedUserListRequest> request) async {
    return getRelatedUserList(call, await request);
  }

  $async.Future<$7.GetUserRelationsResponse> getUserRelations(
      $grpc.ServiceCall call, $7.GetUserRelationsRequest request);
  $async.Future<$7.SendInvitationResponse> sendInvitation(
      $grpc.ServiceCall call, $7.SendInvitationRequest request);
  $async.Future<$7.DeleteInvitationResponse> deleteInvitation(
      $grpc.ServiceCall call, $7.DeleteInvitationRequest request);
  $async.Future<$7.ProcessInvitationResponse> processInvitation(
      $grpc.ServiceCall call, $7.ProcessInvitationRequest request);
  $async.Future<$7.DeleteContactResponse> deleteContact(
      $grpc.ServiceCall call, $7.DeleteContactRequest request);
  $async.Future<$7.GetRelatedUserListResponse> getRelatedUserList(
      $grpc.ServiceCall call, $7.GetRelatedUserListRequest request);
}
