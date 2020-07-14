///
//  Generated code. Do not modify.
//  source: service_socialnetwork.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_socialnetwork.pb.dart' as $5;
export 'service_socialnetwork.pb.dart';

class SocialNetworkServiceClient extends $grpc.Client {
  static final _$getUserRelations = $grpc.ClientMethod<
          $5.GetUserRelationsRequest, $5.GetUserRelationsResponse>(
      '/e8.SocialNetworkService/GetUserRelations',
      ($5.GetUserRelationsRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $5.GetUserRelationsResponse.fromBuffer(value));
  static final _$sendInvitation =
      $grpc.ClientMethod<$5.SendInvitationRequest, $5.SendInvitationResponse>(
          '/e8.SocialNetworkService/SendInvitation',
          ($5.SendInvitationRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $5.SendInvitationResponse.fromBuffer(value));
  static final _$deleteInvitation = $grpc.ClientMethod<
          $5.DeleteInvitationRequest, $5.DeleteInvitationResponse>(
      '/e8.SocialNetworkService/DeleteInvitation',
      ($5.DeleteInvitationRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $5.DeleteInvitationResponse.fromBuffer(value));
  static final _$processInvitation = $grpc.ClientMethod<
          $5.ProcessInvitationRequest, $5.ProcessInvitationResponse>(
      '/e8.SocialNetworkService/ProcessInvitation',
      ($5.ProcessInvitationRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $5.ProcessInvitationResponse.fromBuffer(value));
  static final _$deleteContact =
      $grpc.ClientMethod<$5.DeleteContactRequest, $5.DeleteContactResponse>(
          '/e8.SocialNetworkService/DeleteContact',
          ($5.DeleteContactRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $5.DeleteContactResponse.fromBuffer(value));
  static final _$getRelatedUserList = $grpc.ClientMethod<
          $5.GetRelatedUserListRequest, $5.GetRelatedUserListResponse>(
      '/e8.SocialNetworkService/GetRelatedUserList',
      ($5.GetRelatedUserListRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $5.GetRelatedUserListResponse.fromBuffer(value));

  SocialNetworkServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$5.GetUserRelationsResponse> getUserRelations(
      $5.GetUserRelationsRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getUserRelations, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$5.SendInvitationResponse> sendInvitation(
      $5.SendInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$sendInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$5.DeleteInvitationResponse> deleteInvitation(
      $5.DeleteInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$deleteInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$5.ProcessInvitationResponse> processInvitation(
      $5.ProcessInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$processInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$5.DeleteContactResponse> deleteContact(
      $5.DeleteContactRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$deleteContact, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$5.GetRelatedUserListResponse> getRelatedUserList(
      $5.GetRelatedUserListRequest request,
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
    $addMethod($grpc.ServiceMethod<$5.GetUserRelationsRequest,
            $5.GetUserRelationsResponse>(
        'GetUserRelations',
        getUserRelations_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $5.GetUserRelationsRequest.fromBuffer(value),
        ($5.GetUserRelationsResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$5.SendInvitationRequest,
            $5.SendInvitationResponse>(
        'SendInvitation',
        sendInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $5.SendInvitationRequest.fromBuffer(value),
        ($5.SendInvitationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$5.DeleteInvitationRequest,
            $5.DeleteInvitationResponse>(
        'DeleteInvitation',
        deleteInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $5.DeleteInvitationRequest.fromBuffer(value),
        ($5.DeleteInvitationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$5.ProcessInvitationRequest,
            $5.ProcessInvitationResponse>(
        'ProcessInvitation',
        processInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $5.ProcessInvitationRequest.fromBuffer(value),
        ($5.ProcessInvitationResponse value) => value.writeToBuffer()));
    $addMethod(
        $grpc.ServiceMethod<$5.DeleteContactRequest, $5.DeleteContactResponse>(
            'DeleteContact',
            deleteContact_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $5.DeleteContactRequest.fromBuffer(value),
            ($5.DeleteContactResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$5.GetRelatedUserListRequest,
            $5.GetRelatedUserListResponse>(
        'GetRelatedUserList',
        getRelatedUserList_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $5.GetRelatedUserListRequest.fromBuffer(value),
        ($5.GetRelatedUserListResponse value) => value.writeToBuffer()));
  }

  $async.Future<$5.GetUserRelationsResponse> getUserRelations_Pre(
      $grpc.ServiceCall call,
      $async.Future<$5.GetUserRelationsRequest> request) async {
    return getUserRelations(call, await request);
  }

  $async.Future<$5.SendInvitationResponse> sendInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$5.SendInvitationRequest> request) async {
    return sendInvitation(call, await request);
  }

  $async.Future<$5.DeleteInvitationResponse> deleteInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$5.DeleteInvitationRequest> request) async {
    return deleteInvitation(call, await request);
  }

  $async.Future<$5.ProcessInvitationResponse> processInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$5.ProcessInvitationRequest> request) async {
    return processInvitation(call, await request);
  }

  $async.Future<$5.DeleteContactResponse> deleteContact_Pre(
      $grpc.ServiceCall call,
      $async.Future<$5.DeleteContactRequest> request) async {
    return deleteContact(call, await request);
  }

  $async.Future<$5.GetRelatedUserListResponse> getRelatedUserList_Pre(
      $grpc.ServiceCall call,
      $async.Future<$5.GetRelatedUserListRequest> request) async {
    return getRelatedUserList(call, await request);
  }

  $async.Future<$5.GetUserRelationsResponse> getUserRelations(
      $grpc.ServiceCall call, $5.GetUserRelationsRequest request);
  $async.Future<$5.SendInvitationResponse> sendInvitation(
      $grpc.ServiceCall call, $5.SendInvitationRequest request);
  $async.Future<$5.DeleteInvitationResponse> deleteInvitation(
      $grpc.ServiceCall call, $5.DeleteInvitationRequest request);
  $async.Future<$5.ProcessInvitationResponse> processInvitation(
      $grpc.ServiceCall call, $5.ProcessInvitationRequest request);
  $async.Future<$5.DeleteContactResponse> deleteContact(
      $grpc.ServiceCall call, $5.DeleteContactRequest request);
  $async.Future<$5.GetRelatedUserListResponse> getRelatedUserList(
      $grpc.ServiceCall call, $5.GetRelatedUserListRequest request);
}
