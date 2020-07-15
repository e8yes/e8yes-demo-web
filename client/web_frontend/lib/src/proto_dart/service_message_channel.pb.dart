///
//  Generated code. Do not modify.
//  source: service_message_channel.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'pagination.pb.dart' as $12;
import 'message_channel.pb.dart' as $14;
import 'user_profile.pb.dart' as $11;

class CreateMessageChannelRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('CreateMessageChannelRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOB(1, 'encrypted')
    ..aOS(2, 'title')
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
  $core.String get title => $_getSZ(1);
  @$pb.TagNumber(2)
  set title($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTitle() => $_has(1);
  @$pb.TagNumber(2)
  void clearTitle() => clearField(2);
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

class AddMemberToMessageChannelRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('AddMemberToMessageChannelRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'channelId')
    ..aInt64(2, 'userId')
    ..hasRequiredFields = false
  ;

  AddMemberToMessageChannelRequest._() : super();
  factory AddMemberToMessageChannelRequest() => create();
  factory AddMemberToMessageChannelRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AddMemberToMessageChannelRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  AddMemberToMessageChannelRequest clone() => AddMemberToMessageChannelRequest()..mergeFromMessage(this);
  AddMemberToMessageChannelRequest copyWith(void Function(AddMemberToMessageChannelRequest) updates) => super.copyWith((message) => updates(message as AddMemberToMessageChannelRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static AddMemberToMessageChannelRequest create() => AddMemberToMessageChannelRequest._();
  AddMemberToMessageChannelRequest createEmptyInstance() => create();
  static $pb.PbList<AddMemberToMessageChannelRequest> createRepeated() => $pb.PbList<AddMemberToMessageChannelRequest>();
  @$core.pragma('dart2js:noInline')
  static AddMemberToMessageChannelRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AddMemberToMessageChannelRequest>(create);
  static AddMemberToMessageChannelRequest _defaultInstance;

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
}

class AddMemberToMessageChannelResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('AddMemberToMessageChannelResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  AddMemberToMessageChannelResponse._() : super();
  factory AddMemberToMessageChannelResponse() => create();
  factory AddMemberToMessageChannelResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AddMemberToMessageChannelResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  AddMemberToMessageChannelResponse clone() => AddMemberToMessageChannelResponse()..mergeFromMessage(this);
  AddMemberToMessageChannelResponse copyWith(void Function(AddMemberToMessageChannelResponse) updates) => super.copyWith((message) => updates(message as AddMemberToMessageChannelResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static AddMemberToMessageChannelResponse create() => AddMemberToMessageChannelResponse._();
  AddMemberToMessageChannelResponse createEmptyInstance() => create();
  static $pb.PbList<AddMemberToMessageChannelResponse> createRepeated() => $pb.PbList<AddMemberToMessageChannelResponse>();
  @$core.pragma('dart2js:noInline')
  static AddMemberToMessageChannelResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AddMemberToMessageChannelResponse>(create);
  static AddMemberToMessageChannelResponse _defaultInstance;
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

class GetJoinedMessageChannelsRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetJoinedMessageChannelsRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$12.Pagination>(1, 'pagination', subBuilder: $12.Pagination.create)
    ..hasRequiredFields = false
  ;

  GetJoinedMessageChannelsRequest._() : super();
  factory GetJoinedMessageChannelsRequest() => create();
  factory GetJoinedMessageChannelsRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetJoinedMessageChannelsRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetJoinedMessageChannelsRequest clone() => GetJoinedMessageChannelsRequest()..mergeFromMessage(this);
  GetJoinedMessageChannelsRequest copyWith(void Function(GetJoinedMessageChannelsRequest) updates) => super.copyWith((message) => updates(message as GetJoinedMessageChannelsRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetJoinedMessageChannelsRequest create() => GetJoinedMessageChannelsRequest._();
  GetJoinedMessageChannelsRequest createEmptyInstance() => create();
  static $pb.PbList<GetJoinedMessageChannelsRequest> createRepeated() => $pb.PbList<GetJoinedMessageChannelsRequest>();
  @$core.pragma('dart2js:noInline')
  static GetJoinedMessageChannelsRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetJoinedMessageChannelsRequest>(create);
  static GetJoinedMessageChannelsRequest _defaultInstance;

  @$pb.TagNumber(1)
  $12.Pagination get pagination => $_getN(0);
  @$pb.TagNumber(1)
  set pagination($12.Pagination v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasPagination() => $_has(0);
  @$pb.TagNumber(1)
  void clearPagination() => clearField(1);
  @$pb.TagNumber(1)
  $12.Pagination ensurePagination() => $_ensure(0);
}

class GetJoinedMessageChannelsResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetJoinedMessageChannelsResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$14.MessageChannel>(1, 'channels', $pb.PbFieldType.PM, subBuilder: $14.MessageChannel.create)
    ..hasRequiredFields = false
  ;

  GetJoinedMessageChannelsResponse._() : super();
  factory GetJoinedMessageChannelsResponse() => create();
  factory GetJoinedMessageChannelsResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetJoinedMessageChannelsResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetJoinedMessageChannelsResponse clone() => GetJoinedMessageChannelsResponse()..mergeFromMessage(this);
  GetJoinedMessageChannelsResponse copyWith(void Function(GetJoinedMessageChannelsResponse) updates) => super.copyWith((message) => updates(message as GetJoinedMessageChannelsResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetJoinedMessageChannelsResponse create() => GetJoinedMessageChannelsResponse._();
  GetJoinedMessageChannelsResponse createEmptyInstance() => create();
  static $pb.PbList<GetJoinedMessageChannelsResponse> createRepeated() => $pb.PbList<GetJoinedMessageChannelsResponse>();
  @$core.pragma('dart2js:noInline')
  static GetJoinedMessageChannelsResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetJoinedMessageChannelsResponse>(create);
  static GetJoinedMessageChannelsResponse _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$14.MessageChannel> get channels => $_getList(0);
}

class GetMessageChannelMembersRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetMessageChannelMembersRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'channelId')
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
}

class GetMessageChannelMembersResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetMessageChannelMembersResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$11.UserPublicProfile>(1, 'userProfiles', $pb.PbFieldType.PM, subBuilder: $11.UserPublicProfile.create)
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
  $core.List<$11.UserPublicProfile> get userProfiles => $_getList(0);
}

