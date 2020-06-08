///
//  Generated code. Do not modify.
//  source: permission.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

// ignore_for_file: UNDEFINED_SHOWN_NAME,UNUSED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class Permission extends $pb.ProtobufEnum {
  static const Permission PERM_USER_GROUP_CREATE = Permission._(0, 'PERM_USER_GROUP_CREATE');
  static const Permission PERM_USER_GROUP_REMOVE = Permission._(1, 'PERM_USER_GROUP_REMOVE');
  static const Permission PERM_USER_GROUP_VIEW = Permission._(10, 'PERM_USER_GROUP_VIEW');
  static const Permission PERM_USER_GROUP_ASSIGN = Permission._(11, 'PERM_USER_GROUP_ASSIGN');
  static const Permission PERM_USER_GROUP_UNASSIGN = Permission._(12, 'PERM_USER_GROUP_UNASSIGN');
  static const Permission PERM_USER_SELF_CREATE = Permission._(20, 'PERM_USER_SELF_CREATE');
  static const Permission PERM_USER_SELF_REMOVE = Permission._(21, 'PERM_USER_SELF_REMOVE');
  static const Permission PERM_USER_CREATE = Permission._(22, 'PERM_USER_CREATE');
  static const Permission PERM_USER_REMOVE = Permission._(23, 'PERM_USER_REMOVE');

  static const $core.List<Permission> values = <Permission> [
    PERM_USER_GROUP_CREATE,
    PERM_USER_GROUP_REMOVE,
    PERM_USER_GROUP_VIEW,
    PERM_USER_GROUP_ASSIGN,
    PERM_USER_GROUP_UNASSIGN,
    PERM_USER_SELF_CREATE,
    PERM_USER_SELF_REMOVE,
    PERM_USER_CREATE,
    PERM_USER_REMOVE,
  ];

  static final $core.Map<$core.int, Permission> _byValue = $pb.ProtobufEnum.initByValue(values);
  static Permission valueOf($core.int value) => _byValue[value];

  const Permission._($core.int v, $core.String n) : super(v, n);
}

