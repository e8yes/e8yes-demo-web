///
//  Generated code. Do not modify.
//  source: service_system.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_system.pb.dart' as $5;
export 'service_system.pb.dart';

class SystemServiceClient extends $grpc.Client {
  static final _$version =
      $grpc.ClientMethod<$5.VersionResquest, $5.VersionResponse>(
          '/e8.SystemService/Version',
          ($5.VersionResquest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $5.VersionResponse.fromBuffer(value));

  SystemServiceClient($grpc.ClientChannel channel, {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$5.VersionResponse> version($5.VersionResquest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$version, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseFuture(call);
  }
}

abstract class SystemServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.SystemService';

  SystemServiceBase() {
    $addMethod($grpc.ServiceMethod<$5.VersionResquest, $5.VersionResponse>(
        'Version',
        version_Pre,
        false,
        false,
        ($core.List<$core.int> value) => $5.VersionResquest.fromBuffer(value),
        ($5.VersionResponse value) => value.writeToBuffer()));
  }

  $async.Future<$5.VersionResponse> version_Pre(
      $grpc.ServiceCall call, $async.Future<$5.VersionResquest> request) async {
    return version(call, await request);
  }

  $async.Future<$5.VersionResponse> version(
      $grpc.ServiceCall call, $5.VersionResquest request);
}
