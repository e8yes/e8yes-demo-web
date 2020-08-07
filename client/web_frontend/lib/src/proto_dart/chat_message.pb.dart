///
//  Generated code. Do not modify.
//  source: chat_message.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'file.pb.dart' as $10;

class ChatMessageEntry extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('ChatMessageEntry', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'threadId')
    ..aInt64(2, 'messageId')
    ..aInt64(3, 'senderId')
    ..pPS(4, 'messages')
    ..pc<$10.FileTokenAccess>(5, 'mediaFileAccesses', $pb.PbFieldType.PM, subBuilder: $10.FileTokenAccess.create)
    ..aInt64(6, 'createdAt')
    ..hasRequiredFields = false
  ;

  ChatMessageEntry._() : super();
  factory ChatMessageEntry() => create();
  factory ChatMessageEntry.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ChatMessageEntry.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  ChatMessageEntry clone() => ChatMessageEntry()..mergeFromMessage(this);
  ChatMessageEntry copyWith(void Function(ChatMessageEntry) updates) => super.copyWith((message) => updates(message as ChatMessageEntry));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ChatMessageEntry create() => ChatMessageEntry._();
  ChatMessageEntry createEmptyInstance() => create();
  static $pb.PbList<ChatMessageEntry> createRepeated() => $pb.PbList<ChatMessageEntry>();
  @$core.pragma('dart2js:noInline')
  static ChatMessageEntry getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ChatMessageEntry>(create);
  static ChatMessageEntry _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get threadId => $_getI64(0);
  @$pb.TagNumber(1)
  set threadId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasThreadId() => $_has(0);
  @$pb.TagNumber(1)
  void clearThreadId() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get messageId => $_getI64(1);
  @$pb.TagNumber(2)
  set messageId($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasMessageId() => $_has(1);
  @$pb.TagNumber(2)
  void clearMessageId() => clearField(2);

  @$pb.TagNumber(3)
  $fixnum.Int64 get senderId => $_getI64(2);
  @$pb.TagNumber(3)
  set senderId($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSenderId() => $_has(2);
  @$pb.TagNumber(3)
  void clearSenderId() => clearField(3);

  @$pb.TagNumber(4)
  $core.List<$core.String> get messages => $_getList(3);

  @$pb.TagNumber(5)
  $core.List<$10.FileTokenAccess> get mediaFileAccesses => $_getList(4);

  @$pb.TagNumber(6)
  $fixnum.Int64 get createdAt => $_getI64(5);
  @$pb.TagNumber(6)
  set createdAt($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasCreatedAt() => $_has(5);
  @$pb.TagNumber(6)
  void clearCreatedAt() => clearField(6);
}

class ChatMessageThread extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('ChatMessageThread', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'threadId')
    ..aOS(2, 'threadTitle')
    ..aInt64(3, 'createdAt')
    ..pc<ChatMessageEntry>(4, 'messages', $pb.PbFieldType.PM, subBuilder: ChatMessageEntry.create)
    ..hasRequiredFields = false
  ;

  ChatMessageThread._() : super();
  factory ChatMessageThread() => create();
  factory ChatMessageThread.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ChatMessageThread.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  ChatMessageThread clone() => ChatMessageThread()..mergeFromMessage(this);
  ChatMessageThread copyWith(void Function(ChatMessageThread) updates) => super.copyWith((message) => updates(message as ChatMessageThread));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ChatMessageThread create() => ChatMessageThread._();
  ChatMessageThread createEmptyInstance() => create();
  static $pb.PbList<ChatMessageThread> createRepeated() => $pb.PbList<ChatMessageThread>();
  @$core.pragma('dart2js:noInline')
  static ChatMessageThread getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ChatMessageThread>(create);
  static ChatMessageThread _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get threadId => $_getI64(0);
  @$pb.TagNumber(1)
  set threadId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasThreadId() => $_has(0);
  @$pb.TagNumber(1)
  void clearThreadId() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get threadTitle => $_getSZ(1);
  @$pb.TagNumber(2)
  set threadTitle($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasThreadTitle() => $_has(1);
  @$pb.TagNumber(2)
  void clearThreadTitle() => clearField(2);

  @$pb.TagNumber(3)
  $fixnum.Int64 get createdAt => $_getI64(2);
  @$pb.TagNumber(3)
  set createdAt($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasCreatedAt() => $_has(2);
  @$pb.TagNumber(3)
  void clearCreatedAt() => clearField(3);

  @$pb.TagNumber(4)
  $core.List<ChatMessageEntry> get messages => $_getList(3);
}

