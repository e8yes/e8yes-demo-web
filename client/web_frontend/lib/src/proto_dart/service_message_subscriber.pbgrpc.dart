///
//  Generated code. Do not modify.
//  source: service_message_subscriber.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_message_subscriber.pb.dart' as $0;
export 'service_message_subscriber.pb.dart';

class MessageSubscriberServiceClient extends $grpc.Client {
  static final _$subscribeRealTimeMessageQueue = $grpc.ClientMethod<
          $0.SubscribeRealTimeMessageQueueRequest,
          $0.SubscribeRealTimeMessageQueueResponse>(
      '/e8.MessageSubscriberService/SubscribeRealTimeMessageQueue',
      ($0.SubscribeRealTimeMessageQueueRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $0.SubscribeRealTimeMessageQueueResponse.fromBuffer(value));
  static final _$subscriberRealTimeMessageQueueLP = $grpc.ClientMethod<
          $0.SubscribeRealTimeMessageQueueRequest,
          $0.SubscribeRealTimeMessageQueueResponse>(
      '/e8.MessageSubscriberService/SubscriberRealTimeMessageQueueLP',
      ($0.SubscribeRealTimeMessageQueueRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $0.SubscribeRealTimeMessageQueueResponse.fromBuffer(value));

  MessageSubscriberServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseStream<$0.SubscribeRealTimeMessageQueueResponse>
      subscribeRealTimeMessageQueue(
          $0.SubscribeRealTimeMessageQueueRequest request,
          {$grpc.CallOptions options}) {
    final call = $createCall(
        _$subscribeRealTimeMessageQueue, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseStream(call);
  }

  $grpc.ResponseFuture<$0.SubscribeRealTimeMessageQueueResponse>
      subscriberRealTimeMessageQueueLP(
          $0.SubscribeRealTimeMessageQueueRequest request,
          {$grpc.CallOptions options}) {
    final call = $createCall(_$subscriberRealTimeMessageQueueLP,
        $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }
}

abstract class MessageSubscriberServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.MessageSubscriberService';

  MessageSubscriberServiceBase() {
    $addMethod($grpc.ServiceMethod<$0.SubscribeRealTimeMessageQueueRequest,
            $0.SubscribeRealTimeMessageQueueResponse>(
        'SubscribeRealTimeMessageQueue',
        subscribeRealTimeMessageQueue_Pre,
        false,
        true,
        ($core.List<$core.int> value) =>
            $0.SubscribeRealTimeMessageQueueRequest.fromBuffer(value),
        ($0.SubscribeRealTimeMessageQueueResponse value) =>
            value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$0.SubscribeRealTimeMessageQueueRequest,
            $0.SubscribeRealTimeMessageQueueResponse>(
        'SubscriberRealTimeMessageQueueLP',
        subscriberRealTimeMessageQueueLP_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $0.SubscribeRealTimeMessageQueueRequest.fromBuffer(value),
        ($0.SubscribeRealTimeMessageQueueResponse value) =>
            value.writeToBuffer()));
  }

  $async.Stream<$0.SubscribeRealTimeMessageQueueResponse>
      subscribeRealTimeMessageQueue_Pre(
          $grpc.ServiceCall call,
          $async.Future<$0.SubscribeRealTimeMessageQueueRequest>
              request) async* {
    yield* subscribeRealTimeMessageQueue(call, await request);
  }

  $async.Future<$0.SubscribeRealTimeMessageQueueResponse>
      subscriberRealTimeMessageQueueLP_Pre(
          $grpc.ServiceCall call,
          $async.Future<$0.SubscribeRealTimeMessageQueueRequest>
              request) async {
    return subscriberRealTimeMessageQueueLP(call, await request);
  }

  $async.Stream<$0.SubscribeRealTimeMessageQueueResponse>
      subscribeRealTimeMessageQueue($grpc.ServiceCall call,
          $0.SubscribeRealTimeMessageQueueRequest request);
  $async.Future<$0.SubscribeRealTimeMessageQueueResponse>
      subscriberRealTimeMessageQueueLP($grpc.ServiceCall call,
          $0.SubscribeRealTimeMessageQueueRequest request);
}
