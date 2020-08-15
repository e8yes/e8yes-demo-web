///
//  Generated code. Do not modify.
//  source: user_profile.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'nullable_primitives.pb.dart' as $15;
import 'file.pb.dart' as $10;
import 'user_relation.pb.dart' as $16;

class UserPublicProfile extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UserPublicProfile', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'userId')
    ..aOM<$15.NullableString>(2, 'alias', subBuilder: $15.NullableString.create)
    ..aOM<$15.NullableString>(3, 'biography', subBuilder: $15.NullableString.create)
    ..aOM<$10.FileTokenAccess>(4, 'avatarReadonlyAccess', subBuilder: $10.FileTokenAccess.create)
    ..aOM<$10.FileTokenAccess>(5, 'avatarPreviewReadonlyAccess', subBuilder: $10.FileTokenAccess.create)
    ..pc<$16.UserRelationRecord>(6, 'relations', $pb.PbFieldType.PM, subBuilder: $16.UserRelationRecord.create)
    ..aInt64(7, 'joinAt')
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
  $15.NullableString get alias => $_getN(1);
  @$pb.TagNumber(2)
  set alias($15.NullableString v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasAlias() => $_has(1);
  @$pb.TagNumber(2)
  void clearAlias() => clearField(2);
  @$pb.TagNumber(2)
  $15.NullableString ensureAlias() => $_ensure(1);

  @$pb.TagNumber(3)
  $15.NullableString get biography => $_getN(2);
  @$pb.TagNumber(3)
  set biography($15.NullableString v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasBiography() => $_has(2);
  @$pb.TagNumber(3)
  void clearBiography() => clearField(3);
  @$pb.TagNumber(3)
  $15.NullableString ensureBiography() => $_ensure(2);

  @$pb.TagNumber(4)
  $10.FileTokenAccess get avatarReadonlyAccess => $_getN(3);
  @$pb.TagNumber(4)
  set avatarReadonlyAccess($10.FileTokenAccess v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasAvatarReadonlyAccess() => $_has(3);
  @$pb.TagNumber(4)
  void clearAvatarReadonlyAccess() => clearField(4);
  @$pb.TagNumber(4)
  $10.FileTokenAccess ensureAvatarReadonlyAccess() => $_ensure(3);

  @$pb.TagNumber(5)
  $10.FileTokenAccess get avatarPreviewReadonlyAccess => $_getN(4);
  @$pb.TagNumber(5)
  set avatarPreviewReadonlyAccess($10.FileTokenAccess v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasAvatarPreviewReadonlyAccess() => $_has(4);
  @$pb.TagNumber(5)
  void clearAvatarPreviewReadonlyAccess() => clearField(5);
  @$pb.TagNumber(5)
  $10.FileTokenAccess ensureAvatarPreviewReadonlyAccess() => $_ensure(4);

  @$pb.TagNumber(6)
  $core.List<$16.UserRelationRecord> get relations => $_getList(5);

  @$pb.TagNumber(7)
  $fixnum.Int64 get joinAt => $_getI64(6);
  @$pb.TagNumber(7)
  set joinAt($fixnum.Int64 v) { $_setInt64(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasJoinAt() => $_has(6);
  @$pb.TagNumber(7)
  void clearJoinAt() => clearField(7);
}

