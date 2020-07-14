///
//  Generated code. Do not modify.
//  source: user_profile.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'nullable_primitives.pb.dart' as $7;
import 'file.pb.dart' as $8;
import 'user_relation.pb.dart' as $9;

class UserPublicProfile extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UserPublicProfile', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'userId')
    ..aOM<$7.NullableString>(2, 'alias', subBuilder: $7.NullableString.create)
    ..aOM<$8.FileTokenAccess>(3, 'avatarReadonlyAccess', subBuilder: $8.FileTokenAccess.create)
    ..aOM<$8.FileTokenAccess>(4, 'avatarPreviewReadonlyAccess', subBuilder: $8.FileTokenAccess.create)
    ..pc<$9.UserRelationRecord>(5, 'relations', $pb.PbFieldType.PM, subBuilder: $9.UserRelationRecord.create)
    ..aInt64(6, 'joinAt')
    ..hasRequiredFields = false
  ;

  UserPublicProfile._() : super();
  factory UserPublicProfile() => create();
  factory UserPublicProfile.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UserPublicProfile.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  UserPublicProfile clone() => UserPublicProfile()..mergeFromMessage(this);
  UserPublicProfile copyWith(void Function(UserPublicProfile) updates) => super.copyWith((message) => updates(message as UserPublicProfile));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static UserPublicProfile create() => UserPublicProfile._();
  UserPublicProfile createEmptyInstance() => create();
  static $pb.PbList<UserPublicProfile> createRepeated() => $pb.PbList<UserPublicProfile>();
  @$core.pragma('dart2js:noInline')
  static UserPublicProfile getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UserPublicProfile>(create);
  static UserPublicProfile _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get userId => $_getI64(0);
  @$pb.TagNumber(1)
  set userId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearUserId() => clearField(1);

  @$pb.TagNumber(2)
  $7.NullableString get alias => $_getN(1);
  @$pb.TagNumber(2)
  set alias($7.NullableString v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasAlias() => $_has(1);
  @$pb.TagNumber(2)
  void clearAlias() => clearField(2);
  @$pb.TagNumber(2)
  $7.NullableString ensureAlias() => $_ensure(1);

  @$pb.TagNumber(3)
  $8.FileTokenAccess get avatarReadonlyAccess => $_getN(2);
  @$pb.TagNumber(3)
  set avatarReadonlyAccess($8.FileTokenAccess v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasAvatarReadonlyAccess() => $_has(2);
  @$pb.TagNumber(3)
  void clearAvatarReadonlyAccess() => clearField(3);
  @$pb.TagNumber(3)
  $8.FileTokenAccess ensureAvatarReadonlyAccess() => $_ensure(2);

  @$pb.TagNumber(4)
  $8.FileTokenAccess get avatarPreviewReadonlyAccess => $_getN(3);
  @$pb.TagNumber(4)
  set avatarPreviewReadonlyAccess($8.FileTokenAccess v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasAvatarPreviewReadonlyAccess() => $_has(3);
  @$pb.TagNumber(4)
  void clearAvatarPreviewReadonlyAccess() => clearField(4);
  @$pb.TagNumber(4)
  $8.FileTokenAccess ensureAvatarPreviewReadonlyAccess() => $_ensure(3);

  @$pb.TagNumber(5)
  $core.List<$9.UserRelationRecord> get relations => $_getList(4);

  @$pb.TagNumber(6)
  $fixnum.Int64 get joinAt => $_getI64(5);
  @$pb.TagNumber(6)
  set joinAt($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasJoinAt() => $_has(5);
  @$pb.TagNumber(6)
  void clearJoinAt() => clearField(6);
}

