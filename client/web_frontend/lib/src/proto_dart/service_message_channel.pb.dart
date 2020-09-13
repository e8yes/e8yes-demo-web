///
//  Generated code. Do not modify.
//  source: service_message_channel.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'nullable_primitives.pb.dart' as $11;
import 'pagination.pb.dart' as $19;
import 'message_channel.pb.dart' as $21;
import 'user_profile.pb.dart' as $13;

import 'message_channel.pbenum.dart' as $21;

class CreateMessageChannelRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('CreateMessageChannelRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOB(1, 'encrypted')
    ..aOB(2, 'closeGroupChannel')
    ..aOM<$11.NullableString>(3, 'title', subBuilder: $11.NullableString.create)
    ..aOM<$11.NullableString>(4, 'description', subBuilder: $11.NullableString.create)
    ..p<$fixnum.Int64>(5, 'memberIds', $pb.PbFieldType.P6)
    ..hasRequiredFields = false
  ;

  CreateMessageChannelRequest._() : super();
  factory CreateMessageChannelRequest() => create();
  factory CreateMessageChannelRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CreateMessageChannelRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  CreateMessageChannelRequest clone() => CreateMessageChannelRequest()..mergeFromMessage(this);
  CreateMessageChannelRequest copyWith(void Function(CreateMessageChannelRequest) updates) => super.copyWith((message) => updates(message as CreateMessageChannelRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static CreateMessageChannelRequest create() => CreateMessageChannelRequest._();
  CreateMessageChannelRequest createEmptyInstance() => create();
  static $pb.PbList<CreateMessageChannelRequest> createRepeated() => $pb.PbList<CreateMessageChannelRequest>();
  @$core.pragma('dart2js:noInline')
  static CreateMessageChannelRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CreateMessageChannelRequest>(create);
  static CreateMessageChannelRequest _defaultInstance;

  @$pb.TagNumber(1)
  $core.bool get encrypted => $_getBF(0);
  @$pb.TagNumber(1)
  set encrypted($core.bool v) { $_setBool(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasEncrypted() => $_has(0);
  @$pb.TagNumber(1)
  void clearEncrypted() => clearField(1);

  @$pb.TagNumber(2)
  $core.bool get closeGroupChannel => $_getBF(1);
  @$pb.TagNumber(2)
  set closeGroupChannel($core.bool v) { $_setBool(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasCloseGroupChannel() => $_has(1);
  @$pb.TagNumber(2)
  void clearCloseGroupChannel() => clearField(2);

  @$pb.TagNumber(3)
  $11.NullableString get title => $_getN(2);
  @$pb.TagNumber(3)
  set title($11.NullableString v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasTitle() => $_has(2);
  @$pb.TagNumber(3)
  void clearTitle() => clearField(3);
  @$pb.TagNumber(3)
  $11.NullableString ensureTitle() => $_ensure(2);

  @$pb.TagNumber(4)
  $11.NullableString get description => $_getN(3);
  @$pb.TagNumber(4)
  set description($11.NullableString v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasDescription() => $_has(3);
  @$pb.TagNumber(4)
  void clearDescription() => clearField(4);
  @$pb.TagNumber(4)
  $11.NullableString ensureDescription() => $_ensure(3);

  @$pb.TagNumber(5)
  $core.List<$fixnum.Int64> get memberIds => $_getList(4);
}

class CreateMessageChannelResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('CreateMessageChannelResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'channelId')
    ..hasRequiredFields = false
  ;

  CreateMessageChannelResponse._() : super();
  factory CreateMessageChannelResponse() => create();
  factory CreateMessageChannelResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CreateMessageChannelResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  CreateMessageChannelResponse clone() => CreateMessageChannelResponse()..mergeFromMessage(this);
  CreateMessageChannelResponse copyWith(void Function(CreateMessageChannelResponse) updates) => super.copyWith((message) => updates(message as CreateMessageChannelResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static CreateMessageChannelResponse create() => CreateMessageChannelResponse._();
  CreateMessageChannelResponse createEmptyInstance() => create();
  static $pb.PbList<CreateMessageChannelResponse> createRepeated() => $pb.PbList<CreateMessageChannelResponse>();
  @$core.pragma('dart2js:noInline')
  static CreateMessageChannelResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CreateMessageChannelResponse>(create);
  static CreateMessageChannelResponse _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get channelId => $_getI64(0);
  @$pb.TagNumber(1)
  set channelId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChannelId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChannelId() => clearField(1);
}

class UpdateMessageChannelRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UpdateMessageChannelRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'channelId')
    ..aOM<$11.NullableString>(2, 'title', subBuilder: $11.NullableString.create)
    ..aOM<$11.NullableString>(3, 'description', subBuilder: $11.NullableString.create)
    ..p<$fixnum.Int64>(4, 'memberIds', $pb.PbFieldType.P6)
    ..hasRequiredFields = false
  ;

  UpdateMessageChannelRequest._() : super();
  factory UpdateMessageChannelRequest() => create();
  factory UpdateMessageChannelRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UpdateMessageChannelRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  UpdateMessageChannelRequest clone() => UpdateMessageChannelRequest()..mergeFromMessage(this);
  UpdateMessageChannelRequest copyWith(void Function(UpdateMessageChannelRequest) updates) => super.copyWith((message) => updates(message as UpdateMessageChannelRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static UpdateMessageChannelRequest create() => UpdateMessageChannelRequest._();
  UpdateMessageChannelRequest createEmptyInstance() => create();
  static $pb.PbList<UpdateMessageChannelRequest> createRepeated() => $pb.PbList<UpdateMessageChannelRequest>();
  @$core.pragma('dart2js:noInline')
  static UpdateMessageChannelRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UpdateMessageChannelRequest>(create);
  static UpdateMessageChannelRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get channelId => $_getI64(0);
  @$pb.TagNumber(1)
  set channelId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChannelId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChannelId() => clearField(1);

  @$pb.TagNumber(2)
  $11.NullableString get title => $_getN(1);
  @$pb.TagNumber(2)
  set title($11.NullableString v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasTitle() => $_has(1);
  @$pb.TagNumber(2)
  void clearTitle() => clearField(2);
  @$pb.TagNumber(2)
  $11.NullableString ensureTitle() => $_ensure(1);

  @$pb.TagNumber(3)
  $11.NullableString get description => $_getN(2);
  @$pb.TagNumber(3)
  set description($11.NullableString v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasDescription() => $_has(2);
  @$pb.TagNumber(3)
  void clearDescription() => clearField(3);
  @$pb.TagNumber(3)
  $11.NullableString ensureDescription() => $_ensure(2);

  @$pb.TagNumber(4)
  $core.List<$fixnum.Int64> get memberIds => $_getList(3);
}

class UpdateMessageChannelResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UpdateMessageChannelResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  UpdateMessageChannelResponse._() : super();
  factory UpdateMessageChannelResponse() => create();
  factory UpdateMessageChannelResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UpdateMessageChannelResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  UpdateMessageChannelResponse clone() => UpdateMessageChannelResponse()..mergeFromMessage(this);
  UpdateMessageChannelResponse copyWith(void Function(UpdateMessageChannelResponse) updates) => super.copyWith((message) => updates(message as UpdateMessageChannelResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static UpdateMessageChannelResponse create() => UpdateMessageChannelResponse._();
  UpdateMessageChannelResponse createEmptyInstance() => create();
  static $pb.PbList<UpdateMessageChannelResponse> createRepeated() => $pb.PbList<UpdateMessageChannelResponse>();
  @$core.pragma('dart2js:noInline')
  static UpdateMessageChannelResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UpdateMessageChannelResponse>(create);
  static UpdateMessageChannelResponse _defaultInstance;
}

class AddUserToMessageChannelRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('AddUserToMessageChannelRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'channelId')
    ..aInt64(2, 'userId')
    ..e<$21.MessageChannelMemberType>(3, 'memberType', $pb.PbFieldType.OE, defaultOrMaker: $21.MessageChannelMemberType.MCMT_INVALID, valueOf: $21.MessageChannelMemberType.valueOf, enumValues: $21.MessageChannelMemberType.values)
    ..hasRequiredFields = false
  ;

  AddUserToMessageChannelRequest._() : super();
  factory AddUserToMessageChannelRequest() => create();
  factory AddUserToMessageChannelRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AddUserToMessageChannelRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  AddUserToMessageChannelRequest clone() => AddUserToMessageChannelRequest()..mergeFromMessage(this);
  AddUserToMessageChannelRequest copyWith(void Function(AddUserToMessageChannelRequest) updates) => super.copyWith((message) => updates(message as AddUserToMessageChannelRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static AddUserToMessageChannelRequest create() => AddUserToMessageChannelRequest._();
  AddUserToMessageChannelRequest createEmptyInstance() => create();
  static $pb.PbList<AddUserToMessageChannelRequest> createRepeated() => $pb.PbList<AddUserToMessageChannelRequest>();
  @$core.pragma('dart2js:noInline')
  static AddUserToMessageChannelRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AddUserToMessageChannelRequest>(create);
  static AddUserToMessageChannelRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get channelId => $_getI64(0);
  @$pb.TagNumber(1)
  set channelId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChannelId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChannelId() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get userId => $_getI64(1);
  @$pb.TagNumber(2)
  set userId($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasUserId() => $_has(1);
  @$pb.TagNumber(2)
  void clearUserId() => clearField(2);

  @$pb.TagNumber(3)
  $21.MessageChannelMemberType get memberType => $_getN(2);
  @$pb.TagNumber(3)
  set memberType($21.MessageChannelMemberType v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasMemberType() => $_has(2);
  @$pb.TagNumber(3)
  void clearMemberType() => clearField(3);
}

class AddUserToMessageChannelResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('AddUserToMessageChannelResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  AddUserToMessageChannelResponse._() : super();
  factory AddUserToMessageChannelResponse() => create();
  factory AddUserToMessageChannelResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AddUserToMessageChannelResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  AddUserToMessageChannelResponse clone() => AddUserToMessageChannelResponse()..mergeFromMessage(this);
  AddUserToMessageChannelResponse copyWith(void Function(AddUserToMessageChannelResponse) updates) => super.copyWith((message) => updates(message as AddUserToMessageChannelResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static AddUserToMessageChannelResponse create() => AddUserToMessageChannelResponse._();
  AddUserToMessageChannelResponse createEmptyInstance() => create();
  static $pb.PbList<AddUserToMessageChannelResponse> createRepeated() => $pb.PbList<AddUserToMessageChannelResponse>();
  @$core.pragma('dart2js:noInline')
  static AddUserToMessageChannelResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AddUserToMessageChannelResponse>(create);
  static AddUserToMessageChannelResponse _defaultInstance;
}

class GetMessageChannelKeyRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetMessageChannelKeyRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'channelId')
    ..a<$core.int>(2, 'timeoutSecs', $pb.PbFieldType.O3)
    ..hasRequiredFields = false
  ;

  GetMessageChannelKeyRequest._() : super();
  factory GetMessageChannelKeyRequest() => create();
  factory GetMessageChannelKeyRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetMessageChannelKeyRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetMessageChannelKeyRequest clone() => GetMessageChannelKeyRequest()..mergeFromMessage(this);
  GetMessageChannelKeyRequest copyWith(void Function(GetMessageChannelKeyRequest) updates) => super.copyWith((message) => updates(message as GetMessageChannelKeyRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetMessageChannelKeyRequest create() => GetMessageChannelKeyRequest._();
  GetMessageChannelKeyRequest createEmptyInstance() => create();
  static $pb.PbList<GetMessageChannelKeyRequest> createRepeated() => $pb.PbList<GetMessageChannelKeyRequest>();
  @$core.pragma('dart2js:noInline')
  static GetMessageChannelKeyRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetMessageChannelKeyRequest>(create);
  static GetMessageChannelKeyRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get channelId => $_getI64(0);
  @$pb.TagNumber(1)
  set channelId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChannelId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChannelId() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get timeoutSecs => $_getIZ(1);
  @$pb.TagNumber(2)
  set timeoutSecs($core.int v) { $_setSignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTimeoutSecs() => $_has(1);
  @$pb.TagNumber(2)
  void clearTimeoutSecs() => clearField(2);
}

class GetMessageChannelKeyResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetMessageChannelKeyResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOS(3, 'channelKey')
    ..hasRequiredFields = false
  ;

  GetMessageChannelKeyResponse._() : super();
  factory GetMessageChannelKeyResponse() => create();
  factory GetMessageChannelKeyResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetMessageChannelKeyResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetMessageChannelKeyResponse clone() => GetMessageChannelKeyResponse()..mergeFromMessage(this);
  GetMessageChannelKeyResponse copyWith(void Function(GetMessageChannelKeyResponse) updates) => super.copyWith((message) => updates(message as GetMessageChannelKeyResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetMessageChannelKeyResponse create() => GetMessageChannelKeyResponse._();
  GetMessageChannelKeyResponse createEmptyInstance() => create();
  static $pb.PbList<GetMessageChannelKeyResponse> createRepeated() => $pb.PbList<GetMessageChannelKeyResponse>();
  @$core.pragma('dart2js:noInline')
  static GetMessageChannelKeyResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetMessageChannelKeyResponse>(create);
  static GetMessageChannelKeyResponse _defaultInstance;

  @$pb.TagNumber(3)
  $core.String get channelKey => $_getSZ(0);
  @$pb.TagNumber(3)
  set channelKey($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(3)
  $core.bool hasChannelKey() => $_has(0);
  @$pb.TagNumber(3)
  void clearChannelKey() => clearField(3);
}

class LeaveMessageChannelRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('LeaveMessageChannelRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'channelId')
    ..hasRequiredFields = false
  ;

  LeaveMessageChannelRequest._() : super();
  factory LeaveMessageChannelRequest() => create();
  factory LeaveMessageChannelRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LeaveMessageChannelRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  LeaveMessageChannelRequest clone() => LeaveMessageChannelRequest()..mergeFromMessage(this);
  LeaveMessageChannelRequest copyWith(void Function(LeaveMessageChannelRequest) updates) => super.copyWith((message) => updates(message as LeaveMessageChannelRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static LeaveMessageChannelRequest create() => LeaveMessageChannelRequest._();
  LeaveMessageChannelRequest createEmptyInstance() => create();
  static $pb.PbList<LeaveMessageChannelRequest> createRepeated() => $pb.PbList<LeaveMessageChannelRequest>();
  @$core.pragma('dart2js:noInline')
  static LeaveMessageChannelRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LeaveMessageChannelRequest>(create);
  static LeaveMessageChannelRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get channelId => $_getI64(0);
  @$pb.TagNumber(1)
  set channelId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChannelId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChannelId() => clearField(1);
}

class LeaveMessageChannelResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('LeaveMessageChannelResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  LeaveMessageChannelResponse._() : super();
  factory LeaveMessageChannelResponse() => create();
  factory LeaveMessageChannelResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LeaveMessageChannelResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  LeaveMessageChannelResponse clone() => LeaveMessageChannelResponse()..mergeFromMessage(this);
  LeaveMessageChannelResponse copyWith(void Function(LeaveMessageChannelResponse) updates) => super.copyWith((message) => updates(message as LeaveMessageChannelResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static LeaveMessageChannelResponse create() => LeaveMessageChannelResponse._();
  LeaveMessageChannelResponse createEmptyInstance() => create();
  static $pb.PbList<LeaveMessageChannelResponse> createRepeated() => $pb.PbList<LeaveMessageChannelResponse>();
  @$core.pragma('dart2js:noInline')
  static LeaveMessageChannelResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LeaveMessageChannelResponse>(create);
  static LeaveMessageChannelResponse _defaultInstance;
}

class GetJoinedInMessageChannelsRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetJoinedInMessageChannelsRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$19.Pagination>(1, 'pagination', subBuilder: $19.Pagination.create)
    ..p<$fixnum.Int64>(2, 'withMemberIds', $pb.PbFieldType.P6)
    ..a<$core.int>(3, 'activeMemberFetchLimit', $pb.PbFieldType.O3)
    ..p<$fixnum.Int64>(4, 'channelIds', $pb.PbFieldType.P6)
    ..hasRequiredFields = false
  ;

  GetJoinedInMessageChannelsRequest._() : super();
  factory GetJoinedInMessageChannelsRequest() => create();
  factory GetJoinedInMessageChannelsRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetJoinedInMessageChannelsRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetJoinedInMessageChannelsRequest clone() => GetJoinedInMessageChannelsRequest()..mergeFromMessage(this);
  GetJoinedInMessageChannelsRequest copyWith(void Function(GetJoinedInMessageChannelsRequest) updates) => super.copyWith((message) => updates(message as GetJoinedInMessageChannelsRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetJoinedInMessageChannelsRequest create() => GetJoinedInMessageChannelsRequest._();
  GetJoinedInMessageChannelsRequest createEmptyInstance() => create();
  static $pb.PbList<GetJoinedInMessageChannelsRequest> createRepeated() => $pb.PbList<GetJoinedInMessageChannelsRequest>();
  @$core.pragma('dart2js:noInline')
  static GetJoinedInMessageChannelsRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetJoinedInMessageChannelsRequest>(create);
  static GetJoinedInMessageChannelsRequest _defaultInstance;

  @$pb.TagNumber(1)
  $19.Pagination get pagination => $_getN(0);
  @$pb.TagNumber(1)
  set pagination($19.Pagination v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasPagination() => $_has(0);
  @$pb.TagNumber(1)
  void clearPagination() => clearField(1);
  @$pb.TagNumber(1)
  $19.Pagination ensurePagination() => $_ensure(0);

  @$pb.TagNumber(2)
  $core.List<$fixnum.Int64> get withMemberIds => $_getList(1);

  @$pb.TagNumber(3)
  $core.int get activeMemberFetchLimit => $_getIZ(2);
  @$pb.TagNumber(3)
  set activeMemberFetchLimit($core.int v) { $_setSignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasActiveMemberFetchLimit() => $_has(2);
  @$pb.TagNumber(3)
  void clearActiveMemberFetchLimit() => clearField(3);

  @$pb.TagNumber(4)
  $core.List<$fixnum.Int64> get channelIds => $_getList(3);
}

class GetJoinedInMessageChannelsResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetJoinedInMessageChannelsResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$21.MessageChannelOveriew>(1, 'channels', $pb.PbFieldType.PM, subBuilder: $21.MessageChannelOveriew.create)
    ..hasRequiredFields = false
  ;

  GetJoinedInMessageChannelsResponse._() : super();
  factory GetJoinedInMessageChannelsResponse() => create();
  factory GetJoinedInMessageChannelsResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetJoinedInMessageChannelsResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetJoinedInMessageChannelsResponse clone() => GetJoinedInMessageChannelsResponse()..mergeFromMessage(this);
  GetJoinedInMessageChannelsResponse copyWith(void Function(GetJoinedInMessageChannelsResponse) updates) => super.copyWith((message) => updates(message as GetJoinedInMessageChannelsResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetJoinedInMessageChannelsResponse create() => GetJoinedInMessageChannelsResponse._();
  GetJoinedInMessageChannelsResponse createEmptyInstance() => create();
  static $pb.PbList<GetJoinedInMessageChannelsResponse> createRepeated() => $pb.PbList<GetJoinedInMessageChannelsResponse>();
  @$core.pragma('dart2js:noInline')
  static GetJoinedInMessageChannelsResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetJoinedInMessageChannelsResponse>(create);
  static GetJoinedInMessageChannelsResponse _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$21.MessageChannelOveriew> get channels => $_getList(0);
}

class GetMessageChannelMembersRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetMessageChannelMembersRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'channelId')
    ..aOM<$19.Pagination>(2, 'pagination', subBuilder: $19.Pagination.create)
    ..hasRequiredFields = false
  ;

  GetMessageChannelMembersRequest._() : super();
  factory GetMessageChannelMembersRequest() => create();
  factory GetMessageChannelMembersRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetMessageChannelMembersRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetMessageChannelMembersRequest clone() => GetMessageChannelMembersRequest()..mergeFromMessage(this);
  GetMessageChannelMembersRequest copyWith(void Function(GetMessageChannelMembersRequest) updates) => super.copyWith((message) => updates(message as GetMessageChannelMembersRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetMessageChannelMembersRequest create() => GetMessageChannelMembersRequest._();
  GetMessageChannelMembersRequest createEmptyInstance() => create();
  static $pb.PbList<GetMessageChannelMembersRequest> createRepeated() => $pb.PbList<GetMessageChannelMembersRequest>();
  @$core.pragma('dart2js:noInline')
  static GetMessageChannelMembersRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetMessageChannelMembersRequest>(create);
  static GetMessageChannelMembersRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get channelId => $_getI64(0);
  @$pb.TagNumber(1)
  set channelId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChannelId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChannelId() => clearField(1);

  @$pb.TagNumber(2)
  $19.Pagination get pagination => $_getN(1);
  @$pb.TagNumber(2)
  set pagination($19.Pagination v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasPagination() => $_has(1);
  @$pb.TagNumber(2)
  void clearPagination() => clearField(2);
  @$pb.TagNumber(2)
  $19.Pagination ensurePagination() => $_ensure(1);
}

class GetMessageChannelMembersResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetMessageChannelMembersResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$13.UserPublicProfile>(1, 'userProfiles', $pb.PbFieldType.PM, subBuilder: $13.UserPublicProfile.create)
    ..pc<$21.MessageChannelRelation>(2, 'channelRelations', $pb.PbFieldType.PM, subBuilder: $21.MessageChannelRelation.create)
    ..hasRequiredFields = false
  ;

  GetMessageChannelMembersResponse._() : super();
  factory GetMessageChannelMembersResponse() => create();
  factory GetMessageChannelMembersResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetMessageChannelMembersResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetMessageChannelMembersResponse clone() => GetMessageChannelMembersResponse()..mergeFromMessage(this);
  GetMessageChannelMembersResponse copyWith(void Function(GetMessageChannelMembersResponse) updates) => super.copyWith((message) => updates(message as GetMessageChannelMembersResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetMessageChannelMembersResponse create() => GetMessageChannelMembersResponse._();
  GetMessageChannelMembersResponse createEmptyInstance() => create();
  static $pb.PbList<GetMessageChannelMembersResponse> createRepeated() => $pb.PbList<GetMessageChannelMembersResponse>();
  @$core.pragma('dart2js:noInline')
  static GetMessageChannelMembersResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetMessageChannelMembersResponse>(create);
  static GetMessageChannelMembersResponse _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$13.UserPublicProfile> get userProfiles => $_getList(0);

  @$pb.TagNumber(2)
  $core.List<$21.MessageChannelRelation> get channelRelations => $_getList(1);
}

