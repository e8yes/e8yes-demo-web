///
//  Generated code. Do not modify.
//  source: node.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

// ignore_for_file: UNDEFINED_SHOWN_NAME,UNUSED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class NodeType extends $pb.ProtobufEnum {
  static const NodeType NDT_INVALID = NodeType._(0, 'NDT_INVALID');
  static const NodeType NDT_FILE_STORE = NodeType._(1, 'NDT_FILE_STORE');
  static const NodeType NDT_MESSAGE_STORE = NodeType._(2, 'NDT_MESSAGE_STORE');
  static const NodeType NDT_TASK_EXECUTOR = NodeType._(3, 'NDT_TASK_EXECUTOR');

  static const $core.List<NodeType> values = <NodeType> [
    NDT_INVALID,
    NDT_FILE_STORE,
    NDT_MESSAGE_STORE,
    NDT_TASK_EXECUTOR,
  ];

  static final $core.Map<$core.int, NodeType> _byValue = $pb.ProtobufEnum.initByValue(values);
  static NodeType valueOf($core.int value) => _byValue[value];

  const NodeType._($core.int v, $core.String n) : super(v, n);
}

