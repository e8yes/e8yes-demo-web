///
//  Generated code. Do not modify.
//  source: service_file.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'service_file.pb.dart' as $3;
export 'service_file.pb.dart';

class FileServiceClient extends $grpc.Client {
  static final _$upload =
      $grpc.ClientMethod<$3.UploadFileRequest, $3.UploadFileResponse>(
          '/e8.FileService/Upload',
          ($3.UploadFileRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $3.UploadFileResponse.fromBuffer(value));
  static final _$download =
      $grpc.ClientMethod<$3.DownloadFileRequest, $3.DownloadFileResponse>(
          '/e8.FileService/Download',
          ($3.DownloadFileRequest value) => value.writeToBuffer(),
          ($core.List<$core.int> value) =>
              $3.DownloadFileResponse.fromBuffer(value));

  FileServiceClient($grpc.ClientChannel channel, {$grpc.CallOptions options})
      : super(channel, options: options);

  $grpc.ResponseFuture<$3.UploadFileResponse> upload(
      $async.Stream<$3.UploadFileRequest> request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$upload, request, options: options);
    return $grpc.ResponseFuture(call);
  }

  $grpc.ResponseStream<$3.DownloadFileResponse> download(
      $3.DownloadFileRequest request,
      {$grpc.CallOptions options}) {
    final call = $createCall(_$download, $async.Stream.fromIterable([request]),
        options: options);
    return $grpc.ResponseStream(call);
  }
}

abstract class FileServiceBase extends $grpc.Service {
  $core.String get $name => 'e8.FileService';

  FileServiceBase() {
    $addMethod($grpc.ServiceMethod<$3.UploadFileRequest, $3.UploadFileResponse>(
        'Upload',
        upload,
        true,
        false,
        ($core.List<$core.int> value) => $3.UploadFileRequest.fromBuffer(value),
        ($3.UploadFileResponse value) => value.writeToBuffer()));
    $addMethod(
        $grpc.ServiceMethod<$3.DownloadFileRequest, $3.DownloadFileResponse>(
            'Download',
            download_Pre,
            false,
            true,
            ($core.List<$core.int> value) =>
                $3.DownloadFileRequest.fromBuffer(value),
            ($3.DownloadFileResponse value) => value.writeToBuffer()));
  }

  $async.Stream<$3.DownloadFileResponse> download_Pre($grpc.ServiceCall call,
      $async.Future<$3.DownloadFileRequest> request) async* {
    yield* download(call, await request);
  }

  $async.Future<$3.UploadFileResponse> upload(
      $grpc.ServiceCall call, $async.Stream<$3.UploadFileRequest> request);
  $async.Stream<$3.DownloadFileResponse> download(
      $grpc.ServiceCall call, $3.DownloadFileRequest request);
}
