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
  static final _$reviseNodeState =
      $grpc.ClientMethod<$1.ReviseNodeStateRequest, $1.ReviseNodeStateResponse>(
          '/e8.NodeStateService/ReviseNodeState',
          ($1.ReviseNodeStateRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $1.ReviseNodeStateResponse.fromBuffer(value));
  static final _$getCurrentRevisionEpoch = $grpc.ClientMethod<
          $1.GetCurrentRevisionEpochRequest,
          $1.GetCurrentRevisionEpochResponse>(
      '/e8.NodeStateService/GetCurrentRevisionEpoch',
      ($1.GetCurrentRevisionEpochRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $1.GetCurrentRevisionEpochResponse.fromBuffer(value));
  static final _$addPeers =
      $grpc.ClientMethod<$1.AddPeersRequest, $1.AddPeersResponse>(
          '/e8.NodeStateService/AddPeers',
          ($1.AddPeersRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $1.AddPeersResponse.fromBuffer(value));
  static final _$deletePeers =
      $grpc.ClientMethod<$1.DeletePeersRequest, $1.DeletePeersResponse>(
          '/e8.NodeStateService/DeletePeers',
          ($1.DeletePeersRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $1.DeletePeersResponse.fromBuffer(value));

  NodeStateServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$1.ReviseNodeStateResponse> reviseNodeState(
      $1.ReviseNodeStateRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$reviseNodeState, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$1.GetCurrentRevisionEpochResponse>
      getCurrentRevisionEpoch($1.GetCurrentRevisionEpochRequest request,
          {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getCurrentRevisionEpoch, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$1.AddPeersResponse> addPeers($1.AddPeersRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$addPeers, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$1.DeletePeersResponse> deletePeers(
      $1.DeletePeersRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$deletePeers, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }
}

abstract class NodeStateServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.NodeStateService';

  NodeStateServiceBase() {
    $addMethod($grpc.ServiceMethod<$1.ReviseNodeStateRequest,
            $1.ReviseNodeStateResponse>(
        'ReviseNodeState',
        reviseNodeState_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $1.ReviseNodeStateRequest.fromBuffer(value),
        ($1.ReviseNodeStateResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$1.GetCurrentRevisionEpochRequest,
            $1.GetCurrentRevisionEpochResponse>(
        'GetCurrentRevisionEpoch',
        getCurrentRevisionEpoch_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $1.GetCurrentRevisionEpochRequest.fromBuffer(value),
        ($1.GetCurrentRevisionEpochResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$1.AddPeersRequest, $1.AddPeersResponse>(
        'AddPeers',
        addPeers_Pre,
        false,
        false,
        ($core.List<$core.int> value) => $1.AddPeersRequest.fromBuffer(value),
        ($1.AddPeersResponse value) => value.writeToBuffer()));
    $addMethod(
        $grpc.ServiceMethod<$1.DeletePeersRequest, $1.DeletePeersResponse>(
            'DeletePeers',
            deletePeers_Pre,
            false,
            false,
            ($core.List<$core.int> value) =>
                $1.DeletePeersRequest.fromBuffer(value),
            ($1.DeletePeersResponse value) => value.writeToBuffer()));
  }

  $async.Future<$1.ReviseNodeStateResponse> reviseNodeState_Pre(
      $grpc.ServiceCall call,
      $async.Future<$1.ReviseNodeStateRequest> request) async {
    return reviseNodeState(call, await request);
  }

  $async.Future<$1.GetCurrentRevisionEpochResponse> getCurrentRevisionEpoch_Pre(
      $grpc.ServiceCall call,
      $async.Future<$1.GetCurrentRevisionEpochRequest> request) async {
    return getCurrentRevisionEpoch(call, await request);
  }

  $async.Future<$1.AddPeersResponse> addPeers_Pre(
      $grpc.ServiceCall call, $async.Future<$1.AddPeersRequest> request) async {
    return addPeers(call, await request);
  }

  $async.Future<$1.DeletePeersResponse> deletePeers_Pre($grpc.ServiceCall call,
      $async.Future<$1.DeletePeersRequest> request) async {
    return deletePeers(call, await request);
  }

  $async.Future<$1.ReviseNodeStateResponse> reviseNodeState(
      $grpc.ServiceCall call, $1.ReviseNodeStateRequest request);
  $async.Future<$1.GetCurrentRevisionEpochResponse> getCurrentRevisionEpoch(
      $grpc.ServiceCall call, $1.GetCurrentRevisionEpochRequest request);
  $async.Future<$1.AddPeersResponse> addPeers(
      $grpc.ServiceCall call, $1.AddPeersRequest request);
  $async.Future<$1.DeletePeersResponse> deletePeers(
      $grpc.ServiceCall call, $1.DeletePeersRequest request);
}
