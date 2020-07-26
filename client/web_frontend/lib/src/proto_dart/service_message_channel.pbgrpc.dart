///
//  Generated code. Do not modify.
//  source: service_message_channel.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_message_channel.pb.dart' as $6;
export 'service_message_channel.pb.dart';

class MessageChannelServiceClient extends $grpc.Client {
  static final _$createMessageChannel = $grpc.ClientMethod<
          $6.CreateMessageChannelRequest, $6.CreateMessageChannelResponse>(
      '/e8.MessageChannelService/CreateMessageChannel',
      ($6.CreateMessageChannelRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $6.CreateMessageChannelResponse.fromBuffer(value));
  static final _$addMemberToMessageChannel = $grpc.ClientMethod<
          $6.AddMemberToMessageChannelRequest,
          $6.AddMemberToMessageChannelResponse>(
      '/e8.MessageChannelService/AddMemberToMessageChannel',
      ($6.AddMemberToMessageChannelRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $6.AddMemberToMessageChannelResponse.fromBuffer(value));
  static final _$getMessageChannelKey = $grpc.ClientMethod<
          $6.GetMessageChannelKeyRequest, $6.GetMessageChannelKeyResponse>(
      '/e8.MessageChannelService/GetMessageChannelKey',
      ($6.GetMessageChannelKeyRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $6.GetMessageChannelKeyResponse.fromBuffer(value));
  static final _$leaveMessageChannel = $grpc.ClientMethod<
          $6.LeaveMessageChannelRequest, $6.LeaveMessageChannelResponse>(
      '/e8.MessageChannelService/LeaveMessageChannel',
      ($6.LeaveMessageChannelRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $6.LeaveMessageChannelResponse.fromBuffer(value));
  static final _$getJoinedInMessageChannels = $grpc.ClientMethod<
          $6.GetJoinedInMessageChannelsRequest,
          $6.GetJoinedInMessageChannelsResponse>(
      '/e8.MessageChannelService/GetJoinedInMessageChannels',
      ($6.GetJoinedInMessageChannelsRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $6.GetJoinedInMessageChannelsResponse.fromBuffer(value));
  static final _$getMessageChannelMembers = $grpc.ClientMethod<
          $6.GetMessageChannelMembersRequest,
          $6.GetMessageChannelMembersResponse>(
      '/e8.MessageChannelService/GetMessageChannelMembers',
      ($6.GetMessageChannelMembersRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $6.GetMessageChannelMembersResponse.fromBuffer(value));

  MessageChannelServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$6.CreateMessageChannelResponse> createMessageChannel(
      $6.CreateMessageChannelRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$createMessageChannel, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$6.AddMemberToMessageChannelResponse>
      addMemberToMessageChannel($6.AddMemberToMessageChannelRequest request,
          {$grpc.CallOptions options}) {
    final call = $createCall(
        _$addMemberToMessageChannel, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$6.GetMessageChannelKeyResponse> getMessageChannelKey(
      $6.GetMessageChannelKeyRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getMessageChannelKey, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$6.LeaveMessageChannelResponse> leaveMessageChannel(
      $6.LeaveMessageChannelRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$leaveMessageChannel, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$6.GetJoinedInMessageChannelsResponse>
      getJoinedInMessageChannels($6.GetJoinedInMessageChannelsRequest request,
          {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getJoinedInMessageChannels, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$6.GetMessageChannelMembersResponse>
      getMessageChannelMembers($6.GetMessageChannelMembersRequest request,
          {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getMessageChannelMembers, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }
}

abstract class MessageChannelServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.MessageChannelService';

  MessageChannelServiceBase() {
    $addMethod($grpc.ServiceMethod<$6.CreateMessageChannelRequest,
            $6.CreateMessageChannelResponse>(
        'CreateMessageChannel',
        createMessageChannel_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $6.CreateMessageChannelRequest.fromBuffer(value),
        ($6.CreateMessageChannelResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$6.AddMemberToMessageChannelRequest,
            $6.AddMemberToMessageChannelResponse>(
        'AddMemberToMessageChannel',
        addMemberToMessageChannel_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $6.AddMemberToMessageChannelRequest.fromBuffer(value),
        ($6.AddMemberToMessageChannelResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$6.GetMessageChannelKeyRequest,
            $6.GetMessageChannelKeyResponse>(
        'GetMessageChannelKey',
        getMessageChannelKey_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $6.GetMessageChannelKeyRequest.fromBuffer(value),
        ($6.GetMessageChannelKeyResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$6.LeaveMessageChannelRequest,
            $6.LeaveMessageChannelResponse>(
        'LeaveMessageChannel',
        leaveMessageChannel_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $6.LeaveMessageChannelRequest.fromBuffer(value),
        ($6.LeaveMessageChannelResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$6.GetJoinedInMessageChannelsRequest,
            $6.GetJoinedInMessageChannelsResponse>(
        'GetJoinedInMessageChannels',
        getJoinedInMessageChannels_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $6.GetJoinedInMessageChannelsRequest.fromBuffer(value),
        ($6.GetJoinedInMessageChannelsResponse value) =>
            value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$6.GetMessageChannelMembersRequest,
            $6.GetMessageChannelMembersResponse>(
        'GetMessageChannelMembers',
        getMessageChannelMembers_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $6.GetMessageChannelMembersRequest.fromBuffer(value),
        ($6.GetMessageChannelMembersResponse value) => value.writeToBuffer()));
  }

  $async.Future<$6.CreateMessageChannelResponse> createMessageChannel_Pre(
      $grpc.ServiceCall call,
      $async.Future<$6.CreateMessageChannelRequest> request) async {
    return createMessageChannel(call, await request);
  }

  $async.Future<$6.AddMemberToMessageChannelResponse>
      addMemberToMessageChannel_Pre($grpc.ServiceCall call,
          $async.Future<$6.AddMemberToMessageChannelRequest> request) async {
    return addMemberToMessageChannel(call, await request);
  }

  $async.Future<$6.GetMessageChannelKeyResponse> getMessageChannelKey_Pre(
      $grpc.ServiceCall call,
      $async.Future<$6.GetMessageChannelKeyRequest> request) async {
    return getMessageChannelKey(call, await request);
  }

  $async.Future<$6.LeaveMessageChannelResponse> leaveMessageChannel_Pre(
      $grpc.ServiceCall call,
      $async.Future<$6.LeaveMessageChannelRequest> request) async {
    return leaveMessageChannel(call, await request);
  }

  $async.Future<$6.GetJoinedInMessageChannelsResponse>
      getJoinedInMessageChannels_Pre($grpc.ServiceCall call,
          $async.Future<$6.GetJoinedInMessageChannelsRequest> request) async {
    return getJoinedInMessageChannels(call, await request);
  }

  $async.Future<$6.GetMessageChannelMembersResponse>
      getMessageChannelMembers_Pre($grpc.ServiceCall call,
          $async.Future<$6.GetMessageChannelMembersRequest> request) async {
    return getMessageChannelMembers(call, await request);
  }

  $async.Future<$6.CreateMessageChannelResponse> createMessageChannel(
      $grpc.ServiceCall call, $6.CreateMessageChannelRequest request);
  $async.Future<$6.AddMemberToMessageChannelResponse> addMemberToMessageChannel(
      $grpc.ServiceCall call, $6.AddMemberToMessageChannelRequest request);
  $async.Future<$6.GetMessageChannelKeyResponse> getMessageChannelKey(
      $grpc.ServiceCall call, $6.GetMessageChannelKeyRequest request);
  $async.Future<$6.LeaveMessageChannelResponse> leaveMessageChannel(
      $grpc.ServiceCall call, $6.LeaveMessageChannelRequest request);
  $async.Future<$6.GetJoinedInMessageChannelsResponse>
      getJoinedInMessageChannels(
          $grpc.ServiceCall call, $6.GetJoinedInMessageChannelsRequest request);
  $async.Future<$6.GetMessageChannelMembersResponse> getMessageChannelMembers(
      $grpc.ServiceCall call, $6.GetMessageChannelMembersRequest request);
}
