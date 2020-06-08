///
//  Generated code. Do not modify.
//  source: file.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

// ignore_for_file: UNDEFINED_SHOWN_NAME,UNUSED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class FileFormat extends $pb.ProtobufEnum {
  static const FileFormat FFMT_UNKNOWN = FileFormat._(0, 'FFMT_UNKNOWN');
  static const FileFormat FFMT_IMAGE_JPEG = FileFormat._(1, 'FFMT_IMAGE_JPEG');
  static const FileFormat FFMT_IMAGE_PNG = FileFormat._(2, 'FFMT_IMAGE_PNG');
  static const FileFormat FFMT_VIDEO_MPEG4 = FileFormat._(3, 'FFMT_VIDEO_MPEG4');
  static const FileFormat FFMT_VIDEO_GIF = FileFormat._(4, 'FFMT_VIDEO_GIF');
  static const FileFormat FFMT_VIDEO_OGV = FileFormat._(5, 'FFMT_VIDEO_OGV');
  static const FileFormat FFMT_AUDIO_MP3 = FileFormat._(6, 'FFMT_AUDIO_MP3');
  static const FileFormat FFMT_AUDIO_OGG = FileFormat._(7, 'FFMT_AUDIO_OGG');

  static const $core.List<FileFormat> values = <FileFormat> [
    FFMT_UNKNOWN,
    FFMT_IMAGE_JPEG,
    FFMT_IMAGE_PNG,
    FFMT_VIDEO_MPEG4,
    FFMT_VIDEO_GIF,
    FFMT_VIDEO_OGV,
    FFMT_AUDIO_MP3,
    FFMT_AUDIO_OGG,
  ];

  static final $core.Map<$core.int, FileFormat> _byValue = $pb.ProtobufEnum.initByValue(values);
  static FileFormat valueOf($core.int value) => _byValue[value];

  const FileFormat._($core.int v, $core.String n) : super(v, n);
}

class FileAccessMode extends $pb.ProtobufEnum {
  static const FileAccessMode FAM_READ = FileAccessMode._(0, 'FAM_READ');
  static const FileAccessMode FAM_WRITE = FileAccessMode._(1, 'FAM_WRITE');
  static const FileAccessMode FAM_READWRITE = FileAccessMode._(2, 'FAM_READWRITE');

  static const $core.List<FileAccessMode> values = <FileAccessMode> [
    FAM_READ,
    FAM_WRITE,
    FAM_READWRITE,
  ];

  static final $core.Map<$core.int, FileAccessMode> _byValue = $pb.ProtobufEnum.initByValue(values);
  static FileAccessMode valueOf($core.int value) => _byValue[value];

  const FileAccessMode._($core.int v, $core.String n) : super(v, n);
}

