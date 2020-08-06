///
//  Generated code. Do not modify.
//  source: service_message_queue.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_message_queue.pb.dart' as $3;
export 'service_message_queue.pb.dart';

class MessageQueueServiceClient extends $grpc.Client {
  static final _$enqueueMessage =
      $grpc.ClientMethod<$3.EnqueueMessageRequest, $3.EnqueueMessageResponse>(
          '/e8.MessageQueueService/EnqueueMessage',
          ($3.EnqueueMessageRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $3.EnqueueMessageResponse.fromBuffer(value));
  static final _$dequeueMessage =
      $grpc.ClientMethod<$3.DequeueMessageRequest, $3.DequeueMessageResponse>(
          '/e8.MessageQueueService/DequeueMessage',
          ($3.DequeueMessageRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $3.DequeueMessageResponse.fromBuffer(value));

  MessageQueueServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$3.EnqueueMessageResponse> enqueueMessage(
      $3.EnqueueMessageRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$enqueueMessage, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseStream<$3.DequeueMessageResponse> dequeueMessage(
      $3.DequeueMessageRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$dequeueMessage, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseStream(call);
  }
}

abstract class MessageQueueServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.MessageQueueService';

  MessageQueueServiceBase() {
    $addMethod($grpc.ServiceMethod<$3.EnqueueMessageRequest,
            $3.EnqueueMessageResponse>(
        'EnqueueMessage',
        enqueueMessage_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $3.EnqueueMessageRequest.fromBuffer(value),
        ($3.EnqueueMessageResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$3.DequeueMessageRequest,
            $3.DequeueMessageResponse>(
        'DequeueMessage',
        dequeueMessage_Pre,
        false,
        true,
        ($core.List<$core.int> value) =>
            $3.DequeueMessageRequest.fromBuffer(value),
        ($3.DequeueMessageResponse value) => value.writeToBuffer()));
  }

  $async.Future<$3.EnqueueMessageResponse> enqueueMessage_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.EnqueueMessageRequest> request) async {
    return enqueueMessage(call, await request);
  }

  $async.Stream<$3.DequeueMessageResponse> dequeueMessage_Pre(
      $grpc.ServiceCall call,
      $async.Future<$3.DequeueMessageRequest> request) async* {
    yield* dequeueMessage(call, await request);
  }

  $async.Future<$3.EnqueueMessageResponse> enqueueMessage(
      $grpc.ServiceCall call, $3.EnqueueMessageRequest request);
  $async.Stream<$3.DequeueMessageResponse> dequeueMessage(
      $grpc.ServiceCall call, $3.DequeueMessageRequest request);
}
