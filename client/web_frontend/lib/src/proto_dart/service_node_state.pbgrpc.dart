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
  static final _$getLatestRevisionEpoch = $grpc.ClientMethod<
          $1.GetLatestRevisionEpochRequest, $1.GetLatestRevisionEpochResponse>(
      '/e8.NodeStateService/GetLatestRevisionEpoch',
      ($1.GetLatestRevisionEpochRequest value) => value.writeToBuffer(),
      ($core.List<$core.int> value) =>
          $1.GetLatestRevisionEpochResponse.fromBuffer(value));
  static final _$addPeer =
      $grpc.ClientMethod<$1.AddPeerRequest, $1.AddPeerResponse>(
          '/e8.NodeStateService/AddPeer',
          ($1.AddPeerRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $1.AddPeerResponse.fromBuffer(value));
  static final _$deletePeer =
      $grpc.ClientMethod<$1.DeletePeerRequest, $1.DeletePeerResponse>(
          '/e8.NodeStateService/DeletePeer',
          ($1.DeletePeerRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $1.DeletePeerResponse.fromBuffer(value));

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

  $grpc.ResponseFuture<$1.GetLatestRevisionEpochResponse>
      getLatestRevisionEpoch($1.GetLatestRevisionEpochRequest request,
          {$grpc.CallOptions options}) {
    final call = $createCall(
        _$getLatestRevisionEpoch, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$1.AddPeerResponse> addPeer($1.AddPeerRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$addPeer, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseFuture<$1.DeletePeerResponse> deletePeer(
      $1.DeletePeerRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(
        _$deletePeer, $async.Stream.fromIterable([request]),
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
    $addMethod($grpc.ServiceMethod<$1.GetLatestRevisionEpochRequest,
            $1.GetLatestRevisionEpochResponse>(
        'GetLatestRevisionEpoch',
        getLatestRevisionEpoch_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $1.GetLatestRevisionEpochRequest.fromBuffer(value),
        ($1.GetLatestRevisionEpochResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$1.AddPeerRequest, $1.AddPeerResponse>(
        'AddPeer',
        addPeer_Pre,
        false,
        false,
        ($core.List<$core.int> value) => $1.AddPeerRequest.fromBuffer(value),
        ($1.AddPeerResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$1.DeletePeerRequest, $1.DeletePeerResponse>(
        'DeletePeer',
        deletePeer_Pre,
        false,
        false,
        ($core.List<$core.int> value) => $1.DeletePeerRequest.fromBuffer(value),
        ($1.DeletePeerResponse value) => value.writeToBuffer()));
  }

  $async.Future<$1.ReviseNodeStateResponse> reviseNodeState_Pre(
      $grpc.ServiceCall call,
      $async.Future<$1.ReviseNodeStateRequest> request) async {
    return reviseNodeState(call, await request);
  }

  $async.Future<$1.GetLatestRevisionEpochResponse> getLatestRevisionEpoch_Pre(
      $grpc.ServiceCall call,
      $async.Future<$1.GetLatestRevisionEpochRequest> request) async {
    return getLatestRevisionEpoch(call, await request);
  }

  $async.Future<$1.AddPeerResponse> addPeer_Pre(
      $grpc.ServiceCall call, $async.Future<$1.AddPeerRequest> request) async {
    return addPeer(call, await request);
  }

  $async.Future<$1.DeletePeerResponse> deletePeer_Pre($grpc.ServiceCall call,
      $async.Future<$1.DeletePeerRequest> request) async {
    return deletePeer(call, await request);
  }

  $async.Future<$1.ReviseNodeStateResponse> reviseNodeState(
      $grpc.ServiceCall call, $1.ReviseNodeStateRequest request);
  $async.Future<$1.GetLatestRevisionEpochResponse> getLatestRevisionEpoch(
      $grpc.ServiceCall call, $1.GetLatestRevisionEpochRequest request);
  $async.Future<$1.AddPeerResponse> addPeer(
      $grpc.ServiceCall call, $1.AddPeerRequest request);
  $async.Future<$1.DeletePeerResponse> deletePeer(
      $grpc.ServiceCall call, $1.DeletePeerRequest request);
}
