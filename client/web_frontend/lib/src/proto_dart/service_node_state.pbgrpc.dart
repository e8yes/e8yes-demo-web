///
//  Generated code. Do not modify.
//  source: service_node_state.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_node_state.pb.dart' as $1;
export 'service_node_state.pb.dart';

class NodeStateServiceClient extends $grpc.Client {
  static final _$updateNodeState =
      $grpc.ClientMethod<$1.UpdateNodeStateRequest, $1.UpdateNodeStateResponse>(
          '/e8.NodeStateService/UpdateNodeState',
          ($1.UpdateNodeStateRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $1.UpdateNodeStateResponse.fromBuffer(value));

  NodeStateServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$1.UpdateNodeStateResponse> updateNodeState(
      $1.UpdateNodeStateRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$updateNodeState, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }
}

abstract class NodeStateServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.NodeStateService';

  NodeStateServiceBase() {
    $addMethod($grpc.ServiceMethod<$1.UpdateNodeStateRequest,
            $1.UpdateNodeStateResponse>(
        'UpdateNodeState',
        updateNodeState_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $1.UpdateNodeStateRequest.fromBuffer(value),
        ($1.UpdateNodeStateResponse value) => value.writeToBuffer()));
  }

  $async.Future<$1.UpdateNodeStateResponse> updateNodeState_Pre(
      $grpc.ServiceCall call,
      $async.Future<$1.UpdateNodeStateRequest> request) async {
    return updateNodeState(call, await request);
  }

  $async.Future<$1.UpdateNodeStateResponse> updateNodeState(
      $grpc.ServiceCall call, $1.UpdateNodeStateRequest request);
}
