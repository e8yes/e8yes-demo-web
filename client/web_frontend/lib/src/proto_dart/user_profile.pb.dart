///
//  Generated code. Do not modify.
//  source: user_profile.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'file.pb.dart' as $5;

class UserPublicProfile extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UserPublicProfile', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'userId')
    ..aOS(2, 'alias')
    ..aOM<$5.FileTokenAccess>(3, 'avatarReadonlyAccess', subBuilder: $5.FileTokenAccess.create)
    ..aOM<$5.FileTokenAccess>(4, 'avatarPreviewReadonlyAccess', subBuilder: $5.FileTokenAccess.create)
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
  $core.String get alias => $_getSZ(1);
  @$pb.TagNumber(2)
  set alias($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAlias() => $_has(1);
  @$pb.TagNumber(2)
  void clearAlias() => clearField(2);

  @$pb.TagNumber(3)
  $5.FileTokenAccess get avatarReadonlyAccess => $_getN(2);
  @$pb.TagNumber(3)
  set avatarReadonlyAccess($5.FileTokenAccess v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasAvatarReadonlyAccess() => $_has(2);
  @$pb.TagNumber(3)
  void clearAvatarReadonlyAccess() => clearField(3);
  @$pb.TagNumber(3)
  $5.FileTokenAccess ensureAvatarReadonlyAccess() => $_ensure(2);

  @$pb.TagNumber(4)
  $5.FileTokenAccess get avatarPreviewReadonlyAccess => $_getN(3);
  @$pb.TagNumber(4)
  set avatarPreviewReadonlyAccess($5.FileTokenAccess v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasAvatarPreviewReadonlyAccess() => $_has(3);
  @$pb.TagNumber(4)
  void clearAvatarPreviewReadonlyAccess() => clearField(4);
  @$pb.TagNumber(4)
  $5.FileTokenAccess ensureAvatarPreviewReadonlyAccess() => $_ensure(3);
}

