///
//  Generated code. Do not modify.
//  source: service_socialnetwork.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;
import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'pagination.pb.dart' as $5;
import 'user_profile.pb.dart' as $4;

class SendInvitationRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('SendInvitationRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'inviteeUserId')
    ..hasRequiredFields = false
  ;

  SendInvitationRequest._() : super();
  factory SendInvitationRequest() => create();
  factory SendInvitationRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SendInvitationRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  SendInvitationRequest clone() => SendInvitationRequest()..mergeFromMessage(this);
  SendInvitationRequest copyWith(void Function(SendInvitationRequest) updates) => super.copyWith((message) => updates(message as SendInvitationRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SendInvitationRequest create() => SendInvitationRequest._();
  SendInvitationRequest createEmptyInstance() => create();
  static $pb.PbList<SendInvitationRequest> createRepeated() => $pb.PbList<SendInvitationRequest>();
  @$core.pragma('dart2js:noInline')
  static SendInvitationRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SendInvitationRequest>(create);
  static SendInvitationRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get inviteeUserId => $_getI64(0);
  @$pb.TagNumber(1)
  set inviteeUserId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasInviteeUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearInviteeUserId() => clearField(1);
}

class SendInvitationResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('SendInvitationResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  SendInvitationResponse._() : super();
  factory SendInvitationResponse() => create();
  factory SendInvitationResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SendInvitationResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  SendInvitationResponse clone() => SendInvitationResponse()..mergeFromMessage(this);
  SendInvitationResponse copyWith(void Function(SendInvitationResponse) updates) => super.copyWith((message) => updates(message as SendInvitationResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SendInvitationResponse create() => SendInvitationResponse._();
  SendInvitationResponse createEmptyInstance() => create();
  static $pb.PbList<SendInvitationResponse> createRepeated() => $pb.PbList<SendInvitationResponse>();
  @$core.pragma('dart2js:noInline')
  static SendInvitationResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SendInvitationResponse>(create);
  static SendInvitationResponse _defaultInstance;
}

class DeleteInvitationRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DeleteInvitationRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'inviteeUserId')
    ..hasRequiredFields = false
  ;

  DeleteInvitationRequest._() : super();
  factory DeleteInvitationRequest() => create();
  factory DeleteInvitationRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeleteInvitationRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DeleteInvitationRequest clone() => DeleteInvitationRequest()..mergeFromMessage(this);
  DeleteInvitationRequest copyWith(void Function(DeleteInvitationRequest) updates) => super.copyWith((message) => updates(message as DeleteInvitationRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DeleteInvitationRequest create() => DeleteInvitationRequest._();
  DeleteInvitationRequest createEmptyInstance() => create();
  static $pb.PbList<DeleteInvitationRequest> createRepeated() => $pb.PbList<DeleteInvitationRequest>();
  @$core.pragma('dart2js:noInline')
  static DeleteInvitationRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeleteInvitationRequest>(create);
  static DeleteInvitationRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get inviteeUserId => $_getI64(0);
  @$pb.TagNumber(1)
  set inviteeUserId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasInviteeUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearInviteeUserId() => clearField(1);
}

class DeleteInvitationResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DeleteInvitationResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  DeleteInvitationResponse._() : super();
  factory DeleteInvitationResponse() => create();
  factory DeleteInvitationResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeleteInvitationResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DeleteInvitationResponse clone() => DeleteInvitationResponse()..mergeFromMessage(this);
  DeleteInvitationResponse copyWith(void Function(DeleteInvitationResponse) updates) => super.copyWith((message) => updates(message as DeleteInvitationResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DeleteInvitationResponse create() => DeleteInvitationResponse._();
  DeleteInvitationResponse createEmptyInstance() => create();
  static $pb.PbList<DeleteInvitationResponse> createRepeated() => $pb.PbList<DeleteInvitationResponse>();
  @$core.pragma('dart2js:noInline')
  static DeleteInvitationResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeleteInvitationResponse>(create);
  static DeleteInvitationResponse _defaultInstance;
}

class AcceptInvitationRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('AcceptInvitationRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'inviterUserId')
    ..hasRequiredFields = false
  ;

  AcceptInvitationRequest._() : super();
  factory AcceptInvitationRequest() => create();
  factory AcceptInvitationRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AcceptInvitationRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  AcceptInvitationRequest clone() => AcceptInvitationRequest()..mergeFromMessage(this);
  AcceptInvitationRequest copyWith(void Function(AcceptInvitationRequest) updates) => super.copyWith((message) => updates(message as AcceptInvitationRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static AcceptInvitationRequest create() => AcceptInvitationRequest._();
  AcceptInvitationRequest createEmptyInstance() => create();
  static $pb.PbList<AcceptInvitationRequest> createRepeated() => $pb.PbList<AcceptInvitationRequest>();
  @$core.pragma('dart2js:noInline')
  static AcceptInvitationRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AcceptInvitationRequest>(create);
  static AcceptInvitationRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get inviterUserId => $_getI64(0);
  @$pb.TagNumber(1)
  set inviterUserId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasInviterUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearInviterUserId() => clearField(1);
}

class AcceptInvitationResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('AcceptInvitationResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  AcceptInvitationResponse._() : super();
  factory AcceptInvitationResponse() => create();
  factory AcceptInvitationResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AcceptInvitationResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  AcceptInvitationResponse clone() => AcceptInvitationResponse()..mergeFromMessage(this);
  AcceptInvitationResponse copyWith(void Function(AcceptInvitationResponse) updates) => super.copyWith((message) => updates(message as AcceptInvitationResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static AcceptInvitationResponse create() => AcceptInvitationResponse._();
  AcceptInvitationResponse createEmptyInstance() => create();
  static $pb.PbList<AcceptInvitationResponse> createRepeated() => $pb.PbList<AcceptInvitationResponse>();
  @$core.pragma('dart2js:noInline')
  static AcceptInvitationResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AcceptInvitationResponse>(create);
  static AcceptInvitationResponse _defaultInstance;
}

class RejectInvitationRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('RejectInvitationRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'inviterUserId')
    ..hasRequiredFields = false
  ;

  RejectInvitationRequest._() : super();
  factory RejectInvitationRequest() => create();
  factory RejectInvitationRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RejectInvitationRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  RejectInvitationRequest clone() => RejectInvitationRequest()..mergeFromMessage(this);
  RejectInvitationRequest copyWith(void Function(RejectInvitationRequest) updates) => super.copyWith((message) => updates(message as RejectInvitationRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static RejectInvitationRequest create() => RejectInvitationRequest._();
  RejectInvitationRequest createEmptyInstance() => create();
  static $pb.PbList<RejectInvitationRequest> createRepeated() => $pb.PbList<RejectInvitationRequest>();
  @$core.pragma('dart2js:noInline')
  static RejectInvitationRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RejectInvitationRequest>(create);
  static RejectInvitationRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get inviterUserId => $_getI64(0);
  @$pb.TagNumber(1)
  set inviterUserId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasInviterUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearInviterUserId() => clearField(1);
}

class RejectInvitationResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('RejectInvitationResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  RejectInvitationResponse._() : super();
  factory RejectInvitationResponse() => create();
  factory RejectInvitationResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RejectInvitationResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  RejectInvitationResponse clone() => RejectInvitationResponse()..mergeFromMessage(this);
  RejectInvitationResponse copyWith(void Function(RejectInvitationResponse) updates) => super.copyWith((message) => updates(message as RejectInvitationResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static RejectInvitationResponse create() => RejectInvitationResponse._();
  RejectInvitationResponse createEmptyInstance() => create();
  static $pb.PbList<RejectInvitationResponse> createRepeated() => $pb.PbList<RejectInvitationResponse>();
  @$core.pragma('dart2js:noInline')
  static RejectInvitationResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RejectInvitationResponse>(create);
  static RejectInvitationResponse _defaultInstance;
}

class GetInvitationListRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetInvitationListRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$5.Pagination>(1, 'pagination', subBuilder: $5.Pagination.create)
    ..hasRequiredFields = false
  ;

  GetInvitationListRequest._() : super();
  factory GetInvitationListRequest() => create();
  factory GetInvitationListRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetInvitationListRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetInvitationListRequest clone() => GetInvitationListRequest()..mergeFromMessage(this);
  GetInvitationListRequest copyWith(void Function(GetInvitationListRequest) updates) => super.copyWith((message) => updates(message as GetInvitationListRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetInvitationListRequest create() => GetInvitationListRequest._();
  GetInvitationListRequest createEmptyInstance() => create();
  static $pb.PbList<GetInvitationListRequest> createRepeated() => $pb.PbList<GetInvitationListRequest>();
  @$core.pragma('dart2js:noInline')
  static GetInvitationListRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetInvitationListRequest>(create);
  static GetInvitationListRequest _defaultInstance;

  @$pb.TagNumber(1)
  $5.Pagination get pagination => $_getN(0);
  @$pb.TagNumber(1)
  set pagination($5.Pagination v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasPagination() => $_has(0);
  @$pb.TagNumber(1)
  void clearPagination() => clearField(1);
  @$pb.TagNumber(1)
  $5.Pagination ensurePagination() => $_ensure(0);
}

class GetInvitationListResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetInvitationListResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$4.UserPublicProfile>(1, 'userProfiles', $pb.PbFieldType.PM, subBuilder: $4.UserPublicProfile.create)
    ..hasRequiredFields = false
  ;

  GetInvitationListResponse._() : super();
  factory GetInvitationListResponse() => create();
  factory GetInvitationListResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetInvitationListResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetInvitationListResponse clone() => GetInvitationListResponse()..mergeFromMessage(this);
  GetInvitationListResponse copyWith(void Function(GetInvitationListResponse) updates) => super.copyWith((message) => updates(message as GetInvitationListResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetInvitationListResponse create() => GetInvitationListResponse._();
  GetInvitationListResponse createEmptyInstance() => create();
  static $pb.PbList<GetInvitationListResponse> createRepeated() => $pb.PbList<GetInvitationListResponse>();
  @$core.pragma('dart2js:noInline')
  static GetInvitationListResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetInvitationListResponse>(create);
  static GetInvitationListResponse _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$4.UserPublicProfile> get userProfiles => $_getList(0);
}

class DeleteFriendRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DeleteFriendRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'friendUserId')
    ..hasRequiredFields = false
  ;

  DeleteFriendRequest._() : super();
  factory DeleteFriendRequest() => create();
  factory DeleteFriendRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeleteFriendRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DeleteFriendRequest clone() => DeleteFriendRequest()..mergeFromMessage(this);
  DeleteFriendRequest copyWith(void Function(DeleteFriendRequest) updates) => super.copyWith((message) => updates(message as DeleteFriendRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DeleteFriendRequest create() => DeleteFriendRequest._();
  DeleteFriendRequest createEmptyInstance() => create();
  static $pb.PbList<DeleteFriendRequest> createRepeated() => $pb.PbList<DeleteFriendRequest>();
  @$core.pragma('dart2js:noInline')
  static DeleteFriendRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeleteFriendRequest>(create);
  static DeleteFriendRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get friendUserId => $_getI64(0);
  @$pb.TagNumber(1)
  set friendUserId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFriendUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearFriendUserId() => clearField(1);
}

class DeleteFriendResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DeleteFriendResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  DeleteFriendResponse._() : super();
  factory DeleteFriendResponse() => create();
  factory DeleteFriendResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeleteFriendResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DeleteFriendResponse clone() => DeleteFriendResponse()..mergeFromMessage(this);
  DeleteFriendResponse copyWith(void Function(DeleteFriendResponse) updates) => super.copyWith((message) => updates(message as DeleteFriendResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DeleteFriendResponse create() => DeleteFriendResponse._();
  DeleteFriendResponse createEmptyInstance() => create();
  static $pb.PbList<DeleteFriendResponse> createRepeated() => $pb.PbList<DeleteFriendResponse>();
  @$core.pragma('dart2js:noInline')
  static DeleteFriendResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeleteFriendResponse>(create);
  static DeleteFriendResponse _defaultInstance;
}

class GetFriendListRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetFriendListRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$5.Pagination>(1, 'pagination', subBuilder: $5.Pagination.create)
    ..hasRequiredFields = false
  ;

  GetFriendListRequest._() : super();
  factory GetFriendListRequest() => create();
  factory GetFriendListRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetFriendListRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetFriendListRequest clone() => GetFriendListRequest()..mergeFromMessage(this);
  GetFriendListRequest copyWith(void Function(GetFriendListRequest) updates) => super.copyWith((message) => updates(message as GetFriendListRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetFriendListRequest create() => GetFriendListRequest._();
  GetFriendListRequest createEmptyInstance() => create();
  static $pb.PbList<GetFriendListRequest> createRepeated() => $pb.PbList<GetFriendListRequest>();
  @$core.pragma('dart2js:noInline')
  static GetFriendListRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetFriendListRequest>(create);
  static GetFriendListRequest _defaultInstance;

  @$pb.TagNumber(1)
  $5.Pagination get pagination => $_getN(0);
  @$pb.TagNumber(1)
  set pagination($5.Pagination v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasPagination() => $_has(0);
  @$pb.TagNumber(1)
  void clearPagination() => clearField(1);
  @$pb.TagNumber(1)
  $5.Pagination ensurePagination() => $_ensure(0);
}

class GetFriendListResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetFriendListResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$4.UserPublicProfile>(1, 'userProfiles', $pb.PbFieldType.PM, subBuilder: $4.UserPublicProfile.create)
    ..hasRequiredFields = false
  ;

  GetFriendListResponse._() : super();
  factory GetFriendListResponse() => create();
  factory GetFriendListResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetFriendListResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetFriendListResponse clone() => GetFriendListResponse()..mergeFromMessage(this);
  GetFriendListResponse copyWith(void Function(GetFriendListResponse) updates) => super.copyWith((message) => updates(message as GetFriendListResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetFriendListResponse create() => GetFriendListResponse._();
  GetFriendListResponse createEmptyInstance() => create();
  static $pb.PbList<GetFriendListResponse> createRepeated() => $pb.PbList<GetFriendListResponse>();
  @$core.pragma('dart2js:noInline')
  static GetFriendListResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetFriendListResponse>(create);
  static GetFriendListResponse _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$4.UserPublicProfile> get userProfiles => $_getList(0);
}

class SocialNetworkServiceApi {
  $pb.RpcClient _client;
  SocialNetworkServiceApi(this._client);

  $async.Future<SendInvitationResponse> sendInvitation($pb.ClientContext ctx, SendInvitationRequest request) {
    var emptyResponse = SendInvitationResponse();
    return _client.invoke<SendInvitationResponse>(ctx, 'SocialNetworkService', 'SendInvitation', request, emptyResponse);
  }
  $async.Future<DeleteInvitationResponse> deleteInvitation($pb.ClientContext ctx, DeleteInvitationRequest request) {
    var emptyResponse = DeleteInvitationResponse();
    return _client.invoke<DeleteInvitationResponse>(ctx, 'SocialNetworkService', 'DeleteInvitation', request, emptyResponse);
  }
  $async.Future<AcceptInvitationResponse> acceptInvitation($pb.ClientContext ctx, AcceptInvitationRequest request) {
    var emptyResponse = AcceptInvitationResponse();
    return _client.invoke<AcceptInvitationResponse>(ctx, 'SocialNetworkService', 'AcceptInvitation', request, emptyResponse);
  }
  $async.Future<RejectInvitationResponse> rejectInvitation($pb.ClientContext ctx, RejectInvitationRequest request) {
    var emptyResponse = RejectInvitationResponse();
    return _client.invoke<RejectInvitationResponse>(ctx, 'SocialNetworkService', 'RejectInvitation', request, emptyResponse);
  }
  $async.Future<GetInvitationListResponse> getInvitationList($pb.ClientContext ctx, GetInvitationListRequest request) {
    var emptyResponse = GetInvitationListResponse();
    return _client.invoke<GetInvitationListResponse>(ctx, 'SocialNetworkService', 'GetInvitationList', request, emptyResponse);
  }
  $async.Future<DeleteFriendResponse> deleteFriend($pb.ClientContext ctx, DeleteFriendRequest request) {
    var emptyResponse = DeleteFriendResponse();
    return _client.invoke<DeleteFriendResponse>(ctx, 'SocialNetworkService', 'DeleteFriend', request, emptyResponse);
  }
  $async.Future<GetFriendListResponse> getFriendList($pb.ClientContext ctx, GetFriendListRequest request) {
    var emptyResponse = GetFriendListResponse();
    return _client.invoke<GetFriendListResponse>(ctx, 'SocialNetworkService', 'GetFriendList', request, emptyResponse);
  }
}

