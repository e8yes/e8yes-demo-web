///
//  Generated code. Do not modify.
//  source: service_socialnetwork.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_socialnetwork.pb.dart' as $8;
export 'service_socialnetwork.pb.dart';

class SocialNetworkServiceClient extends $grpc.Client {
  static final _$getUserRelations = $grpc.ClientMethod<
          $8.GetUserRelationsRequest, $8.GetUserRelationsResponse>(
      '/e8.SocialNetworkService/GetUserRelations',
      ($8.GetUserRelationsRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $8.GetUserRelationsResponse.fromBuffer(value));
  static final _$sendInvitation =
      $grpc.ClientMethod<$8.SendInvitationRequest, $8.SendInvitationResponse>(
          '/e8.SocialNetworkService/SendInvitation',
          ($8.SendInvitationRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $8.SendInvitationResponse.fromBuffer(value));
  static final _$deleteInvitation = $grpc.ClientMethod<
          $8.DeleteInvitationRequest, $8.DeleteInvitationResponse>(
      '/e8.SocialNetworkService/DeleteInvitation',
      ($8.DeleteInvitationRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $8.DeleteInvitationResponse.fromBuffer(value));
  static final _$processInvitation = $grpc.ClientMethod<
          $8.ProcessInvitationRequest, $8.ProcessInvitationResponse>(
      '/e8.SocialNetworkService/ProcessInvitation',
      ($8.ProcessInvitationRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $8.ProcessInvitationResponse.fromBuffer(value));
  static final _$deleteContact =
      $grpc.ClientMethod<$8.DeleteContactRequest, $8.DeleteContactResponse>(
          '/e8.SocialNetworkService/DeleteContact',
          ($8.DeleteContactRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $8.DeleteContactResponse.fromBuffer(value));
  static final _$searchRelatedUserList = $grpc.ClientMethod<
          $8.SearchRelatedUserListRequest, $8.SearchRelatedUserListResponse>(
      '/e8.SocialNetworkService/SearchRelatedUserList',
      ($8.SearchRelatedUserListRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $8.SearchRelatedUserListResponse.fromBuffer(value));

  SocialNetworkServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$8.GetUserRelationsResponse> getUserRelations(
      $8.GetUserRelationsRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getUserRelations, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$8.SendInvitationResponse> sendInvitation(
      $8.SendInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$sendInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$8.DeleteInvitationResponse> deleteInvitation(
      $8.DeleteInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$deleteInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$8.ProcessInvitationResponse> processInvitation(
      $8.ProcessInvitationRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$processInvitation, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$8.DeleteContactResponse> deleteContact(
      $8.DeleteContactRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$deleteContact, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$8.SearchRelatedUserListResponse> searchRelatedUserList(
      $8.SearchRelatedUserListRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$searchRelatedUserList, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }
}

abstract class SocialNetworkServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.SocialNetworkService';

  SocialNetworkServiceBase() {
    $addMethod($grpc.ServiceMethod<$8.GetUserRelationsRequest,
            $8.GetUserRelationsResponse>(
        'GetUserRelations',
        getUserRelations_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $8.GetUserRelationsRequest.fromBuffer(value),
        ($8.GetUserRelationsResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$8.SendInvitationRequest,
            $8.SendInvitationResponse>(
        'SendInvitation',
        sendInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $8.SendInvitationRequest.fromBuffer(value),
        ($8.SendInvitationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$8.DeleteInvitationRequest,
            $8.DeleteInvitationResponse>(
        'DeleteInvitation',
        deleteInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $8.DeleteInvitationRequest.fromBuffer(value),
        ($8.DeleteInvitationResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$8.ProcessInvitationRequest,
            $8.ProcessInvitationResponse>(
        'ProcessInvitation',
        processInvitation_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $8.ProcessInvitationRequest.fromBuffer(value),
        ($8.ProcessInvitationResponse value) => value.writeToBuffer()));
    $addMethod(
        $grpc.ServiceMethod<$8.DeleteContactRequest, $8.DeleteContactResponse>(
            'DeleteContact',
            deleteContact_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $8.DeleteContactRequest.fromBuffer(value),
            ($8.DeleteContactResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$8.SearchRelatedUserListRequest,
            $8.SearchRelatedUserListResponse>(
        'SearchRelatedUserList',
        searchRelatedUserList_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $8.SearchRelatedUserListRequest.fromBuffer(value),
        ($8.SearchRelatedUserListResponse value) => value.writeToBuffer()));
  }

  $async.Future<$8.GetUserRelationsResponse> getUserRelations_Pre(
      $grpc.ServiceCall call,
      $async.Future<$8.GetUserRelationsRequest> request) async {
    return getUserRelations(call, await request);
  }

  $async.Future<$8.SendInvitationResponse> sendInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$8.SendInvitationRequest> request) async {
    return sendInvitation(call, await request);
  }

  $async.Future<$8.DeleteInvitationResponse> deleteInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$8.DeleteInvitationRequest> request) async {
    return deleteInvitation(call, await request);
  }

  $async.Future<$8.ProcessInvitationResponse> processInvitation_Pre(
      $grpc.ServiceCall call,
      $async.Future<$8.ProcessInvitationRequest> request) async {
    return processInvitation(call, await request);
  }

  $async.Future<$8.DeleteContactResponse> deleteContact_Pre(
      $grpc.ServiceCall call,
      $async.Future<$8.DeleteContactRequest> request) async {
    return deleteContact(call, await request);
  }

  $async.Future<$8.SearchRelatedUserListResponse> searchRelatedUserList_Pre(
      $grpc.ServiceCall call,
      $async.Future<$8.SearchRelatedUserListRequest> request) async {
    return searchRelatedUserList(call, await request);
  }

  $async.Future<$8.GetUserRelationsResponse> getUserRelations(
      $grpc.ServiceCall call, $8.GetUserRelationsRequest request);
  $async.Future<$8.SendInvitationResponse> sendInvitation(
      $grpc.ServiceCall call, $8.SendInvitationRequest request);
  $async.Future<$8.DeleteInvitationResponse> deleteInvitation(
      $grpc.ServiceCall call, $8.DeleteInvitationRequest request);
  $async.Future<$8.ProcessInvitationResponse> processInvitation(
      $grpc.ServiceCall call, $8.ProcessInvitationRequest request);
  $async.Future<$8.DeleteContactResponse> deleteContact(
      $grpc.ServiceCall call, $8.DeleteContactRequest request);
  $async.Future<$8.SearchRelatedUserListResponse> searchRelatedUserList(
      $grpc.ServiceCall call, $8.SearchRelatedUserListRequest request);
}
