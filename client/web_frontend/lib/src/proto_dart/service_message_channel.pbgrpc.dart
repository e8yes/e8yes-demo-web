///
//  Generated code. Do not modify.
//  source: service_message_channel.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_message_channel.pb.dart' as $7;
export 'service_message_channel.pb.dart';

class MessageChannelServiceClient extends $grpc.Client {
  static final _$createMessageChannel = $grpc.ClientMethod<
          $7.CreateMessageChannelRequest, $7.CreateMessageChannelResponse>(
      '/e8.MessageChannelService/CreateMessageChannel',
      ($7.CreateMessageChannelRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $7.CreateMessageChannelResponse.fromBuffer(value));
  static final _$addMemberToMessageChannel = $grpc.ClientMethod<
          $7.AddMemberToMessageChannelRequest,
          $7.AddMemberToMessageChannelResponse>(
      '/e8.MessageChannelService/AddMemberToMessageChannel',
      ($7.AddMemberToMessageChannelRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $7.AddMemberToMessageChannelResponse.fromBuffer(value));
  static final _$getMessageChannelKey = $grpc.ClientMethod<
          $7.GetMessageChannelKeyRequest, $7.GetMessageChannelKeyResponse>(
      '/e8.MessageChannelService/GetMessageChannelKey',
      ($7.GetMessageChannelKeyRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $7.GetMessageChannelKeyResponse.fromBuffer(value));
  static final _$leaveMessageChannel = $grpc.ClientMethod<
          $7.LeaveMessageChannelRequest, $7.LeaveMessageChannelResponse>(
      '/e8.MessageChannelService/LeaveMessageChannel',
      ($7.LeaveMessageChannelRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $7.LeaveMessageChannelResponse.fromBuffer(value));
  static final _$getJoinedInMessageChannels = $grpc.ClientMethod<
          $7.GetJoinedInMessageChannelsRequest,
          $7.GetJoinedInMessageChannelsResponse>(
      '/e8.MessageChannelService/GetJoinedInMessageChannels',
      ($7.GetJoinedInMessageChannelsRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $7.GetJoinedInMessageChannelsResponse.fromBuffer(value));
  static final _$getMessageChannelMembers = $grpc.ClientMethod<
          $7.GetMessageChannelMembersRequest,
          $7.GetMessageChannelMembersResponse>(
      '/e8.MessageChannelService/GetMessageChannelMembers',
      ($7.GetMessageChannelMembersRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $7.GetMessageChannelMembersResponse.fromBuffer(value));

  MessageChannelServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$7.CreateMessageChannelResponse> createMessageChannel(
      $7.CreateMessageChannelRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$createMessageChannel, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$7.AddMemberToMessageChannelResponse>
      addMemberToMessageChannel($7.AddMemberToMessageChannelRequest request,
          {$grpc.CallOptions options}) {
    final call = $createCall(
        _$addMemberToMessageChannel, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$7.GetMessageChannelKeyResponse> getMessageChannelKey(
      $7.GetMessageChannelKeyRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getMessageChannelKey, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$7.LeaveMessageChannelResponse> leaveMessageChannel(
      $7.LeaveMessageChannelRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$leaveMessageChannel, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$7.GetJoinedInMessageChannelsResponse>
      getJoinedInMessageChannels($7.GetJoinedInMessageChannelsRequest request,
          {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getJoinedInMessageChannels, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$7.GetMessageChannelMembersResponse>
      getMessageChannelMembers($7.GetMessageChannelMembersRequest request,
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
    $addMethod($grpc.ServiceMethod<$7.CreateMessageChannelRequest,
            $7.CreateMessageChannelResponse>(
        'CreateMessageChannel',
        createMessageChannel_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $7.CreateMessageChannelRequest.fromBuffer(value),
        ($7.CreateMessageChannelResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$7.AddMemberToMessageChannelRequest,
            $7.AddMemberToMessageChannelResponse>(
        'AddMemberToMessageChannel',
        addMemberToMessageChannel_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $7.AddMemberToMessageChannelRequest.fromBuffer(value),
        ($7.AddMemberToMessageChannelResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$7.GetMessageChannelKeyRequest,
            $7.GetMessageChannelKeyResponse>(
        'GetMessageChannelKey',
        getMessageChannelKey_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $7.GetMessageChannelKeyRequest.fromBuffer(value),
        ($7.GetMessageChannelKeyResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$7.LeaveMessageChannelRequest,
            $7.LeaveMessageChannelResponse>(
        'LeaveMessageChannel',
        leaveMessageChannel_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $7.LeaveMessageChannelRequest.fromBuffer(value),
        ($7.LeaveMessageChannelResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$7.GetJoinedInMessageChannelsRequest,
            $7.GetJoinedInMessageChannelsResponse>(
        'GetJoinedInMessageChannels',
        getJoinedInMessageChannels_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $7.GetJoinedInMessageChannelsRequest.fromBuffer(value),
        ($7.GetJoinedInMessageChannelsResponse value) =>
            value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$7.GetMessageChannelMembersRequest,
            $7.GetMessageChannelMembersResponse>(
        'GetMessageChannelMembers',
        getMessageChannelMembers_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $7.GetMessageChannelMembersRequest.fromBuffer(value),
        ($7.GetMessageChannelMembersResponse value) => value.writeToBuffer()));
  }

  $async.Future<$7.CreateMessageChannelResponse> createMessageChannel_Pre(
      $grpc.ServiceCall call,
      $async.Future<$7.CreateMessageChannelRequest> request) async {
    return createMessageChannel(call, await request);
  }

  $async.Future<$7.AddMemberToMessageChannelResponse>
      addMemberToMessageChannel_Pre($grpc.ServiceCall call,
          $async.Future<$7.AddMemberToMessageChannelRequest> request) async {
    return addMemberToMessageChannel(call, await request);
  }

  $async.Future<$7.GetMessageChannelKeyResponse> getMessageChannelKey_Pre(
      $grpc.ServiceCall call,
      $async.Future<$7.GetMessageChannelKeyRequest> request) async {
    return getMessageChannelKey(call, await request);
  }

  $async.Future<$7.LeaveMessageChannelResponse> leaveMessageChannel_Pre(
      $grpc.ServiceCall call,
      $async.Future<$7.LeaveMessageChannelRequest> request) async {
    return leaveMessageChannel(call, await request);
  }

  $async.Future<$7.GetJoinedInMessageChannelsResponse>
      getJoinedInMessageChannels_Pre($grpc.ServiceCall call,
          $async.Future<$7.GetJoinedInMessageChannelsRequest> request) async {
    return getJoinedInMessageChannels(call, await request);
  }

  $async.Future<$7.GetMessageChannelMembersResponse>
      getMessageChannelMembers_Pre($grpc.ServiceCall call,
          $async.Future<$7.GetMessageChannelMembersRequest> request) async {
    return getMessageChannelMembers(call, await request);
  }

  $async.Future<$7.CreateMessageChannelResponse> createMessageChannel(
      $grpc.ServiceCall call, $7.CreateMessageChannelRequest request);
  $async.Future<$7.AddMemberToMessageChannelResponse> addMemberToMessageChannel(
      $grpc.ServiceCall call, $7.AddMemberToMessageChannelRequest request);
  $async.Future<$7.GetMessageChannelKeyResponse> getMessageChannelKey(
      $grpc.ServiceCall call, $7.GetMessageChannelKeyRequest request);
  $async.Future<$7.LeaveMessageChannelResponse> leaveMessageChannel(
      $grpc.ServiceCall call, $7.LeaveMessageChannelRequest request);
  $async.Future<$7.GetJoinedInMessageChannelsResponse>
      getJoinedInMessageChannels(
          $grpc.ServiceCall call, $7.GetJoinedInMessageChannelsRequest request);
  $async.Future<$7.GetMessageChannelMembersResponse> getMessageChannelMembers(
      $grpc.ServiceCall call, $7.GetMessageChannelMembersRequest request);
}
