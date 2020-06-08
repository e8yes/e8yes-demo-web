///
//  Generated code. Do not modify.
//  source: service_system.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'package:protobuf/protobuf.dart' as $pb;

import 'dart:core' as $core;
import 'service_system.pb.dart' as $7;
import 'service_system.pbjson.dart';

export 'service_system.pb.dart';

abstract class SystemServiceBase extends $pb.GeneratedService {
  $async.Future<$7.VersionResponse> version($pb.ServerContext ctx, $7.Empty request);

  $pb.GeneratedMessage createRequest($core.String method) {
    switch (method) {
      case 'Version': return $7.Empty();
      default: throw $core.ArgumentError('Unknown method: $method');
    }
  }

  $async.Future<$pb.GeneratedMessage> handleCall($pb.ServerContext ctx, $core.String method, $pb.GeneratedMessage request) {
    switch (method) {
      case 'Version': return this.version(ctx, request);
      default: throw $core.ArgumentError('Unknown method: $method');
    }
  }

  $core.Map<$core.String, $core.dynamic> get $json => SystemServiceBase$json;
  $core.Map<$core.String, $core.Map<$core.String, $core.dynamic>> get $messageJson => SystemServiceBase$messageJson;
}

