///
//  Generated code. Do not modify.
//  source: user_relation.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

// ignore_for_file: UNDEFINED_SHOWN_NAME,UNUSED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class UserRelation extends $pb.ProtobufEnum {
  static const UserRelation URL_UNRELATED = UserRelation._(0, 'URL_UNRELATED');
  static const UserRelation URL_INVITATION_SENT = UserRelation._(1, 'URL_INVITATION_SENT');
  static const UserRelation URL_INVITATION_RECEIVED = UserRelation._(2, 'URL_INVITATION_RECEIVED');
  static const UserRelation URL_INVITATION_REJECTED = UserRelation._(3, 'URL_INVITATION_REJECTED');
  static const UserRelation URL_CONTACT = UserRelation._(4, 'URL_CONTACT');
  static const UserRelation URL_BLOCKING = UserRelation._(5, 'URL_BLOCKING');
  static const UserRelation URL_BLOCKED = UserRelation._(6, 'URL_BLOCKED');

  static const $core.List<UserRelation> values = <UserRelation> [
    URL_UNRELATED,
    URL_INVITATION_SENT,
    URL_INVITATION_RECEIVED,
    URL_INVITATION_REJECTED,
    URL_CONTACT,
    URL_BLOCKING,
    URL_BLOCKED,
  ];

  static final $core.Map<$core.int, UserRelation> _byValue = $pb.ProtobufEnum.initByValue(values);
  static UserRelation valueOf($core.int value) => _byValue[value];

  const UserRelation._($core.int v, $core.String n) : super(v, n);
}

