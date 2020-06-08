///
//  Generated code. Do not modify.
//  source: service_file.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'package:protobuf/protobuf.dart' as $pb;

import 'dart:core' as $core;
import 'service_file.pb.dart' as $8;
import 'service_file.pbjson.dart';

export 'service_file.pb.dart';

abstract class FileServiceBase extends $pb.GeneratedService {
  $async.Future<$8.UploadFileResponse> upload($pb.ServerContext ctx, $8.UploadFileRequest request);
  $async.Future<$8.DownloadFileResponse> download($pb.ServerContext ctx, $8.DownloadFileRequest request);

  $pb.GeneratedMessage createRequest($core.String method) {
    switch (method) {
      case 'Upload': return $8.UploadFileRequest();
      case 'Download': return $8.DownloadFileRequest();
      default: throw $core.ArgumentError('Unknown method: $method');
    }
  }

  $async.Future<$pb.GeneratedMessage> handleCall($pb.ServerContext ctx, $core.String method, $pb.GeneratedMessage request) {
    switch (method) {
      case 'Upload': return this.upload(ctx, request);
      case 'Download': return this.download(ctx, request);
      default: throw $core.ArgumentError('Unknown method: $method');
    }
  }

  $core.Map<$core.String, $core.dynamic> get $json => FileServiceBase$json;
  $core.Map<$core.String, $core.Map<$core.String, $core.dynamic>> get $messageJson => FileServiceBase$messageJson;
}

