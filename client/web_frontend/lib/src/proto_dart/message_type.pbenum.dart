///
//  Generated code. Do not modify.
//  source: message_type.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

// ignore_for_file: UNDEFINED_SHOWN_NAME,UNUSED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class MessageType extends $pb.ProtobufEnum {
  static const MessageType MST_INVALID = MessageType._(0, 'MST_INVALID');
  static const MessageType MST_NOTIFICATION = MessageType._(1, 'MST_NOTIFICATION');
  static const MessageType MST_CHAT = MessageType._(2, 'MST_CHAT');
  static const MessageType MST_ACCOUNT_DATA = MessageType._(3, 'MST_ACCOUNT_DATA');

  static const $core.List<MessageType> values = <MessageType> [
    MST_INVALID,
    MST_NOTIFICATION,
    MST_CHAT,
    MST_ACCOUNT_DATA,
  ];

  static final $core.Map<$core.int, MessageType> _byValue = $pb.ProtobufEnum.initByValue(values);
  static MessageType valueOf($core.int value) => _byValue[value];

  const MessageType._($core.int v, $core.String n) : super(v, n);
}

