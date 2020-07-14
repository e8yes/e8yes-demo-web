///
//  Generated code. Do not modify.
//  source: service_system.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

class VersionResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('VersionResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOS(1, 'version')
    ..hasRequiredFields = false
  ;

  VersionResponse._() : super();
  factory VersionResponse() => create();
  factory VersionResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory VersionResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  VersionResponse clone() => VersionResponse()..mergeFromMessage(this);
  VersionResponse copyWith(void Function(VersionResponse) updates) => super.copyWith((message) => updates(message as VersionResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static VersionResponse create() => VersionResponse._();
  VersionResponse createEmptyInstance() => create();
  static $pb.PbList<VersionResponse> createRepeated() => $pb.PbList<VersionResponse>();
  @$core.pragma('dart2js:noInline')
  static VersionResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<VersionResponse>(create);
  static VersionResponse _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get version => $_getSZ(0);
  @$pb.TagNumber(1)
  set version($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasVersion() => $_has(0);
  @$pb.TagNumber(1)
  void clearVersion() => clearField(1);
}

class VersionResquest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('VersionResquest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  VersionResquest._() : super();
  factory VersionResquest() => create();
  factory VersionResquest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory VersionResquest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  VersionResquest clone() => VersionResquest()..mergeFromMessage(this);
  VersionResquest copyWith(void Function(VersionResquest) updates) => super.copyWith((message) => updates(message as VersionResquest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static VersionResquest create() => VersionResquest._();
  VersionResquest createEmptyInstance() => create();
  static $pb.PbList<VersionResquest> createRepeated() => $pb.PbList<VersionResquest>();
  @$core.pragma('dart2js:noInline')
  static VersionResquest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<VersionResquest>(create);
  static VersionResquest _defaultInstance;
}

