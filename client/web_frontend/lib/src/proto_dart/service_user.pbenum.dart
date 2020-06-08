///
//  Generated code. Do not modify.
//  source: service_user.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

// ignore_for_file: UNDEFINED_SHOWN_NAME,UNUSED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class RegistrationReponse_RegistrationErrorType extends $pb.ProtobufEnum {
  static const RegistrationReponse_RegistrationErrorType RET_NoError = RegistrationReponse_RegistrationErrorType._(0, 'RET_NoError');
  static const RegistrationReponse_RegistrationErrorType RET_BadSecurityKey = RegistrationReponse_RegistrationErrorType._(1, 'RET_BadSecurityKey');
  static const RegistrationReponse_RegistrationErrorType RET_BotDetected = RegistrationReponse_RegistrationErrorType._(2, 'RET_BotDetected');

  static const $core.List<RegistrationReponse_RegistrationErrorType> values = <RegistrationReponse_RegistrationErrorType> [
    RET_NoError,
    RET_BadSecurityKey,
    RET_BotDetected,
  ];

  static final $core.Map<$core.int, RegistrationReponse_RegistrationErrorType> _byValue = $pb.ProtobufEnum.initByValue(values);
  static RegistrationReponse_RegistrationErrorType valueOf($core.int value) => _byValue[value];

  const RegistrationReponse_RegistrationErrorType._($core.int v, $core.String n) : super(v, n);
}

