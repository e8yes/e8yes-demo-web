///
//  Generated code. Do not modify.
//  source: invitation_status.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

// ignore_for_file: UNDEFINED_SHOWN_NAME,UNUSED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class InvitationStatus extends $pb.ProtobufEnum {
  static const InvitationStatus UNKNOWN = InvitationStatus._(0, 'UNKNOWN');
  static const InvitationStatus PENDING = InvitationStatus._(1, 'PENDING');
  static const InvitationStatus ACCEPTED = InvitationStatus._(2, 'ACCEPTED');
  static const InvitationStatus REJECTED = InvitationStatus._(3, 'REJECTED');
  static const InvitationStatus HIDDEN = InvitationStatus._(4, 'HIDDEN');

  static const $core.List<InvitationStatus> values = <InvitationStatus> [
    UNKNOWN,
    PENDING,
    ACCEPTED,
    REJECTED,
    HIDDEN,
  ];

  static final $core.Map<$core.int, InvitationStatus> _byValue = $pb.ProtobufEnum.initByValue(values);
  static InvitationStatus valueOf($core.int value) => _byValue[value];

  const InvitationStatus._($core.int v, $core.String n) : super(v, n);
}

