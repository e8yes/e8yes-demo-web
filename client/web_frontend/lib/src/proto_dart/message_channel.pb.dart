///
//  Generated code. Do not modify.
//  source: message_channel.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'file.pb.dart' as $8;

class MessageChannel extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('MessageChannel', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'channelId')
    ..aInt64(2, 'adminUserId')
    ..aOS(3, 'title')
    ..aOM<$8.FileTokenAccess>(4, 'avatarReadonlyAccess', subBuilder: $8.FileTokenAccess.create)
    ..aOM<$8.FileTokenAccess>(5, 'avatarPreviewReadonlyAccess', subBuilder: $8.FileTokenAccess.create)
    ..aInt64(6, 'createdAt')
    ..aInt64(7, 'joinedAt')
    ..hasRequiredFields = false
  ;

  MessageChannel._() : super();
  factory MessageChannel() => create();
  factory MessageChannel.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MessageChannel.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  MessageChannel clone() => MessageChannel()..mergeFromMessage(this);
  MessageChannel copyWith(void Function(MessageChannel) updates) => super.copyWith((message) => updates(message as MessageChannel));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static MessageChannel create() => MessageChannel._();
  MessageChannel createEmptyInstance() => create();
  static $pb.PbList<MessageChannel> createRepeated() => $pb.PbList<MessageChannel>();
  @$core.pragma('dart2js:noInline')
  static MessageChannel getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MessageChannel>(create);
  static MessageChannel _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get channelId => $_getI64(0);
  @$pb.TagNumber(1)
  set channelId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChannelId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChannelId() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get adminUserId => $_getI64(1);
  @$pb.TagNumber(2)
  set adminUserId($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAdminUserId() => $_has(1);
  @$pb.TagNumber(2)
  void clearAdminUserId() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get title => $_getSZ(2);
  @$pb.TagNumber(3)
  set title($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTitle() => $_has(2);
  @$pb.TagNumber(3)
  void clearTitle() => clearField(3);

  @$pb.TagNumber(4)
  $8.FileTokenAccess get avatarReadonlyAccess => $_getN(3);
  @$pb.TagNumber(4)
  set avatarReadonlyAccess($8.FileTokenAccess v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasAvatarReadonlyAccess() => $_has(3);
  @$pb.TagNumber(4)
  void clearAvatarReadonlyAccess() => clearField(4);
  @$pb.TagNumber(4)
  $8.FileTokenAccess ensureAvatarReadonlyAccess() => $_ensure(3);

  @$pb.TagNumber(5)
  $8.FileTokenAccess get avatarPreviewReadonlyAccess => $_getN(4);
  @$pb.TagNumber(5)
  set avatarPreviewReadonlyAccess($8.FileTokenAccess v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasAvatarPreviewReadonlyAccess() => $_has(4);
  @$pb.TagNumber(5)
  void clearAvatarPreviewReadonlyAccess() => clearField(5);
  @$pb.TagNumber(5)
  $8.FileTokenAccess ensureAvatarPreviewReadonlyAccess() => $_ensure(4);

  @$pb.TagNumber(6)
  $fixnum.Int64 get createdAt => $_getI64(5);
  @$pb.TagNumber(6)
  set createdAt($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasCreatedAt() => $_has(5);
  @$pb.TagNumber(6)
  void clearCreatedAt() => clearField(6);

  @$pb.TagNumber(7)
  $fixnum.Int64 get joinedAt => $_getI64(6);
  @$pb.TagNumber(7)
  set joinedAt($fixnum.Int64 v) { $_setInt64(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasJoinedAt() => $_has(6);
  @$pb.TagNumber(7)
  void clearJoinedAt() => clearField(7);
}

