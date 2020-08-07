///
//  Generated code. Do not modify.
//  source: node.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

import 'node.pbenum.dart';

export 'node.pbenum.dart';

class Node extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('Node', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, 'ipAddress', $pb.PbFieldType.OY)
    ..aOS(2, 'name')
    ..pc<NodeType>(3, 'types', $pb.PbFieldType.PE, valueOf: NodeType.valueOf, enumValues: NodeType.values)
    ..p<$core.int>(4, 'ports', $pb.PbFieldType.P3)
    ..hasRequiredFields = false
  ;

  Node._() : super();
  factory Node() => create();
  factory Node.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Node.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  Node clone() => Node()..mergeFromMessage(this);
  Node copyWith(void Function(Node) updates) => super.copyWith((message) => updates(message as Node));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Node create() => Node._();
  Node createEmptyInstance() => create();
  static $pb.PbList<Node> createRepeated() => $pb.PbList<Node>();
  @$core.pragma('dart2js:noInline')
  static Node getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Node>(create);
  static Node _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get ipAddress => $_getN(0);
  @$pb.TagNumber(1)
  set ipAddress($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasIpAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearIpAddress() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get name => $_getSZ(1);
  @$pb.TagNumber(2)
  set name($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasName() => $_has(1);
  @$pb.TagNumber(2)
  void clearName() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<NodeType> get types => $_getList(2);

  @$pb.TagNumber(4)
  $core.List<$core.int> get ports => $_getList(3);
}

