///
//  Generated code. Do not modify.
//  source: service_system.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_system.pb.dart' as $4;
export 'service_system.pb.dart';

class SystemServiceClient extends $grpc.Client {
  static final _$version =
      $grpc.ClientMethod<$4.VersionResquest, $4.VersionResponse>(
          '/e8.SystemService/Version',
          ($4.VersionResquest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $4.VersionResponse.fromBuffer(value));

  SystemServiceClient($grpc.ClientChannel channel, {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$4.VersionResponse> version($4.VersionResquest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$version, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }
}

abstract class SystemServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.SystemService';

  SystemServiceBase() {
    $addMethod($grpc.ServiceMethod<$4.VersionResquest, $4.VersionResponse>(
        'Version',
        version_Pre,
        false,
        false,
        ($core.List<$core.int> value) => $4.VersionResquest.fromBuffer(value),
        ($4.VersionResponse value) => value.writeToBuffer()));
  }

  $async.Future<$4.VersionResponse> version_Pre(
      $grpc.ServiceCall call, $async.Future<$4.VersionResquest> request) async {
    return version(call, await request);
  }

  $async.Future<$4.VersionResponse> version(
      $grpc.ServiceCall call, $4.VersionResquest request);
}
