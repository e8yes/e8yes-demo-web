///
//  Generated code. Do not modify.
//  source: service_file.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

import 'file.pb.dart' as $5;

class UploadFileRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UploadFileRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$5.FileDescriptor>(1, 'fileDescriptor', subBuilder: $5.FileDescriptor.create)
    ..aOM<$5.FileChunk>(2, 'currentChunk', subBuilder: $5.FileChunk.create)
    ..hasRequiredFields = false
  ;

  UploadFileRequest._() : super();
  factory UploadFileRequest() => create();
  factory UploadFileRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UploadFileRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  UploadFileRequest clone() => UploadFileRequest()..mergeFromMessage(this);
  UploadFileRequest copyWith(void Function(UploadFileRequest) updates) => super.copyWith((message) => updates(message as UploadFileRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static UploadFileRequest create() => UploadFileRequest._();
  UploadFileRequest createEmptyInstance() => create();
  static $pb.PbList<UploadFileRequest> createRepeated() => $pb.PbList<UploadFileRequest>();
  @$core.pragma('dart2js:noInline')
  static UploadFileRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UploadFileRequest>(create);
  static UploadFileRequest _defaultInstance;

  @$pb.TagNumber(1)
  $5.FileDescriptor get fileDescriptor => $_getN(0);
  @$pb.TagNumber(1)
  set fileDescriptor($5.FileDescriptor v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasFileDescriptor() => $_has(0);
  @$pb.TagNumber(1)
  void clearFileDescriptor() => clearField(1);
  @$pb.TagNumber(1)
  $5.FileDescriptor ensureFileDescriptor() => $_ensure(0);

  @$pb.TagNumber(2)
  $5.FileChunk get currentChunk => $_getN(1);
  @$pb.TagNumber(2)
  set currentChunk($5.FileChunk v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasCurrentChunk() => $_has(1);
  @$pb.TagNumber(2)
  void clearCurrentChunk() => clearField(2);
  @$pb.TagNumber(2)
  $5.FileChunk ensureCurrentChunk() => $_ensure(1);
}

class UploadFileResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UploadFileResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  UploadFileResponse._() : super();
  factory UploadFileResponse() => create();
  factory UploadFileResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UploadFileResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  UploadFileResponse clone() => UploadFileResponse()..mergeFromMessage(this);
  UploadFileResponse copyWith(void Function(UploadFileResponse) updates) => super.copyWith((message) => updates(message as UploadFileResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static UploadFileResponse create() => UploadFileResponse._();
  UploadFileResponse createEmptyInstance() => create();
  static $pb.PbList<UploadFileResponse> createRepeated() => $pb.PbList<UploadFileResponse>();
  @$core.pragma('dart2js:noInline')
  static UploadFileResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UploadFileResponse>(create);
  static UploadFileResponse _defaultInstance;
}

class DownloadFileRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DownloadFileRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$5.FileDescriptor>(1, 'fileDescriptor', subBuilder: $5.FileDescriptor.create)
    ..hasRequiredFields = false
  ;

  DownloadFileRequest._() : super();
  factory DownloadFileRequest() => create();
  factory DownloadFileRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DownloadFileRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DownloadFileRequest clone() => DownloadFileRequest()..mergeFromMessage(this);
  DownloadFileRequest copyWith(void Function(DownloadFileRequest) updates) => super.copyWith((message) => updates(message as DownloadFileRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DownloadFileRequest create() => DownloadFileRequest._();
  DownloadFileRequest createEmptyInstance() => create();
  static $pb.PbList<DownloadFileRequest> createRepeated() => $pb.PbList<DownloadFileRequest>();
  @$core.pragma('dart2js:noInline')
  static DownloadFileRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DownloadFileRequest>(create);
  static DownloadFileRequest _defaultInstance;

  @$pb.TagNumber(1)
  $5.FileDescriptor get fileDescriptor => $_getN(0);
  @$pb.TagNumber(1)
  set fileDescriptor($5.FileDescriptor v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasFileDescriptor() => $_has(0);
  @$pb.TagNumber(1)
  void clearFileDescriptor() => clearField(1);
  @$pb.TagNumber(1)
  $5.FileDescriptor ensureFileDescriptor() => $_ensure(0);
}

class DownloadFileResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DownloadFileResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$5.FileDescriptor>(1, 'fileDescriptor', subBuilder: $5.FileDescriptor.create)
    ..aOM<$5.FileChunk>(2, 'currentChunk', subBuilder: $5.FileChunk.create)
    ..hasRequiredFields = false
  ;

  DownloadFileResponse._() : super();
  factory DownloadFileResponse() => create();
  factory DownloadFileResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DownloadFileResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DownloadFileResponse clone() => DownloadFileResponse()..mergeFromMessage(this);
  DownloadFileResponse copyWith(void Function(DownloadFileResponse) updates) => super.copyWith((message) => updates(message as DownloadFileResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DownloadFileResponse create() => DownloadFileResponse._();
  DownloadFileResponse createEmptyInstance() => create();
  static $pb.PbList<DownloadFileResponse> createRepeated() => $pb.PbList<DownloadFileResponse>();
  @$core.pragma('dart2js:noInline')
  static DownloadFileResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DownloadFileResponse>(create);
  static DownloadFileResponse _defaultInstance;

  @$pb.TagNumber(1)
  $5.FileDescriptor get fileDescriptor => $_getN(0);
  @$pb.TagNumber(1)
  set fileDescriptor($5.FileDescriptor v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasFileDescriptor() => $_has(0);
  @$pb.TagNumber(1)
  void clearFileDescriptor() => clearField(1);
  @$pb.TagNumber(1)
  $5.FileDescriptor ensureFileDescriptor() => $_ensure(0);

  @$pb.TagNumber(2)
  $5.FileChunk get currentChunk => $_getN(1);
  @$pb.TagNumber(2)
  set currentChunk($5.FileChunk v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasCurrentChunk() => $_has(1);
  @$pb.TagNumber(2)
  void clearCurrentChunk() => clearField(2);
  @$pb.TagNumber(2)
  $5.FileChunk ensureCurrentChunk() => $_ensure(1);
}

