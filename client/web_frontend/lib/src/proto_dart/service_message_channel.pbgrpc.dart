///
//  Generated code. Do not modify.
//  source: service_message_channel.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_message_channel.pb.dart' as $4;
export 'service_message_channel.pb.dart';

class MessageChannelServiceClient extends $grpc.Client {
  static final _$createMessageChannel = $grpc.ClientMethod<
          $4.CreateMessageChannelRequest, $4.CreateMessageChannelResponse>(
      '/e8.MessageChannelService/CreateMessageChannel',
      ($4.CreateMessageChannelRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $4.CreateMessageChannelResponse.fromBuffer(value));
  static final _$addMemberToMessageChannel = $grpc.ClientMethod<
          $4.AddMemberToMessageChannelRequest,
          $4.AddMemberToMessageChannelResponse>(
      '/e8.MessageChannelService/AddMemberToMessageChannel',
      ($4.AddMemberToMessageChannelRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $4.AddMemberToMessageChannelResponse.fromBuffer(value));
  static final _$getMessageChannelKey = $grpc.ClientMethod<
          $4.GetMessageChannelKeyRequest, $4.GetMessageChannelKeyResponse>(
      '/e8.MessageChannelService/GetMessageChannelKey',
      ($4.GetMessageChannelKeyRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $4.GetMessageChannelKeyResponse.fromBuffer(value));
  static final _$leaveMessageChannel = $grpc.ClientMethod<
          $4.LeaveMessageChannelRequest, $4.LeaveMessageChannelResponse>(
      '/e8.MessageChannelService/LeaveMessageChannel',
      ($4.LeaveMessageChannelRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $4.LeaveMessageChannelResponse.fromBuffer(value));
  static final _$getJoinedInMessageChannels = $grpc.ClientMethod<
          $4.GetJoinedInMessageChannelsRequest,
          $4.GetJoinedInMessageChannelsResponse>(
      '/e8.MessageChannelService/GetJoinedInMessageChannels',
      ($4.GetJoinedInMessageChannelsRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $4.GetJoinedInMessageChannelsResponse.fromBuffer(value));
  static final _$getMessageChannelMembers = $grpc.ClientMethod<
          $4.GetMessageChannelMembersRequest,
          $4.GetMessageChannelMembersResponse>(
      '/e8.MessageChannelService/GetMessageChannelMembers',
      ($4.GetMessageChannelMembersRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $4.GetMessageChannelMembersResponse.fromBuffer(value));

  MessageChannelServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$4.CreateMessageChannelResponse> createMessageChannel(
      $4.CreateMessageChannelRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$createMessageChannel, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$4.AddMemberToMessageChannelResponse>
      addMemberToMessageChannel($4.AddMemberToMessageChannelRequest request,
          {$grpc.CallOptions options}) {
    final call = $createCall(
        _$addMemberToMessageChannel, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$4.GetMessageChannelKeyResponse> getMessageChannelKey(
      $4.GetMessageChannelKeyRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getMessageChannelKey, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$4.LeaveMessageChannelResponse> leaveMessageChannel(
      $4.LeaveMessageChannelRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$leaveMessageChannel, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$4.GetJoinedInMessageChannelsResponse>
      getJoinedInMessageChannels($4.GetJoinedInMessageChannelsRequest request,
          {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getJoinedInMessageChannels, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$4.GetMessageChannelMembersResponse>
      getMessageChannelMembers($4.GetMessageChannelMembersRequest request,
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
    $addMethod($grpc.ServiceMethod<$4.CreateMessageChannelRequest,
            $4.CreateMessageChannelResponse>(
        'CreateMessageChannel',
        createMessageChannel_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $4.CreateMessageChannelRequest.fromBuffer(value),
        ($4.CreateMessageChannelResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$4.AddMemberToMessageChannelRequest,
            $4.AddMemberToMessageChannelResponse>(
        'AddMemberToMessageChannel',
        addMemberToMessageChannel_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $4.AddMemberToMessageChannelRequest.fromBuffer(value),
        ($4.AddMemberToMessageChannelResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$4.GetMessageChannelKeyRequest,
            $4.GetMessageChannelKeyResponse>(
        'GetMessageChannelKey',
        getMessageChannelKey_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $4.GetMessageChannelKeyRequest.fromBuffer(value),
        ($4.GetMessageChannelKeyResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$4.LeaveMessageChannelRequest,
            $4.LeaveMessageChannelResponse>(
        'LeaveMessageChannel',
        leaveMessageChannel_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $4.LeaveMessageChannelRequest.fromBuffer(value),
        ($4.LeaveMessageChannelResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$4.GetJoinedInMessageChannelsRequest,
            $4.GetJoinedInMessageChannelsResponse>(
        'GetJoinedInMessageChannels',
        getJoinedInMessageChannels_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $4.GetJoinedInMessageChannelsRequest.fromBuffer(value),
        ($4.GetJoinedInMessageChannelsResponse value) =>
            value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$4.GetMessageChannelMembersRequest,
            $4.GetMessageChannelMembersResponse>(
        'GetMessageChannelMembers',
        getMessageChannelMembers_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $4.GetMessageChannelMembersRequest.fromBuffer(value),
        ($4.GetMessageChannelMembersResponse value) => value.writeToBuffer()));
  }

  $async.Future<$4.CreateMessageChannelResponse> createMessageChannel_Pre(
      $grpc.ServiceCall call,
      $async.Future<$4.CreateMessageChannelRequest> request) async {
    return createMessageChannel(call, await request);
  }

  $async.Future<$4.AddMemberToMessageChannelResponse>
      addMemberToMessageChannel_Pre($grpc.ServiceCall call,
          $async.Future<$4.AddMemberToMessageChannelRequest> request) async {
    return addMemberToMessageChannel(call, await request);
  }

  $async.Future<$4.GetMessageChannelKeyResponse> getMessageChannelKey_Pre(
      $grpc.ServiceCall call,
      $async.Future<$4.GetMessageChannelKeyRequest> request) async {
    return getMessageChannelKey(call, await request);
  }

  $async.Future<$4.LeaveMessageChannelResponse> leaveMessageChannel_Pre(
      $grpc.ServiceCall call,
      $async.Future<$4.LeaveMessageChannelRequest> request) async {
    return leaveMessageChannel(call, await request);
  }

  $async.Future<$4.GetJoinedInMessageChannelsResponse>
      getJoinedInMessageChannels_Pre($grpc.ServiceCall call,
          $async.Future<$4.GetJoinedInMessageChannelsRequest> request) async {
    return getJoinedInMessageChannels(call, await request);
  }

  $async.Future<$4.GetMessageChannelMembersResponse>
      getMessageChannelMembers_Pre($grpc.ServiceCall call,
          $async.Future<$4.GetMessageChannelMembersRequest> request) async {
    return getMessageChannelMembers(call, await request);
  }

  $async.Future<$4.CreateMessageChannelResponse> createMessageChannel(
      $grpc.ServiceCall call, $4.CreateMessageChannelRequest request);
  $async.Future<$4.AddMemberToMessageChannelResponse> addMemberToMessageChannel(
      $grpc.ServiceCall call, $4.AddMemberToMessageChannelRequest request);
  $async.Future<$4.GetMessageChannelKeyResponse> getMessageChannelKey(
      $grpc.ServiceCall call, $4.GetMessageChannelKeyRequest request);
  $async.Future<$4.LeaveMessageChannelResponse> leaveMessageChannel(
      $grpc.ServiceCall call, $4.LeaveMessageChannelRequest request);
  $async.Future<$4.GetJoinedInMessageChannelsResponse>
      getJoinedInMessageChannels(
          $grpc.ServiceCall call, $4.GetJoinedInMessageChannelsRequest request);
  $async.Future<$4.GetMessageChannelMembersResponse> getMessageChannelMembers(
      $grpc.ServiceCall call, $4.GetMessageChannelMembersRequest request);
}
