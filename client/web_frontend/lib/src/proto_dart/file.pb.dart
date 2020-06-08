///
//  Generated code. Do not modify.
//  source: file.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'file.pbenum.dart';
import 'encryption_source.pbenum.dart' as $0;

export 'file.pbenum.dart';

class FileChunk extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('FileChunk', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..a<$core.int>(1, 'chunkNumber', $pb.PbFieldType.O3)
    ..a<$core.List<$core.int>>(2, 'data', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  FileChunk._() : super();
  factory FileChunk() => create();
  factory FileChunk.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory FileChunk.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  FileChunk clone() => FileChunk()..mergeFromMessage(this);
  FileChunk copyWith(void Function(FileChunk) updates) => super.copyWith((message) => updates(message as FileChunk));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static FileChunk create() => FileChunk._();
  FileChunk createEmptyInstance() => create();
  static $pb.PbList<FileChunk> createRepeated() => $pb.PbList<FileChunk>();
  @$core.pragma('dart2js:noInline')
  static FileChunk getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<FileChunk>(create);
  static FileChunk _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get chunkNumber => $_getIZ(0);
  @$pb.TagNumber(1)
  set chunkNumber($core.int v) { $_setSignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChunkNumber() => $_has(0);
  @$pb.TagNumber(1)
  void clearChunkNumber() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get data => $_getN(1);
  @$pb.TagNumber(2)
  set data($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasData() => $_has(1);
  @$pb.TagNumber(2)
  void clearData() => clearField(2);
}

class SignableFileAccess extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('SignableFileAccess', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'expiryTimestamp')
    ..aInt64(2, 'viewerId')
    ..aOS(3, 'filePath')
    ..e<FileAccessMode>(4, 'accessMode', $pb.PbFieldType.OE, defaultOrMaker: FileAccessMode.FAM_READ, valueOf: FileAccessMode.valueOf, enumValues: FileAccessMode.values)
    ..hasRequiredFields = false
  ;

  SignableFileAccess._() : super();
  factory SignableFileAccess() => create();
  factory SignableFileAccess.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SignableFileAccess.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  SignableFileAccess clone() => SignableFileAccess()..mergeFromMessage(this);
  SignableFileAccess copyWith(void Function(SignableFileAccess) updates) => super.copyWith((message) => updates(message as SignableFileAccess));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SignableFileAccess create() => SignableFileAccess._();
  SignableFileAccess createEmptyInstance() => create();
  static $pb.PbList<SignableFileAccess> createRepeated() => $pb.PbList<SignableFileAccess>();
  @$core.pragma('dart2js:noInline')
  static SignableFileAccess getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SignableFileAccess>(create);
  static SignableFileAccess _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get expiryTimestamp => $_getI64(0);
  @$pb.TagNumber(1)
  set expiryTimestamp($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasExpiryTimestamp() => $_has(0);
  @$pb.TagNumber(1)
  void clearExpiryTimestamp() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get viewerId => $_getI64(1);
  @$pb.TagNumber(2)
  set viewerId($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasViewerId() => $_has(1);
  @$pb.TagNumber(2)
  void clearViewerId() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get filePath => $_getSZ(2);
  @$pb.TagNumber(3)
  set filePath($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFilePath() => $_has(2);
  @$pb.TagNumber(3)
  void clearFilePath() => clearField(3);

  @$pb.TagNumber(4)
  FileAccessMode get accessMode => $_getN(3);
  @$pb.TagNumber(4)
  set accessMode(FileAccessMode v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasAccessMode() => $_has(3);
  @$pb.TagNumber(4)
  void clearAccessMode() => clearField(4);
}

class FileTokenAccess extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('FileTokenAccess', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, 'accessToken', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  FileTokenAccess._() : super();
  factory FileTokenAccess() => create();
  factory FileTokenAccess.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory FileTokenAccess.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  FileTokenAccess clone() => FileTokenAccess()..mergeFromMessage(this);
  FileTokenAccess copyWith(void Function(FileTokenAccess) updates) => super.copyWith((message) => updates(message as FileTokenAccess));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static FileTokenAccess create() => FileTokenAccess._();
  FileTokenAccess createEmptyInstance() => create();
  static $pb.PbList<FileTokenAccess> createRepeated() => $pb.PbList<FileTokenAccess>();
  @$core.pragma('dart2js:noInline')
  static FileTokenAccess getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<FileTokenAccess>(create);
  static FileTokenAccess _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get accessToken => $_getN(0);
  @$pb.TagNumber(1)
  set accessToken($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAccessToken() => $_has(0);
  @$pb.TagNumber(1)
  void clearAccessToken() => clearField(1);
}

class FileDirectAccess extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('FileDirectAccess', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOS(2, 'path')
    ..e<FileAccessMode>(3, 'accessMode', $pb.PbFieldType.OE, defaultOrMaker: FileAccessMode.FAM_READ, valueOf: FileAccessMode.valueOf, enumValues: FileAccessMode.values)
    ..hasRequiredFields = false
  ;

  FileDirectAccess._() : super();
  factory FileDirectAccess() => create();
  factory FileDirectAccess.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory FileDirectAccess.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  FileDirectAccess clone() => FileDirectAccess()..mergeFromMessage(this);
  FileDirectAccess copyWith(void Function(FileDirectAccess) updates) => super.copyWith((message) => updates(message as FileDirectAccess));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static FileDirectAccess create() => FileDirectAccess._();
  FileDirectAccess createEmptyInstance() => create();
  static $pb.PbList<FileDirectAccess> createRepeated() => $pb.PbList<FileDirectAccess>();
  @$core.pragma('dart2js:noInline')
  static FileDirectAccess getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<FileDirectAccess>(create);
  static FileDirectAccess _defaultInstance;

  @$pb.TagNumber(2)
  $core.String get path => $_getSZ(0);
  @$pb.TagNumber(2)
  set path($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(2)
  $core.bool hasPath() => $_has(0);
  @$pb.TagNumber(2)
  void clearPath() => clearField(2);

  @$pb.TagNumber(3)
  FileAccessMode get accessMode => $_getN(1);
  @$pb.TagNumber(3)
  set accessMode(FileAccessMode v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasAccessMode() => $_has(1);
  @$pb.TagNumber(3)
  void clearAccessMode() => clearField(3);
}

enum FileDescriptor_AccessMethod {
  fileTokenAccess, 
  fileDirectAccess, 
  notSet
}

class FileDescriptor extends $pb.GeneratedMessage {
  static const $core.Map<$core.int, FileDescriptor_AccessMethod> _FileDescriptor_AccessMethodByTag = {
    1 : FileDescriptor_AccessMethod.fileTokenAccess,
    2 : FileDescriptor_AccessMethod.fileDirectAccess,
    0 : FileDescriptor_AccessMethod.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('FileDescriptor', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..oo(0, [1, 2])
    ..aOM<FileTokenAccess>(1, 'fileTokenAccess', subBuilder: FileTokenAccess.create)
    ..aOM<FileDirectAccess>(2, 'fileDirectAccess', subBuilder: FileDirectAccess.create)
    ..aOS(3, 'fileName')
    ..e<FileFormat>(4, 'fileFormat', $pb.PbFieldType.OE, defaultOrMaker: FileFormat.FFMT_UNKNOWN, valueOf: FileFormat.valueOf, enumValues: FileFormat.values)
    ..e<$0.EncryptionSource>(5, 'encryptionSource', $pb.PbFieldType.OE, defaultOrMaker: $0.EncryptionSource.ESRC_NONE, valueOf: $0.EncryptionSource.valueOf, enumValues: $0.EncryptionSource.values)
    ..aInt64(6, 'storageSize')
    ..aInt64(7, 'createdAtTimestamp')
    ..aInt64(8, 'lastModifiedAtTimestamp')
    ..hasRequiredFields = false
  ;

  FileDescriptor._() : super();
  factory FileDescriptor() => create();
  factory FileDescriptor.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory FileDescriptor.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  FileDescriptor clone() => FileDescriptor()..mergeFromMessage(this);
  FileDescriptor copyWith(void Function(FileDescriptor) updates) => super.copyWith((message) => updates(message as FileDescriptor));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static FileDescriptor create() => FileDescriptor._();
  FileDescriptor createEmptyInstance() => create();
  static $pb.PbList<FileDescriptor> createRepeated() => $pb.PbList<FileDescriptor>();
  @$core.pragma('dart2js:noInline')
  static FileDescriptor getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<FileDescriptor>(create);
  static FileDescriptor _defaultInstance;

  FileDescriptor_AccessMethod whichAccessMethod() => _FileDescriptor_AccessMethodByTag[$_whichOneof(0)];
  void clearAccessMethod() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  FileTokenAccess get fileTokenAccess => $_getN(0);
  @$pb.TagNumber(1)
  set fileTokenAccess(FileTokenAccess v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasFileTokenAccess() => $_has(0);
  @$pb.TagNumber(1)
  void clearFileTokenAccess() => clearField(1);
  @$pb.TagNumber(1)
  FileTokenAccess ensureFileTokenAccess() => $_ensure(0);

  @$pb.TagNumber(2)
  FileDirectAccess get fileDirectAccess => $_getN(1);
  @$pb.TagNumber(2)
  set fileDirectAccess(FileDirectAccess v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasFileDirectAccess() => $_has(1);
  @$pb.TagNumber(2)
  void clearFileDirectAccess() => clearField(2);
  @$pb.TagNumber(2)
  FileDirectAccess ensureFileDirectAccess() => $_ensure(1);

  @$pb.TagNumber(3)
  $core.String get fileName => $_getSZ(2);
  @$pb.TagNumber(3)
  set fileName($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFileName() => $_has(2);
  @$pb.TagNumber(3)
  void clearFileName() => clearField(3);

  @$pb.TagNumber(4)
  FileFormat get fileFormat => $_getN(3);
  @$pb.TagNumber(4)
  set fileFormat(FileFormat v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasFileFormat() => $_has(3);
  @$pb.TagNumber(4)
  void clearFileFormat() => clearField(4);

  @$pb.TagNumber(5)
  $0.EncryptionSource get encryptionSource => $_getN(4);
  @$pb.TagNumber(5)
  set encryptionSource($0.EncryptionSource v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasEncryptionSource() => $_has(4);
  @$pb.TagNumber(5)
  void clearEncryptionSource() => clearField(5);

  @$pb.TagNumber(6)
  $fixnum.Int64 get storageSize => $_getI64(5);
  @$pb.TagNumber(6)
  set storageSize($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasStorageSize() => $_has(5);
  @$pb.TagNumber(6)
  void clearStorageSize() => clearField(6);

  @$pb.TagNumber(7)
  $fixnum.Int64 get createdAtTimestamp => $_getI64(6);
  @$pb.TagNumber(7)
  set createdAtTimestamp($fixnum.Int64 v) { $_setInt64(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasCreatedAtTimestamp() => $_has(6);
  @$pb.TagNumber(7)
  void clearCreatedAtTimestamp() => clearField(7);

  @$pb.TagNumber(8)
  $fixnum.Int64 get lastModifiedAtTimestamp => $_getI64(7);
  @$pb.TagNumber(8)
  set lastModifiedAtTimestamp($fixnum.Int64 v) { $_setInt64(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasLastModifiedAtTimestamp() => $_has(7);
  @$pb.TagNumber(8)
  void clearLastModifiedAtTimestamp() => clearField(8);
}

