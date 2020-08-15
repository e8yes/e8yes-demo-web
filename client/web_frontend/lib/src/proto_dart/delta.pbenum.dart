///
//  Generated code. Do not modify.
//  source: delta.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

// ignore_for_file: UNDEFINED_SHOWN_NAME,UNUSED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class DeltaOperation extends $pb.ProtobufEnum {
  static const DeltaOperation DOP_INVALID = DeltaOperation._(0, 'DOP_INVALID');
  static const DeltaOperation DOP_ADD = DeltaOperation._(1, 'DOP_ADD');
  static const DeltaOperation DOP_DELETE = DeltaOperation._(2, 'DOP_DELETE');
  static const DeltaOperation DOP_SWAP = DeltaOperation._(3, 'DOP_SWAP');

  static const $core.List<DeltaOperation> values = <DeltaOperation> [
    DOP_INVALID,
    DOP_ADD,
    DOP_DELETE,
    DOP_SWAP,
  ];

  static final $core.Map<$core.int, DeltaOperation> _byValue = $pb.ProtobufEnum.initByValue(values);
  static DeltaOperation valueOf($core.int value) => _byValue[value];

  const DeltaOperation._($core.int v, $core.String n) : super(v, n);
}

