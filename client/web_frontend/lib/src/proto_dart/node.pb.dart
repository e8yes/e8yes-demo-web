///
//  Generated code. Do not modify.
//  source: node.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'node.pbenum.dart';

export 'node.pbenum.dart';

class NodeState extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('NodeState', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'clock')
    ..a<$core.List<$core.int>>(2, 'ipAddress', $pb.PbFieldType.OY)
    ..aOS(3, 'name')
    ..pc<NodeType>(4, 'types', $pb.PbFieldType.PE, valueOf: NodeType.valueOf, enumValues: NodeType.values)
    ..p<$core.int>(5, 'ports', $pb.PbFieldType.P3)
    ..hasRequiredFields = false
  ;

  NodeState._() : super();
  factory NodeState() => create();
  factory NodeState.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NodeState.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  NodeState clone() => NodeState()..mergeFromMessage(this);
  NodeState copyWith(void Function(NodeState) updates) => super.copyWith((message) => updates(message as NodeState));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static NodeState create() => NodeState._();
  NodeState createEmptyInstance() => create();
  static $pb.PbList<NodeState> createRepeated() => $pb.PbList<NodeState>();
  @$core.pragma('dart2js:noInline')
  static NodeState getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NodeState>(create);
  static NodeState _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get clock => $_getI64(0);
  @$pb.TagNumber(1)
  set clock($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasClock() => $_has(0);
  @$pb.TagNumber(1)
  void clearClock() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get ipAddress => $_getN(1);
  @$pb.TagNumber(2)
  set ipAddress($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasIpAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearIpAddress() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get name => $_getSZ(2);
  @$pb.TagNumber(3)
  set name($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasName() => $_has(2);
  @$pb.TagNumber(3)
  void clearName() => clearField(3);

  @$pb.TagNumber(4)
  $core.List<NodeType> get types => $_getList(3);

  @$pb.TagNumber(5)
  $core.List<$core.int> get ports => $_getList(4);
}

