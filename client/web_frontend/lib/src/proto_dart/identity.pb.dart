///
//  Generated code. Do not modify.
//  source: identity.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

class Identity extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('Identity', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'expiryTimestamp')
    ..aInt64(2, 'userId')
    ..pPS(3, 'groupNames')
    ..hasRequiredFields = false
  ;

  Identity._() : super();
  factory Identity() => create();
  factory Identity.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Identity.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  Identity clone() => Identity()..mergeFromMessage(this);
  Identity copyWith(void Function(Identity) updates) => super.copyWith((message) => updates(message as Identity));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Identity create() => Identity._();
  Identity createEmptyInstance() => create();
  static $pb.PbList<Identity> createRepeated() => $pb.PbList<Identity>();
  @$core.pragma('dart2js:noInline')
  static Identity getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Identity>(create);
  static Identity _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get expiryTimestamp => $_getI64(0);
  @$pb.TagNumber(1)
  set expiryTimestamp($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasExpiryTimestamp() => $_has(0);
  @$pb.TagNumber(1)
  void clearExpiryTimestamp() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get userId => $_getI64(1);
  @$pb.TagNumber(2)
  set userId($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasUserId() => $_has(1);
  @$pb.TagNumber(2)
  void clearUserId() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<$core.String> get groupNames => $_getList(2);
}

class IdentitySignature extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('IdentitySignature', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, 'signature', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  IdentitySignature._() : super();
  factory IdentitySignature() => create();
  factory IdentitySignature.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory IdentitySignature.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  IdentitySignature clone() => IdentitySignature()..mergeFromMessage(this);
  IdentitySignature copyWith(void Function(IdentitySignature) updates) => super.copyWith((message) => updates(message as IdentitySignature));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static IdentitySignature create() => IdentitySignature._();
  IdentitySignature createEmptyInstance() => create();
  static $pb.PbList<IdentitySignature> createRepeated() => $pb.PbList<IdentitySignature>();
  @$core.pragma('dart2js:noInline')
  static IdentitySignature getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<IdentitySignature>(create);
  static IdentitySignature _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get signature => $_getN(0);
  @$pb.TagNumber(1)
  set signature($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSignature() => $_has(0);
  @$pb.TagNumber(1)
  void clearSignature() => clearField(1);
}

