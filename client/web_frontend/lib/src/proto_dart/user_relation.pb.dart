///
//  Generated code. Do not modify.
//  source: user_relation.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'user_relation.pbenum.dart';

export 'user_relation.pbenum.dart';

class UserRelationRecord extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UserRelationRecord', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..e<UserRelation>(1, 'relation', $pb.PbFieldType.OE, defaultOrMaker: UserRelation.URL_UNRELATED, valueOf: UserRelation.valueOf, enumValues: UserRelation.values)
    ..aInt64(2, 'createdAt')
    ..hasRequiredFields = false
  ;

  UserRelationRecord._() : super();
  factory UserRelationRecord() => create();
  factory UserRelationRecord.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UserRelationRecord.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  UserRelationRecord clone() => UserRelationRecord()..mergeFromMessage(this);
  UserRelationRecord copyWith(void Function(UserRelationRecord) updates) => super.copyWith((message) => updates(message as UserRelationRecord));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static UserRelationRecord create() => UserRelationRecord._();
  UserRelationRecord createEmptyInstance() => create();
  static $pb.PbList<UserRelationRecord> createRepeated() => $pb.PbList<UserRelationRecord>();
  @$core.pragma('dart2js:noInline')
  static UserRelationRecord getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UserRelationRecord>(create);
  static UserRelationRecord _defaultInstance;

  @$pb.TagNumber(1)
  UserRelation get relation => $_getN(0);
  @$pb.TagNumber(1)
  set relation(UserRelation v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasRelation() => $_has(0);
  @$pb.TagNumber(1)
  void clearRelation() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get createdAt => $_getI64(1);
  @$pb.TagNumber(2)
  set createdAt($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasCreatedAt() => $_has(1);
  @$pb.TagNumber(2)
  void clearCreatedAt() => clearField(2);
}

