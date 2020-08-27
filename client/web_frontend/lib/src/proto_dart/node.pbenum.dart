///
//  Generated code. Do not modify.
//  source: node.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

// ignore_for_file: UNDEFINED_SHOWN_NAME,UNUSED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class NodeFunction extends $pb.ProtobufEnum {
  static const NodeFunction NDF_INVALID = NodeFunction._(0, 'NDF_INVALID');
  static const NodeFunction NDF_DISTRIBUTOR = NodeFunction._(1, 'NDF_DISTRIBUTOR');
  static const NodeFunction NDF_FILE_STORE = NodeFunction._(2, 'NDF_FILE_STORE');
  static const NodeFunction NDF_MESSAGE_QUEUE = NodeFunction._(3, 'NDF_MESSAGE_QUEUE');
  static const NodeFunction NDF_TASK_EXECUTOR = NodeFunction._(4, 'NDF_TASK_EXECUTOR');
  static const NodeFunction NDF_RELATIONAL_STORE = NodeFunction._(5, 'NDF_RELATIONAL_STORE');
  static const NodeFunction NDF_LOAD_BALANCER = NodeFunction._(6, 'NDF_LOAD_BALANCER');
  static const NodeFunction NDF_IMAGE_BUILDER = NodeFunction._(7, 'NDF_IMAGE_BUILDER');

  static const $core.List<NodeFunction> values = <NodeFunction> [
    NDF_INVALID,
    NDF_DISTRIBUTOR,
    NDF_FILE_STORE,
    NDF_MESSAGE_QUEUE,
    NDF_TASK_EXECUTOR,
    NDF_RELATIONAL_STORE,
    NDF_LOAD_BALANCER,
    NDF_IMAGE_BUILDER,
  ];

  static final $core.Map<$core.int, NodeFunction> _byValue = $pb.ProtobufEnum.initByValue(values);
  static NodeFunction valueOf($core.int value) => _byValue[value];

  const NodeFunction._($core.int v, $core.String n) : super(v, n);
}

class NodeStatus extends $pb.ProtobufEnum {
  static const NodeStatus NDS_INVALID = NodeStatus._(0, 'NDS_INVALID');
  static const NodeStatus NDS_INITIALIZING = NodeStatus._(1, 'NDS_INITIALIZING');
  static const NodeStatus NDS_READY = NodeStatus._(2, 'NDS_READY');
  static const NodeStatus NDS_UNAVALIABLE = NodeStatus._(3, 'NDS_UNAVALIABLE');

  static const $core.List<NodeStatus> values = <NodeStatus> [
    NDS_INVALID,
    NDS_INITIALIZING,
    NDS_READY,
    NDS_UNAVALIABLE,
  ];

  static final $core.Map<$core.int, NodeStatus> _byValue = $pb.ProtobufEnum.initByValue(values);
  static NodeStatus valueOf($core.int value) => _byValue[value];

  const NodeStatus._($core.int v, $core.String n) : super(v, n);
}

