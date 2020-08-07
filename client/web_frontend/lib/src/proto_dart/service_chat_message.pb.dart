///
//  Generated code. Do not modify.
//  source: service_chat_message.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'chat_message.pb.dart' as $11;
import 'pagination.pb.dart' as $18;

class SendChatMessageRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('SendChatMessageRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'channelId')
    ..aOM<$11.ChatMessageEntry>(2, 'message', subBuilder: $11.ChatMessageEntry.create)
    ..hasRequiredFields = false
  ;

  SendChatMessageRequest._() : super();
  factory SendChatMessageRequest() => create();
  factory SendChatMessageRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SendChatMessageRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  SendChatMessageRequest clone() => SendChatMessageRequest()..mergeFromMessage(this);
  SendChatMessageRequest copyWith(void Function(SendChatMessageRequest) updates) => super.copyWith((message) => updates(message as SendChatMessageRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SendChatMessageRequest create() => SendChatMessageRequest._();
  SendChatMessageRequest createEmptyInstance() => create();
  static $pb.PbList<SendChatMessageRequest> createRepeated() => $pb.PbList<SendChatMessageRequest>();
  @$core.pragma('dart2js:noInline')
  static SendChatMessageRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SendChatMessageRequest>(create);
  static SendChatMessageRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get channelId => $_getI64(0);
  @$pb.TagNumber(1)
  set channelId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChannelId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChannelId() => clearField(1);

  @$pb.TagNumber(2)
  $11.ChatMessageEntry get message => $_getN(1);
  @$pb.TagNumber(2)
  set message($11.ChatMessageEntry v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasMessage() => $_has(1);
  @$pb.TagNumber(2)
  void clearMessage() => clearField(2);
  @$pb.TagNumber(2)
  $11.ChatMessageEntry ensureMessage() => $_ensure(1);
}

class SendChatMessageResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('SendChatMessageResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$11.ChatMessageThread>(1, 'updatedThread', subBuilder: $11.ChatMessageThread.create)
    ..hasRequiredFields = false
  ;

  SendChatMessageResponse._() : super();
  factory SendChatMessageResponse() => create();
  factory SendChatMessageResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SendChatMessageResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  SendChatMessageResponse clone() => SendChatMessageResponse()..mergeFromMessage(this);
  SendChatMessageResponse copyWith(void Function(SendChatMessageResponse) updates) => super.copyWith((message) => updates(message as SendChatMessageResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SendChatMessageResponse create() => SendChatMessageResponse._();
  SendChatMessageResponse createEmptyInstance() => create();
  static $pb.PbList<SendChatMessageResponse> createRepeated() => $pb.PbList<SendChatMessageResponse>();
  @$core.pragma('dart2js:noInline')
  static SendChatMessageResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SendChatMessageResponse>(create);
  static SendChatMessageResponse _defaultInstance;

  @$pb.TagNumber(1)
  $11.ChatMessageThread get updatedThread => $_getN(0);
  @$pb.TagNumber(1)
  set updatedThread($11.ChatMessageThread v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasUpdatedThread() => $_has(0);
  @$pb.TagNumber(1)
  void clearUpdatedThread() => clearField(1);
  @$pb.TagNumber(1)
  $11.ChatMessageThread ensureUpdatedThread() => $_ensure(0);
}

class GetChatMessageRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetChatMessageRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'channelId')
    ..aOM<$18.Pagination>(2, 'pagination', subBuilder: $18.Pagination.create)
    ..hasRequiredFields = false
  ;

  GetChatMessageRequest._() : super();
  factory GetChatMessageRequest() => create();
  factory GetChatMessageRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetChatMessageRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetChatMessageRequest clone() => GetChatMessageRequest()..mergeFromMessage(this);
  GetChatMessageRequest copyWith(void Function(GetChatMessageRequest) updates) => super.copyWith((message) => updates(message as GetChatMessageRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetChatMessageRequest create() => GetChatMessageRequest._();
  GetChatMessageRequest createEmptyInstance() => create();
  static $pb.PbList<GetChatMessageRequest> createRepeated() => $pb.PbList<GetChatMessageRequest>();
  @$core.pragma('dart2js:noInline')
  static GetChatMessageRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetChatMessageRequest>(create);
  static GetChatMessageRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get channelId => $_getI64(0);
  @$pb.TagNumber(1)
  set channelId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChannelId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChannelId() => clearField(1);

  @$pb.TagNumber(2)
  $18.Pagination get pagination => $_getN(1);
  @$pb.TagNumber(2)
  set pagination($18.Pagination v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasPagination() => $_has(1);
  @$pb.TagNumber(2)
  void clearPagination() => clearField(2);
  @$pb.TagNumber(2)
  $18.Pagination ensurePagination() => $_ensure(1);
}

class GetChatMessageResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetChatMessageResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$11.ChatMessageThread>(1, 'threads', $pb.PbFieldType.PM, subBuilder: $11.ChatMessageThread.create)
    ..hasRequiredFields = false
  ;

  GetChatMessageResponse._() : super();
  factory GetChatMessageResponse() => create();
  factory GetChatMessageResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetChatMessageResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetChatMessageResponse clone() => GetChatMessageResponse()..mergeFromMessage(this);
  GetChatMessageResponse copyWith(void Function(GetChatMessageResponse) updates) => super.copyWith((message) => updates(message as GetChatMessageResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetChatMessageResponse create() => GetChatMessageResponse._();
  GetChatMessageResponse createEmptyInstance() => create();
  static $pb.PbList<GetChatMessageResponse> createRepeated() => $pb.PbList<GetChatMessageResponse>();
  @$core.pragma('dart2js:noInline')
  static GetChatMessageResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetChatMessageResponse>(create);
  static GetChatMessageResponse _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$11.ChatMessageThread> get threads => $_getList(0);
}

