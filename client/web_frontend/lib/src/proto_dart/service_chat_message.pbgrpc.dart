///
//  Generated code. Do not modify.
//  source: service_chat_message.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_chat_message.pb.dart' as $3;
export 'service_chat_message.pb.dart';

class ChatMessageServiceClient extends $grpc.Client {
  static final _$createChatMessageThread = $grpc.ClientMethod<
          $3.CreateChatMessageThreadRequest,
          $3.CreateChatMessageThreadResponse>(
      '/e8.ChatMessageService/CreateChatMessageThread',
      ($3.CreateChatMessageThreadRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $3.CreateChatMessageThreadResponse.fromBuffer(value));
  static final _$sendChatMessage =
      $grpc.ClientMethod<$3.SendChatMessageRequest, $3.SendChatMessageResponse>(
          '/e8.ChatMessageService/SendChatMessage',
          ($3.SendChatMessageRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $3.SendChatMessageResponse.fromBuffer(value));
  static final _$getChatMessage =
      $grpc.ClientMethod<$3.GetChatMessageRequest, $3.GetChatMessageResponse>(
          '/e8.ChatMessageService/GetChatMessage',
          ($3.GetChatMessageRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $3.GetChatMessageResponse.fromBuffer(value));

  ChatMessageServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$3.CreateChatMessageThreadResponse>
      createChatMessageThread($3.CreateChatMessageThreadRequest request,
          {$grpc.CallOptions options}) {
    final call = $createCall(
        _$createChatMessageThread, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$3.SendChatMessageResponse> sendChatMessage(
      $3.SendChatMessageRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$sendChatMessage, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$3.GetChatMessageResponse> getChatMessage(
      $3.GetChatMessageRequest request,
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
    $addMethod($grpc.ServiceMethod<$3.CreateChatMessageThreadRequest,
            $3.CreateChatMessageThreadResponse>(
        'CreateChatMessageThread',
        createChatMessageThread_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $3.CreateChatMessageThreadRequest.fromBuffer(value),
        ($3.CreateChatMessageThreadResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$3.SendChatMessageRequest,
            $3.SendChatMessageResponse>(
        'SendChatMessage',
        sendChatMessage_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $3.SendChatMessageRequest.fromBuffer(value),
        ($3.SendChatMessageResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$3.GetChatMessageRequest,
            $3.GetChatMessageResponse>(
        'GetChatMessage',
        getChatMessage_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $3.GetChatMessageRequest.fromBuffer(value),
        ($3.GetChatMessageResponse value) => value.writeToBuffer()));
  }

  $async.Future<$3.CreateChatMessageThreadResponse> createChatMessageThread_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.CreateChatMessageThreadRequest> request) async {
    return createChatMessageThread(call, await request);
  }

  $async.Future<$3.SendChatMessageResponse> sendChatMessage_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.SendChatMessageRequest> request) async {
    return sendChatMessage(call, await request);
  }

  $async.Future<$3.GetChatMessageResponse> getChatMessage_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.GetChatMessageRequest> request) async {
    return getChatMessage(call, await request);
  }

  $async.Future<$3.CreateChatMessageThreadResponse> createChatMessageThread(
      $grpc.ServiceCall call, $3.CreateChatMessageThreadRequest request);
  $async.Future<$3.SendChatMessageResponse> sendChatMessage(
      $grpc.ServiceCall call, $3.SendChatMessageRequest request);
  $async.Future<$3.GetChatMessageResponse> getChatMessage(
      $grpc.ServiceCall call, $3.GetChatMessageRequest request);
}
