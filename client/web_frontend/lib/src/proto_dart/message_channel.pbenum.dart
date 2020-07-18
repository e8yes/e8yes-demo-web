///
//  Generated code. Do not modify.
//  source: message_channel.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

// ignore_for_file: UNDEFINED_SHOWN_NAME,UNUSED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class MessageChannelMemberType extends $pb.ProtobufEnum {
  static const MessageChannelMemberType MCMT_INVALID = MessageChannelMemberType._(0, 'MCMT_INVALID');
  static const MessageChannelMemberType MCMT_MEMBER = MessageChannelMemberType._(1, 'MCMT_MEMBER');
  static const MessageChannelMemberType MCMT_ADMIN = MessageChannelMemberType._(2, 'MCMT_ADMIN');

  static const $core.List<MessageChannelMemberType> values = <MessageChannelMemberType> [
    MCMT_INVALID,
    MCMT_MEMBER,
    MCMT_ADMIN,
  ];

  static final $core.Map<$core.int, MessageChannelMemberType> _byValue = $pb.ProtobufEnum.initByValue(values);
  static MessageChannelMemberType valueOf($core.int value) => _byValue[value];

  const MessageChannelMemberType._($core.int v, $core.String n) : super(v, n);
}

