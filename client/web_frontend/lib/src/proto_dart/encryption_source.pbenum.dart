///
//  Generated code. Do not modify.
//  source: encryption_source.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

// ignore_for_file: UNDEFINED_SHOWN_NAME,UNUSED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class EncryptionSource extends $pb.ProtobufEnum {
  static const EncryptionSource ESRC_NONE = EncryptionSource._(0, 'ESRC_NONE');
  static const EncryptionSource ESRC_SECURITY_KEY = EncryptionSource._(1, 'ESRC_SECURITY_KEY');
  static const EncryptionSource ESRC_MESSAGING_CHANNEL_KEY = EncryptionSource._(2, 'ESRC_MESSAGING_CHANNEL_KEY');

  static const $core.List<EncryptionSource> values = <EncryptionSource> [
    ESRC_NONE,
    ESRC_SECURITY_KEY,
    ESRC_MESSAGING_CHANNEL_KEY,
  ];

  static final $core.Map<$core.int, EncryptionSource> _byValue = $pb.ProtobufEnum.initByValue(values);
  static EncryptionSource valueOf($core.int value) => _byValue[value];

  const EncryptionSource._($core.int v, $core.String n) : super(v, n);
}

