///
//  Generated code. Do not modify.
//  source: real_time_message.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'user_profile.pb.dart' as $13;
import 'chat_message.pb.dart' as $14;

class InvitationReceivedMessage extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('InvitationReceivedMessage', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$13.UserPublicProfile>(1, 'inviter', subBuilder: $13.UserPublicProfile.create)
    ..hasRequiredFields = false
  ;

  InvitationReceivedMessage._() : super();
  factory InvitationReceivedMessage() => create();
  factory InvitationReceivedMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory InvitationReceivedMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  InvitationReceivedMessage clone() => InvitationReceivedMessage()..mergeFromMessage(this);
  InvitationReceivedMessage copyWith(void Function(InvitationReceivedMessage) updates) => super.copyWith((message) => updates(message as InvitationReceivedMessage));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static InvitationReceivedMessage create() => InvitationReceivedMessage._();
  InvitationReceivedMessage createEmptyInstance() => create();
  static $pb.PbList<InvitationReceivedMessage> createRepeated() => $pb.PbList<InvitationReceivedMessage>();
  @$core.pragma('dart2js:noInline')
  static InvitationReceivedMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<InvitationReceivedMessage>(create);
  static InvitationReceivedMessage _defaultInstance;

  @$pb.TagNumber(1)
  $13.UserPublicProfile get inviter => $_getN(0);
  @$pb.TagNumber(1)
  set inviter($13.UserPublicProfile v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasInviter() => $_has(0);
  @$pb.TagNumber(1)
  void clearInviter() => clearField(1);
  @$pb.TagNumber(1)
  $13.UserPublicProfile ensureInviter() => $_ensure(0);
}

class InvitationAcceptedMessage extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('InvitationAcceptedMessage', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$13.UserPublicProfile>(1, 'invitee', subBuilder: $13.UserPublicProfile.create)
    ..hasRequiredFields = false
  ;

  InvitationAcceptedMessage._() : super();
  factory InvitationAcceptedMessage() => create();
  factory InvitationAcceptedMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory InvitationAcceptedMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  InvitationAcceptedMessage clone() => InvitationAcceptedMessage()..mergeFromMessage(this);
  InvitationAcceptedMessage copyWith(void Function(InvitationAcceptedMessage) updates) => super.copyWith((message) => updates(message as InvitationAcceptedMessage));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static InvitationAcceptedMessage create() => InvitationAcceptedMessage._();
  InvitationAcceptedMessage createEmptyInstance() => create();
  static $pb.PbList<InvitationAcceptedMessage> createRepeated() => $pb.PbList<InvitationAcceptedMessage>();
  @$core.pragma('dart2js:noInline')
  static InvitationAcceptedMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<InvitationAcceptedMessage>(create);
  static InvitationAcceptedMessage _defaultInstance;

  @$pb.TagNumber(1)
  $13.UserPublicProfile get invitee => $_getN(0);
  @$pb.TagNumber(1)
  set invitee($13.UserPublicProfile v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasInvitee() => $_has(0);
  @$pb.TagNumber(1)
  void clearInvitee() => clearField(1);
  @$pb.TagNumber(1)
  $13.UserPublicProfile ensureInvitee() => $_ensure(0);
}

class UnreadChatMessage extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UnreadChatMessage', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$14.ChatMessageThread>(1, 'messageThreads', $pb.PbFieldType.PM, subBuilder: $14.ChatMessageThread.create)
    ..hasRequiredFields = false
  ;

  UnreadChatMessage._() : super();
  factory UnreadChatMessage() => create();
  factory UnreadChatMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UnreadChatMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  UnreadChatMessage clone() => UnreadChatMessage()..mergeFromMessage(this);
  UnreadChatMessage copyWith(void Function(UnreadChatMessage) updates) => super.copyWith((message) => updates(message as UnreadChatMessage));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static UnreadChatMessage create() => UnreadChatMessage._();
  UnreadChatMessage createEmptyInstance() => create();
  static $pb.PbList<UnreadChatMessage> createRepeated() => $pb.PbList<UnreadChatMessage>();
  @$core.pragma('dart2js:noInline')
  static UnreadChatMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UnreadChatMessage>(create);
  static UnreadChatMessage _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$14.ChatMessageThread> get messageThreads => $_getList(0);
}

enum RealTimeMessageContent_NotificationContent {
  invitationReceived, 
  invitationAccepted, 
  unreadChat, 
  notSet
}

class RealTimeMessageContent extends $pb.GeneratedMessage {
  static const $core.Map<$core.int, RealTimeMessageContent_NotificationContent> _RealTimeMessageContent_NotificationContentByTag = {
    1 : RealTimeMessageContent_NotificationContent.invitationReceived,
    2 : RealTimeMessageContent_NotificationContent.invitationAccepted,
    3 : RealTimeMessageContent_NotificationContent.unreadChat,
    0 : RealTimeMessageContent_NotificationContent.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('RealTimeMessageContent', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3])
    ..aOM<InvitationReceivedMessage>(1, 'invitationReceived', subBuilder: InvitationReceivedMessage.create)
    ..aOM<InvitationAcceptedMessage>(2, 'invitationAccepted', subBuilder: InvitationAcceptedMessage.create)
    ..aOM<UnreadChatMessage>(3, 'unreadChat', subBuilder: UnreadChatMessage.create)
    ..hasRequiredFields = false
  ;

  RealTimeMessageContent._() : super();
  factory RealTimeMessageContent() => create();
  factory RealTimeMessageContent.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RealTimeMessageContent.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  RealTimeMessageContent clone() => RealTimeMessageContent()..mergeFromMessage(this);
  RealTimeMessageContent copyWith(void Function(RealTimeMessageContent) updates) => super.copyWith((message) => updates(message as RealTimeMessageContent));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static RealTimeMessageContent create() => RealTimeMessageContent._();
  RealTimeMessageContent createEmptyInstance() => create();
  static $pb.PbList<RealTimeMessageContent> createRepeated() => $pb.PbList<RealTimeMessageContent>();
  @$core.pragma('dart2js:noInline')
  static RealTimeMessageContent getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RealTimeMessageContent>(create);
  static RealTimeMessageContent _defaultInstance;

  RealTimeMessageContent_NotificationContent whichNotificationContent() => _RealTimeMessageContent_NotificationContentByTag[$_whichOneof(0)];
  void clearNotificationContent() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  InvitationReceivedMessage get invitationReceived => $_getN(0);
  @$pb.TagNumber(1)
  set invitationReceived(InvitationReceivedMessage v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasInvitationReceived() => $_has(0);
  @$pb.TagNumber(1)
  void clearInvitationReceived() => clearField(1);
  @$pb.TagNumber(1)
  InvitationReceivedMessage ensureInvitationReceived() => $_ensure(0);

  @$pb.TagNumber(2)
  InvitationAcceptedMessage get invitationAccepted => $_getN(1);
  @$pb.TagNumber(2)
  set invitationAccepted(InvitationAcceptedMessage v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasInvitationAccepted() => $_has(1);
  @$pb.TagNumber(2)
  void clearInvitationAccepted() => clearField(2);
  @$pb.TagNumber(2)
  InvitationAcceptedMessage ensureInvitationAccepted() => $_ensure(1);

  @$pb.TagNumber(3)
  UnreadChatMessage get unreadChat => $_getN(2);
  @$pb.TagNumber(3)
  set unreadChat(UnreadChatMessage v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasUnreadChat() => $_has(2);
  @$pb.TagNumber(3)
  void clearUnreadChat() => clearField(3);
  @$pb.TagNumber(3)
  UnreadChatMessage ensureUnreadChat() => $_ensure(2);
}

class RealTimeMessage extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('RealTimeMessage', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'realTimeMessageId')
    ..aInt64(2, 'targetUserId')
    ..aOM<RealTimeMessageContent>(3, 'content', subBuilder: RealTimeMessageContent.create)
    ..aInt64(4, 'createdAt')
    ..aOB(5, 'popUp')
    ..hasRequiredFields = false
  ;

  RealTimeMessage._() : super();
  factory RealTimeMessage() => create();
  factory RealTimeMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RealTimeMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  RealTimeMessage clone() => RealTimeMessage()..mergeFromMessage(this);
  RealTimeMessage copyWith(void Function(RealTimeMessage) updates) => super.copyWith((message) => updates(message as RealTimeMessage));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static RealTimeMessage create() => RealTimeMessage._();
  RealTimeMessage createEmptyInstance() => create();
  static $pb.PbList<RealTimeMessage> createRepeated() => $pb.PbList<RealTimeMessage>();
  @$core.pragma('dart2js:noInline')
  static RealTimeMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RealTimeMessage>(create);
  static RealTimeMessage _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get realTimeMessageId => $_getI64(0);
  @$pb.TagNumber(1)
  set realTimeMessageId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasRealTimeMessageId() => $_has(0);
  @$pb.TagNumber(1)
  void clearRealTimeMessageId() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get targetUserId => $_getI64(1);
  @$pb.TagNumber(2)
  set targetUserId($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTargetUserId() => $_has(1);
  @$pb.TagNumber(2)
  void clearTargetUserId() => clearField(2);

  @$pb.TagNumber(3)
  RealTimeMessageContent get content => $_getN(2);
  @$pb.TagNumber(3)
  set content(RealTimeMessageContent v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasContent() => $_has(2);
  @$pb.TagNumber(3)
  void clearContent() => clearField(3);
  @$pb.TagNumber(3)
  RealTimeMessageContent ensureContent() => $_ensure(2);

  @$pb.TagNumber(4)
  $fixnum.Int64 get createdAt => $_getI64(3);
  @$pb.TagNumber(4)
  set createdAt($fixnum.Int64 v) { $_setInt64(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasCreatedAt() => $_has(3);
  @$pb.TagNumber(4)
  void clearCreatedAt() => clearField(4);

  @$pb.TagNumber(5)
  $core.bool get popUp => $_getBF(4);
  @$pb.TagNumber(5)
  set popUp($core.bool v) { $_setBool(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasPopUp() => $_has(4);
  @$pb.TagNumber(5)
  void clearPopUp() => clearField(5);
}

