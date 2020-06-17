///
//  Generated code. Do not modify.
//  source: service_socialnetwork.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'pagination.pb.dart' as $10;
import 'user_profile.pb.dart' as $9;

import 'user_relation.pbenum.dart' as $7;

class GetUserRelationsRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetUserRelationsRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'targetUserId')
    ..hasRequiredFields = false
  ;

  GetUserRelationsRequest._() : super();
  factory GetUserRelationsRequest() => create();
  factory GetUserRelationsRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetUserRelationsRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetUserRelationsRequest clone() => GetUserRelationsRequest()..mergeFromMessage(this);
  GetUserRelationsRequest copyWith(void Function(GetUserRelationsRequest) updates) => super.copyWith((message) => updates(message as GetUserRelationsRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetUserRelationsRequest create() => GetUserRelationsRequest._();
  GetUserRelationsRequest createEmptyInstance() => create();
  static $pb.PbList<GetUserRelationsRequest> createRepeated() => $pb.PbList<GetUserRelationsRequest>();
  @$core.pragma('dart2js:noInline')
  static GetUserRelationsRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetUserRelationsRequest>(create);
  static GetUserRelationsRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get targetUserId => $_getI64(0);
  @$pb.TagNumber(1)
  set targetUserId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTargetUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearTargetUserId() => clearField(1);
}

class GetUserRelationsResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetUserRelationsResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'sourceUserId')
    ..aInt64(2, 'targetUserId')
    ..pc<$7.UserRelation>(3, 'userRelation', $pb.PbFieldType.PE, valueOf: $7.UserRelation.valueOf, enumValues: $7.UserRelation.values)
    ..hasRequiredFields = false
  ;

  GetUserRelationsResponse._() : super();
  factory GetUserRelationsResponse() => create();
  factory GetUserRelationsResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetUserRelationsResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetUserRelationsResponse clone() => GetUserRelationsResponse()..mergeFromMessage(this);
  GetUserRelationsResponse copyWith(void Function(GetUserRelationsResponse) updates) => super.copyWith((message) => updates(message as GetUserRelationsResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetUserRelationsResponse create() => GetUserRelationsResponse._();
  GetUserRelationsResponse createEmptyInstance() => create();
  static $pb.PbList<GetUserRelationsResponse> createRepeated() => $pb.PbList<GetUserRelationsResponse>();
  @$core.pragma('dart2js:noInline')
  static GetUserRelationsResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetUserRelationsResponse>(create);
  static GetUserRelationsResponse _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get sourceUserId => $_getI64(0);
  @$pb.TagNumber(1)
  set sourceUserId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSourceUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearSourceUserId() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get targetUserId => $_getI64(1);
  @$pb.TagNumber(2)
  set targetUserId($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTargetUserId() => $_has(1);
  @$pb.TagNumber(2)
  void clearTargetUserId() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<$7.UserRelation> get userRelation => $_getList(2);
}

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
    ..aOM<$10.Pagination>(1, 'pagination', subBuilder: $10.Pagination.create)
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
  $10.Pagination get pagination => $_getN(0);
  @$pb.TagNumber(1)
  set pagination($10.Pagination v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasPagination() => $_has(0);
  @$pb.TagNumber(1)
  void clearPagination() => clearField(1);
  @$pb.TagNumber(1)
  $10.Pagination ensurePagination() => $_ensure(0);
}

class GetInvitationListResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetInvitationListResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$9.UserPublicProfile>(1, 'userProfiles', $pb.PbFieldType.PM, subBuilder: $9.UserPublicProfile.create)
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
  $core.List<$9.UserPublicProfile> get userProfiles => $_getList(0);
}

class DeleteContactRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DeleteContactRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'deletedContactUserId')
    ..hasRequiredFields = false
  ;

  DeleteContactRequest._() : super();
  factory DeleteContactRequest() => create();
  factory DeleteContactRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeleteContactRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DeleteContactRequest clone() => DeleteContactRequest()..mergeFromMessage(this);
  DeleteContactRequest copyWith(void Function(DeleteContactRequest) updates) => super.copyWith((message) => updates(message as DeleteContactRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DeleteContactRequest create() => DeleteContactRequest._();
  DeleteContactRequest createEmptyInstance() => create();
  static $pb.PbList<DeleteContactRequest> createRepeated() => $pb.PbList<DeleteContactRequest>();
  @$core.pragma('dart2js:noInline')
  static DeleteContactRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeleteContactRequest>(create);
  static DeleteContactRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get deletedContactUserId => $_getI64(0);
  @$pb.TagNumber(1)
  set deletedContactUserId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDeletedContactUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearDeletedContactUserId() => clearField(1);
}

class DeleteContactResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DeleteContactResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  DeleteContactResponse._() : super();
  factory DeleteContactResponse() => create();
  factory DeleteContactResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeleteContactResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DeleteContactResponse clone() => DeleteContactResponse()..mergeFromMessage(this);
  DeleteContactResponse copyWith(void Function(DeleteContactResponse) updates) => super.copyWith((message) => updates(message as DeleteContactResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DeleteContactResponse create() => DeleteContactResponse._();
  DeleteContactResponse createEmptyInstance() => create();
  static $pb.PbList<DeleteContactResponse> createRepeated() => $pb.PbList<DeleteContactResponse>();
  @$core.pragma('dart2js:noInline')
  static DeleteContactResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeleteContactResponse>(create);
  static DeleteContactResponse _defaultInstance;
}

class GetContactListRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetContactListRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$10.Pagination>(1, 'pagination', subBuilder: $10.Pagination.create)
    ..hasRequiredFields = false
  ;

  GetContactListRequest._() : super();
  factory GetContactListRequest() => create();
  factory GetContactListRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetContactListRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetContactListRequest clone() => GetContactListRequest()..mergeFromMessage(this);
  GetContactListRequest copyWith(void Function(GetContactListRequest) updates) => super.copyWith((message) => updates(message as GetContactListRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetContactListRequest create() => GetContactListRequest._();
  GetContactListRequest createEmptyInstance() => create();
  static $pb.PbList<GetContactListRequest> createRepeated() => $pb.PbList<GetContactListRequest>();
  @$core.pragma('dart2js:noInline')
  static GetContactListRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetContactListRequest>(create);
  static GetContactListRequest _defaultInstance;

  @$pb.TagNumber(1)
  $10.Pagination get pagination => $_getN(0);
  @$pb.TagNumber(1)
  set pagination($10.Pagination v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasPagination() => $_has(0);
  @$pb.TagNumber(1)
  void clearPagination() => clearField(1);
  @$pb.TagNumber(1)
  $10.Pagination ensurePagination() => $_ensure(0);
}

class GetContactListResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetContactListResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$9.UserPublicProfile>(1, 'userProfiles', $pb.PbFieldType.PM, subBuilder: $9.UserPublicProfile.create)
    ..hasRequiredFields = false
  ;

  GetContactListResponse._() : super();
  factory GetContactListResponse() => create();
  factory GetContactListResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetContactListResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetContactListResponse clone() => GetContactListResponse()..mergeFromMessage(this);
  GetContactListResponse copyWith(void Function(GetContactListResponse) updates) => super.copyWith((message) => updates(message as GetContactListResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetContactListResponse create() => GetContactListResponse._();
  GetContactListResponse createEmptyInstance() => create();
  static $pb.PbList<GetContactListResponse> createRepeated() => $pb.PbList<GetContactListResponse>();
  @$core.pragma('dart2js:noInline')
  static GetContactListResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetContactListResponse>(create);
  static GetContactListResponse _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$9.UserPublicProfile> get userProfiles => $_getList(0);
}

