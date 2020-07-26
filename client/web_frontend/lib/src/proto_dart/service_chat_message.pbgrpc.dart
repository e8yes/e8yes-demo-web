///
//  Generated code. Do not modify.
//  source: service_chat_message.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_chat_message.pb.dart' as $2;
export 'service_chat_message.pb.dart';

class ChatMessageServiceClient extends $grpc.Client {
  static final _$sendChatMessage =
      $grpc.ClientMethod<$2.SendChatMessageRequest, $2.SendChatMessageResponse>(
          '/e8.ChatMessageService/SendChatMessage',
          ($2.SendChatMessageRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $2.SendChatMessageResponse.fromBuffer(value));
  static final _$getChatMessage =
      $grpc.ClientMethod<$2.GetChatMessageRequest, $2.GetChatMessageResponse>(
          '/e8.ChatMessageService/GetChatMessage',
          ($2.GetChatMessageRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $2.GetChatMessageResponse.fromBuffer(value));

  ChatMessageServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$2.SendChatMessageResponse> sendChatMessage(
      $2.SendChatMessageRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$sendChatMessage, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$2.GetChatMessageResponse> getChatMessage(
      $2.GetChatMessageRequest request,
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
    $addMethod($grpc.ServiceMethod<$2.SendChatMessageRequest,
            $2.SendChatMessageResponse>(
        'SendChatMessage',
        sendChatMessage_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $2.SendChatMessageRequest.fromBuffer(value),
        ($2.SendChatMessageResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$2.GetChatMessageRequest,
            $2.GetChatMessageResponse>(
        'GetChatMessage',
        getChatMessage_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $2.GetChatMessageRequest.fromBuffer(value),
        ($2.GetChatMessageResponse value) => value.writeToBuffer()));
  }

  $async.Future<$2.SendChatMessageResponse> sendChatMessage_Pre(
      $grpc.ServiceCall call,
      $async.Future<$2.SendChatMessageRequest> request) async {
    return sendChatMessage(call, await request);
  }

  $async.Future<$2.GetChatMessageResponse> getChatMessage_Pre(
      $grpc.ServiceCall call,
      $async.Future<$2.GetChatMessageRequest> request) async {
    return getChatMessage(call, await request);
  }

  $async.Future<$2.SendChatMessageResponse> sendChatMessage(
      $grpc.ServiceCall call, $2.SendChatMessageRequest request);
  $async.Future<$2.GetChatMessageResponse> getChatMessage(
      $grpc.ServiceCall call, $2.GetChatMessageRequest request);
}
