///
//  Generated code. Do not modify.
//  source: service_chat_message.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_chat_message.pb.dart' as $1;
export 'service_chat_message.pb.dart';

class ChatMessageServiceClient extends $grpc.Client {
  static final _$sendChatMessage =
      $grpc.ClientMethod<$1.SendChatMessageRequest, $1.SendChatMessageResponse>(
          '/e8.ChatMessageService/SendChatMessage',
          ($1.SendChatMessageRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $1.SendChatMessageResponse.fromBuffer(value));
  static final _$getChatMessage =
      $grpc.ClientMethod<$1.GetChatMessageRequest, $1.GetChatMessageResponse>(
          '/e8.ChatMessageService/GetChatMessage',
          ($1.GetChatMessageRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $1.GetChatMessageResponse.fromBuffer(value));

  ChatMessageServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$1.SendChatMessageResponse> sendChatMessage(
      $1.SendChatMessageRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$sendChatMessage, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$1.GetChatMessageResponse> getChatMessage(
      $1.GetChatMessageRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getChatMessage, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }
}

abstract class ChatMessageServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.ChatMessageService';

  ChatMessageServiceBase() {
    $addMethod($grpc.ServiceMethod<$1.SendChatMessageRequest,
            $1.SendChatMessageResponse>(
        'SendChatMessage',
        sendChatMessage_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $1.SendChatMessageRequest.fromBuffer(value),
        ($1.SendChatMessageResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$1.GetChatMessageRequest,
            $1.GetChatMessageResponse>(
        'GetChatMessage',
        getChatMessage_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $1.GetChatMessageRequest.fromBuffer(value),
        ($1.GetChatMessageResponse value) => value.writeToBuffer()));
  }

  $async.Future<$1.SendChatMessageResponse> sendChatMessage_Pre(
      $grpc.ServiceCall call,
      $async.Future<$1.SendChatMessageRequest> request) async {
    return sendChatMessage(call, await request);
  }

  $async.Future<$1.GetChatMessageResponse> getChatMessage_Pre(
      $grpc.ServiceCall call,
      $async.Future<$1.GetChatMessageRequest> request) async {
    return getChatMessage(call, await request);
  }

  $async.Future<$1.SendChatMessageResponse> sendChatMessage(
      $grpc.ServiceCall call, $1.SendChatMessageRequest request);
  $async.Future<$1.GetChatMessageResponse> getChatMessage(
      $grpc.ServiceCall call, $1.GetChatMessageRequest request);
}
