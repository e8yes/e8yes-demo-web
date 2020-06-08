///
//  Generated code. Do not modify.
//  source: nullable_primitives.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

class NullableInt32 extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('NullableInt32', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..a<$core.int>(1, 'value', $pb.PbFieldType.O3)
    ..hasRequiredFields = false
  ;

  NullableInt32._() : super();
  factory NullableInt32() => create();
  factory NullableInt32.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NullableInt32.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  NullableInt32 clone() => NullableInt32()..mergeFromMessage(this);
  NullableInt32 copyWith(void Function(NullableInt32) updates) => super.copyWith((message) => updates(message as NullableInt32));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static NullableInt32 create() => NullableInt32._();
  NullableInt32 createEmptyInstance() => create();
  static $pb.PbList<NullableInt32> createRepeated() => $pb.PbList<NullableInt32>();
  @$core.pragma('dart2js:noInline')
  static NullableInt32 getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NullableInt32>(create);
  static NullableInt32 _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get value => $_getIZ(0);
  @$pb.TagNumber(1)
  set value($core.int v) { $_setSignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValue() => $_has(0);
  @$pb.TagNumber(1)
  void clearValue() => clearField(1);
}

class NullableInt64 extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('NullableInt64', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'value')
    ..hasRequiredFields = false
  ;

  NullableInt64._() : super();
  factory NullableInt64() => create();
  factory NullableInt64.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NullableInt64.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  NullableInt64 clone() => NullableInt64()..mergeFromMessage(this);
  NullableInt64 copyWith(void Function(NullableInt64) updates) => super.copyWith((message) => updates(message as NullableInt64));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static NullableInt64 create() => NullableInt64._();
  NullableInt64 createEmptyInstance() => create();
  static $pb.PbList<NullableInt64> createRepeated() => $pb.PbList<NullableInt64>();
  @$core.pragma('dart2js:noInline')
  static NullableInt64 getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NullableInt64>(create);
  static NullableInt64 _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get value => $_getI64(0);
  @$pb.TagNumber(1)
  set value($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValue() => $_has(0);
  @$pb.TagNumber(1)
  void clearValue() => clearField(1);
}

class NullableFloat extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('NullableFloat', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..a<$core.double>(1, 'value', $pb.PbFieldType.OF)
    ..hasRequiredFields = false
  ;

  NullableFloat._() : super();
  factory NullableFloat() => create();
  factory NullableFloat.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NullableFloat.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  NullableFloat clone() => NullableFloat()..mergeFromMessage(this);
  NullableFloat copyWith(void Function(NullableFloat) updates) => super.copyWith((message) => updates(message as NullableFloat));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static NullableFloat create() => NullableFloat._();
  NullableFloat createEmptyInstance() => create();
  static $pb.PbList<NullableFloat> createRepeated() => $pb.PbList<NullableFloat>();
  @$core.pragma('dart2js:noInline')
  static NullableFloat getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NullableFloat>(create);
  static NullableFloat _defaultInstance;

  @$pb.TagNumber(1)
  $core.double get value => $_getN(0);
  @$pb.TagNumber(1)
  set value($core.double v) { $_setFloat(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValue() => $_has(0);
  @$pb.TagNumber(1)
  void clearValue() => clearField(1);
}

class NullableDouble extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('NullableDouble', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..a<$core.double>(1, 'value', $pb.PbFieldType.OD)
    ..hasRequiredFields = false
  ;

  NullableDouble._() : super();
  factory NullableDouble() => create();
  factory NullableDouble.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NullableDouble.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  NullableDouble clone() => NullableDouble()..mergeFromMessage(this);
  NullableDouble copyWith(void Function(NullableDouble) updates) => super.copyWith((message) => updates(message as NullableDouble));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static NullableDouble create() => NullableDouble._();
  NullableDouble createEmptyInstance() => create();
  static $pb.PbList<NullableDouble> createRepeated() => $pb.PbList<NullableDouble>();
  @$core.pragma('dart2js:noInline')
  static NullableDouble getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NullableDouble>(create);
  static NullableDouble _defaultInstance;

  @$pb.TagNumber(1)
  $core.double get value => $_getN(0);
  @$pb.TagNumber(1)
  set value($core.double v) { $_setDouble(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValue() => $_has(0);
  @$pb.TagNumber(1)
  void clearValue() => clearField(1);
}

class NullableString extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('NullableString', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOS(1, 'value')
    ..hasRequiredFields = false
  ;

  NullableString._() : super();
  factory NullableString() => create();
  factory NullableString.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NullableString.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  NullableString clone() => NullableString()..mergeFromMessage(this);
  NullableString copyWith(void Function(NullableString) updates) => super.copyWith((message) => updates(message as NullableString));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static NullableString create() => NullableString._();
  NullableString createEmptyInstance() => create();
  static $pb.PbList<NullableString> createRepeated() => $pb.PbList<NullableString>();
  @$core.pragma('dart2js:noInline')
  static NullableString getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NullableString>(create);
  static NullableString _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get value => $_getSZ(0);
  @$pb.TagNumber(1)
  set value($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValue() => $_has(0);
  @$pb.TagNumber(1)
  void clearValue() => clearField(1);
}

