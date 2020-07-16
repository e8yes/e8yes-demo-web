///
//  Generated code. Do not modify.
//  source: message_channel.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

// ignore_for_file: UNDEFINED_SHOWN_NAME,UNUSED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class MessageChannelUserType extends $pb.ProtobufEnum {
  static const MessageChannelUserType MCUT_INVALID = MessageChannelUserType._(0, 'MCUT_INVALID');
  static const MessageChannelUserType MCUT_MEMBER = MessageChannelUserType._(1, 'MCUT_MEMBER');
  static const MessageChannelUserType MCUT_ADMIN = MessageChannelUserType._(2, 'MCUT_ADMIN');

  static const $core.List<MessageChannelUserType> values = <MessageChannelUserType> [
    MCUT_INVALID,
    MCUT_MEMBER,
    MCUT_ADMIN,
  ];

  static final $core.Map<$core.int, MessageChannelUserType> _byValue = $pb.ProtobufEnum.initByValue(values);
  static MessageChannelUserType valueOf($core.int value) => _byValue[value];

  const MessageChannelUserType._($core.int v, $core.String n) : super(v, n);
}

