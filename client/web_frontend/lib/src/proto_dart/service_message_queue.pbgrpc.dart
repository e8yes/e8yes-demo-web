///
//  Generated code. Do not modify.
//  source: service_message_queue.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_message_queue.pb.dart' as $4;
export 'service_message_queue.pb.dart';

class MessageQueueServiceClient extends $grpc.Client {
  static final _$enqueueMessage =
      $grpc.ClientMethod<$4.EnqueueMessageRequest, $4.EnqueueMessageResponse>(
          '/e8.MessageQueueService/EnqueueMessage',
          ($4.EnqueueMessageRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $4.EnqueueMessageResponse.fromBuffer(value));
  static final _$dequeueMessage =
      $grpc.ClientMethod<$4.DequeueMessageRequest, $4.DequeueMessageResponse>(
          '/e8.MessageQueueService/DequeueMessage',
          ($4.DequeueMessageRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $4.DequeueMessageResponse.fromBuffer(value));

  MessageQueueServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$4.EnqueueMessageResponse> enqueueMessage(
      $4.EnqueueMessageRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$enqueueMessage, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseStream<$4.DequeueMessageResponse> dequeueMessage(
      $async.Stream<$4.DequeueMessageRequest> request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$dequeueMessage, request, options: options);
    return $grpc.ResponseStream(call);
  }
}

abstract class MessageQueueServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.MessageQueueService';

  MessageQueueServiceBase() {
    $addMethod($grpc.ServiceMethod<$4.EnqueueMessageRequest,
            $4.EnqueueMessageResponse>(
        'EnqueueMessage',
        enqueueMessage_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $4.EnqueueMessageRequest.fromBuffer(value),
        ($4.EnqueueMessageResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$4.DequeueMessageRequest,
            $4.DequeueMessageResponse>(
        'DequeueMessage',
        dequeueMessage,
        true,
        true,
        ($core.List<$core.int> value) =>
            $4.DequeueMessageRequest.fromBuffer(value),
        ($4.DequeueMessageResponse value) => value.writeToBuffer()));
  }

  $async.Future<$4.EnqueueMessageResponse> enqueueMessage_Pre(
      $grpc.ServiceCall call,
      $async.Future<$4.EnqueueMessageRequest> request) async {
    return enqueueMessage(call, await request);
  }

  $async.Future<$4.EnqueueMessageResponse> enqueueMessage(
      $grpc.ServiceCall call, $4.EnqueueMessageRequest request);
  $async.Stream<$4.DequeueMessageResponse> dequeueMessage(
      $grpc.ServiceCall call, $async.Stream<$4.DequeueMessageRequest> request);
}
