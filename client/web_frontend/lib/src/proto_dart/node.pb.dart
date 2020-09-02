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
import 'delta.pbenum.dart' as $16;

export 'node.pbenum.dart';

class NodeState extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('NodeState', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOS(1, 'name')
    ..a<$core.List<$core.int>>(2, 'ipAddress', $pb.PbFieldType.OY)
    ..e<NodeStatus>(3, 'status', $pb.PbFieldType.OE, defaultOrMaker: NodeStatus.NDS_INVALID, valueOf: NodeStatus.valueOf, enumValues: NodeStatus.values)
    ..pc<NodeFunction>(4, 'functions', $pb.PbFieldType.PE, valueOf: NodeFunction.valueOf, enumValues: NodeFunction.values)
    ..p<$core.int>(5, 'functionPorts', $pb.PbFieldType.P3)
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
  $core.String get name => $_getSZ(0);
  @$pb.TagNumber(1)
  set name($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasName() => $_has(0);
  @$pb.TagNumber(1)
  void clearName() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get ipAddress => $_getN(1);
  @$pb.TagNumber(2)
  set ipAddress($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasIpAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearIpAddress() => clearField(2);

  @$pb.TagNumber(3)
  NodeStatus get status => $_getN(2);
  @$pb.TagNumber(3)
  set status(NodeStatus v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasStatus() => $_has(2);
  @$pb.TagNumber(3)
  void clearStatus() => clearField(3);

  @$pb.TagNumber(4)
  $core.List<NodeFunction> get functions => $_getList(3);

  @$pb.TagNumber(5)
  $core.List<$core.int> get functionPorts => $_getList(4);
}

class NodeStateRevision extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('NodeStateRevision', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'revisionEpoch')
    ..m<$core.String, NodeState>(3, 'nodes', entryClassName: 'NodeStateRevision.NodesEntry', keyFieldType: $pb.PbFieldType.OS, valueFieldType: $pb.PbFieldType.OM, valueCreator: NodeState.create, packageName: const $pb.PackageName('e8'))
    ..m<$core.String, $16.DeltaOperation>(4, 'deltaOperations', entryClassName: 'NodeStateRevision.DeltaOperationsEntry', keyFieldType: $pb.PbFieldType.OS, valueFieldType: $pb.PbFieldType.OE, valueOf: $16.DeltaOperation.valueOf, enumValues: $16.DeltaOperation.values, packageName: const $pb.PackageName('e8'))
    ..hasRequiredFields = false
  ;

  NodeStateRevision._() : super();
  factory NodeStateRevision() => create();
  factory NodeStateRevision.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NodeStateRevision.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  NodeStateRevision clone() => NodeStateRevision()..mergeFromMessage(this);
  NodeStateRevision copyWith(void Function(NodeStateRevision) updates) => super.copyWith((message) => updates(message as NodeStateRevision));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static NodeStateRevision create() => NodeStateRevision._();
  NodeStateRevision createEmptyInstance() => create();
  static $pb.PbList<NodeStateRevision> createRepeated() => $pb.PbList<NodeStateRevision>();
  @$core.pragma('dart2js:noInline')
  static NodeStateRevision getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NodeStateRevision>(create);
  static NodeStateRevision _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get revisionEpoch => $_getI64(0);
  @$pb.TagNumber(1)
  set revisionEpoch($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasRevisionEpoch() => $_has(0);
  @$pb.TagNumber(1)
  void clearRevisionEpoch() => clearField(1);

  @$pb.TagNumber(3)
  $core.Map<$core.String, NodeState> get nodes => $_getMap(1);

  @$pb.TagNumber(4)
  $core.Map<$core.String, $16.DeltaOperation> get deltaOperations => $_getMap(2);
}

