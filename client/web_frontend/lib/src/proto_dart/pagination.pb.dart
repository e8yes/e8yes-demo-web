///
//  Generated code. Do not modify.
//  source: pagination.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

class Pagination extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('Pagination', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..a<$core.int>(3, 'pageNumber', $pb.PbFieldType.O3)
    ..a<$core.int>(4, 'resultPerPage', $pb.PbFieldType.O3)
    ..hasRequiredFields = false
  ;

  Pagination._() : super();
  factory Pagination() => create();
  factory Pagination.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Pagination.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  Pagination clone() => Pagination()..mergeFromMessage(this);
  Pagination copyWith(void Function(Pagination) updates) => super.copyWith((message) => updates(message as Pagination));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Pagination create() => Pagination._();
  Pagination createEmptyInstance() => create();
  static $pb.PbList<Pagination> createRepeated() => $pb.PbList<Pagination>();
  @$core.pragma('dart2js:noInline')
  static Pagination getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Pagination>(create);
  static Pagination _defaultInstance;

  @$pb.TagNumber(3)
  $core.int get pageNumber => $_getIZ(0);
  @$pb.TagNumber(3)
  set pageNumber($core.int v) { $_setSignedInt32(0, v); }
  @$pb.TagNumber(3)
  $core.bool hasPageNumber() => $_has(0);
  @$pb.TagNumber(3)
  void clearPageNumber() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get resultPerPage => $_getIZ(1);
  @$pb.TagNumber(4)
  set resultPerPage($core.int v) { $_setSignedInt32(1, v); }
  @$pb.TagNumber(4)
  $core.bool hasResultPerPage() => $_has(1);
  @$pb.TagNumber(4)
  void clearResultPerPage() => clearField(4);
}

