///
//  Generated code. Do not modify.
//  source: real_time_message.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'chat_message.pb.dart' as $11;

class InvitationReceivedMessage extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('InvitationReceivedMessage', package: const $pb.PackageName('e8'), createEmptyInstance: create)
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
}

class InvitationAcceptedMessage extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('InvitationAcceptedMessage', package: const $pb.PackageName('e8'), createEmptyInstance: create)
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
}

class UnreadChatMessage extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UnreadChatMessage', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$11.ChatMessageThread>(1, 'messageThreads', $pb.PbFieldType.PM, subBuilder: $11.ChatMessageThread.create)
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
  $core.List<$11.ChatMessageThread> get messageThreads => $_getList(0);
}

enum RealTimeMessagenContent_NotificationContent {
  invitationReceived, 
  invitationAccepted, 
  unreadChat, 
  notSet
}

class RealTimeMessagenContent extends $pb.GeneratedMessage {
  static const $core.Map<$core.int, RealTimeMessagenContent_NotificationContent> _RealTimeMessagenContent_NotificationContentByTag = {
    1 : RealTimeMessagenContent_NotificationContent.invitationReceived,
    2 : RealTimeMessagenContent_NotificationContent.invitationAccepted,
    3 : RealTimeMessagenContent_NotificationContent.unreadChat,
    0 : RealTimeMessagenContent_NotificationContent.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('RealTimeMessagenContent', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3])
    ..aOM<InvitationReceivedMessage>(1, 'invitationReceived', subBuilder: InvitationReceivedMessage.create)
    ..aOM<InvitationAcceptedMessage>(2, 'invitationAccepted', subBuilder: InvitationAcceptedMessage.create)
    ..aOM<UnreadChatMessage>(3, 'unreadChat', subBuilder: UnreadChatMessage.create)
    ..hasRequiredFields = false
  ;

  RealTimeMessagenContent._() : super();
  factory RealTimeMessagenContent() => create();
  factory RealTimeMessagenContent.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RealTimeMessagenContent.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  RealTimeMessagenContent clone() => RealTimeMessagenContent()..mergeFromMessage(this);
  RealTimeMessagenContent copyWith(void Function(RealTimeMessagenContent) updates) => super.copyWith((message) => updates(message as RealTimeMessagenContent));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static RealTimeMessagenContent create() => RealTimeMessagenContent._();
  RealTimeMessagenContent createEmptyInstance() => create();
  static $pb.PbList<RealTimeMessagenContent> createRepeated() => $pb.PbList<RealTimeMessagenContent>();
  @$core.pragma('dart2js:noInline')
  static RealTimeMessagenContent getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RealTimeMessagenContent>(create);
  static RealTimeMessagenContent _defaultInstance;

  RealTimeMessagenContent_NotificationContent whichNotificationContent() => _RealTimeMessagenContent_NotificationContentByTag[$_whichOneof(0)];
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
    ..aInt64(2, 'createdAt')
    ..aInt64(3, 'targetUserId')
    ..aOM<RealTimeMessagenContent>(4, 'content', subBuilder: RealTimeMessagenContent.create)
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
  $fixnum.Int64 get createdAt => $_getI64(1);
  @$pb.TagNumber(2)
  set createdAt($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasCreatedAt() => $_has(1);
  @$pb.TagNumber(2)
  void clearCreatedAt() => clearField(2);

  @$pb.TagNumber(3)
  $fixnum.Int64 get targetUserId => $_getI64(2);
  @$pb.TagNumber(3)
  set targetUserId($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTargetUserId() => $_has(2);
  @$pb.TagNumber(3)
  void clearTargetUserId() => clearField(3);

  @$pb.TagNumber(4)
  RealTimeMessagenContent get content => $_getN(3);
  @$pb.TagNumber(4)
  set content(RealTimeMessagenContent v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasContent() => $_has(3);
  @$pb.TagNumber(4)
  void clearContent() => clearField(4);
  @$pb.TagNumber(4)
  RealTimeMessagenContent ensureContent() => $_ensure(3);
}

