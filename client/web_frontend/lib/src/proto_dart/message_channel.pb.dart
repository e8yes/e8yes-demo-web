///
//  Generated code. Do not modify.
//  source: message_channel.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'file.pb.dart' as $10;
import 'user_profile.pb.dart' as $13;

import 'message_channel.pbenum.dart';

export 'message_channel.pbenum.dart';

class MessageChannelRelation extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('MessageChannelRelation', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..e<MessageChannelMemberType>(1, 'memberType', $pb.PbFieldType.OE, defaultOrMaker: MessageChannelMemberType.MCMT_INVALID, valueOf: MessageChannelMemberType.valueOf, enumValues: MessageChannelMemberType.values)
    ..aInt64(2, 'joinAt')
    ..hasRequiredFields = false
  ;

  MessageChannelRelation._() : super();
  factory MessageChannelRelation() => create();
  factory MessageChannelRelation.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MessageChannelRelation.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  MessageChannelRelation clone() => MessageChannelRelation()..mergeFromMessage(this);
  MessageChannelRelation copyWith(void Function(MessageChannelRelation) updates) => super.copyWith((message) => updates(message as MessageChannelRelation));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static MessageChannelRelation create() => MessageChannelRelation._();
  MessageChannelRelation createEmptyInstance() => create();
  static $pb.PbList<MessageChannelRelation> createRepeated() => $pb.PbList<MessageChannelRelation>();
  @$core.pragma('dart2js:noInline')
  static MessageChannelRelation getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MessageChannelRelation>(create);
  static MessageChannelRelation _defaultInstance;

  @$pb.TagNumber(1)
  MessageChannelMemberType get memberType => $_getN(0);
  @$pb.TagNumber(1)
  set memberType(MessageChannelMemberType v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasMemberType() => $_has(0);
  @$pb.TagNumber(1)
  void clearMemberType() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get joinAt => $_getI64(1);
  @$pb.TagNumber(2)
  set joinAt($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasJoinAt() => $_has(1);
  @$pb.TagNumber(2)
  void clearJoinAt() => clearField(2);
}

class MessageChannel extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('MessageChannel', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'channelId')
    ..aOS(2, 'title')
    ..aOS(3, 'description')
    ..aOM<$10.FileTokenAccess>(4, 'avatarReadonlyAccess', subBuilder: $10.FileTokenAccess.create)
    ..aOM<$10.FileTokenAccess>(5, 'avatarPreviewReadonlyAccess', subBuilder: $10.FileTokenAccess.create)
    ..aInt64(6, 'createdAt')
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
  $core.String get title => $_getSZ(1);
  @$pb.TagNumber(2)
  set title($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTitle() => $_has(1);
  @$pb.TagNumber(2)
  void clearTitle() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get description => $_getSZ(2);
  @$pb.TagNumber(3)
  set description($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasDescription() => $_has(2);
  @$pb.TagNumber(3)
  void clearDescription() => clearField(3);

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
  $fixnum.Int64 get createdAt => $_getI64(5);
  @$pb.TagNumber(6)
  set createdAt($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasCreatedAt() => $_has(5);
  @$pb.TagNumber(6)
  void clearCreatedAt() => clearField(6);
}

class MessageChannelOveriew extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('MessageChannelOveriew', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<MessageChannel>(1, 'channel', subBuilder: MessageChannel.create)
    ..aInt64(2, 'channelLastInteractedAt')
    ..aOM<MessageChannelRelation>(3, 'channelRelation', subBuilder: MessageChannelRelation.create)
    ..pc<$13.UserPublicProfile>(4, 'mostActiveUsers', $pb.PbFieldType.PM, subBuilder: $13.UserPublicProfile.create)
    ..hasRequiredFields = false
  ;

  MessageChannelOveriew._() : super();
  factory MessageChannelOveriew() => create();
  factory MessageChannelOveriew.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MessageChannelOveriew.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  MessageChannelOveriew clone() => MessageChannelOveriew()..mergeFromMessage(this);
  MessageChannelOveriew copyWith(void Function(MessageChannelOveriew) updates) => super.copyWith((message) => updates(message as MessageChannelOveriew));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static MessageChannelOveriew create() => MessageChannelOveriew._();
  MessageChannelOveriew createEmptyInstance() => create();
  static $pb.PbList<MessageChannelOveriew> createRepeated() => $pb.PbList<MessageChannelOveriew>();
  @$core.pragma('dart2js:noInline')
  static MessageChannelOveriew getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MessageChannelOveriew>(create);
  static MessageChannelOveriew _defaultInstance;

  @$pb.TagNumber(1)
  MessageChannel get channel => $_getN(0);
  @$pb.TagNumber(1)
  set channel(MessageChannel v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasChannel() => $_has(0);
  @$pb.TagNumber(1)
  void clearChannel() => clearField(1);
  @$pb.TagNumber(1)
  MessageChannel ensureChannel() => $_ensure(0);

  @$pb.TagNumber(2)
  $fixnum.Int64 get channelLastInteractedAt => $_getI64(1);
  @$pb.TagNumber(2)
  set channelLastInteractedAt($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasChannelLastInteractedAt() => $_has(1);
  @$pb.TagNumber(2)
  void clearChannelLastInteractedAt() => clearField(2);

  @$pb.TagNumber(3)
  MessageChannelRelation get channelRelation => $_getN(2);
  @$pb.TagNumber(3)
  set channelRelation(MessageChannelRelation v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasChannelRelation() => $_has(2);
  @$pb.TagNumber(3)
  void clearChannelRelation() => clearField(3);
  @$pb.TagNumber(3)
  MessageChannelRelation ensureChannelRelation() => $_ensure(2);

  @$pb.TagNumber(4)
  $core.List<$13.UserPublicProfile> get mostActiveUsers => $_getList(3);
}

