///
//  Generated code. Do not modify.
//  source: service_socialnetwork.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'user_relation.pb.dart' as $12;
import 'pagination.pb.dart' as $19;
import 'user_profile.pb.dart' as $13;

import 'user_relation.pbenum.dart' as $12;

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
    ..pc<$12.UserRelationRecord>(3, 'userRelations', $pb.PbFieldType.PM, subBuilder: $12.UserRelationRecord.create)
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
  $core.List<$12.UserRelationRecord> get userRelations => $_getList(2);
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

class ProcessInvitationRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('ProcessInvitationRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'inviterUserId')
    ..aOB(2, 'accept')
    ..hasRequiredFields = false
  ;

  ProcessInvitationRequest._() : super();
  factory ProcessInvitationRequest() => create();
  factory ProcessInvitationRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ProcessInvitationRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  ProcessInvitationRequest clone() => ProcessInvitationRequest()..mergeFromMessage(this);
  ProcessInvitationRequest copyWith(void Function(ProcessInvitationRequest) updates) => super.copyWith((message) => updates(message as ProcessInvitationRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ProcessInvitationRequest create() => ProcessInvitationRequest._();
  ProcessInvitationRequest createEmptyInstance() => create();
  static $pb.PbList<ProcessInvitationRequest> createRepeated() => $pb.PbList<ProcessInvitationRequest>();
  @$core.pragma('dart2js:noInline')
  static ProcessInvitationRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ProcessInvitationRequest>(create);
  static ProcessInvitationRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get inviterUserId => $_getI64(0);
  @$pb.TagNumber(1)
  set inviterUserId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasInviterUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearInviterUserId() => clearField(1);

  @$pb.TagNumber(2)
  $core.bool get accept => $_getBF(1);
  @$pb.TagNumber(2)
  set accept($core.bool v) { $_setBool(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAccept() => $_has(1);
  @$pb.TagNumber(2)
  void clearAccept() => clearField(2);
}

class ProcessInvitationResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('ProcessInvitationResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  ProcessInvitationResponse._() : super();
  factory ProcessInvitationResponse() => create();
  factory ProcessInvitationResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ProcessInvitationResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  ProcessInvitationResponse clone() => ProcessInvitationResponse()..mergeFromMessage(this);
  ProcessInvitationResponse copyWith(void Function(ProcessInvitationResponse) updates) => super.copyWith((message) => updates(message as ProcessInvitationResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ProcessInvitationResponse create() => ProcessInvitationResponse._();
  ProcessInvitationResponse createEmptyInstance() => create();
  static $pb.PbList<ProcessInvitationResponse> createRepeated() => $pb.PbList<ProcessInvitationResponse>();
  @$core.pragma('dart2js:noInline')
  static ProcessInvitationResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ProcessInvitationResponse>(create);
  static ProcessInvitationResponse _defaultInstance;
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
    ..aOB(1, 'deleted')
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

  @$pb.TagNumber(1)
  $core.bool get deleted => $_getBF(0);
  @$pb.TagNumber(1)
  set deleted($core.bool v) { $_setBool(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDeleted() => $_has(0);
  @$pb.TagNumber(1)
  void clearDeleted() => clearField(1);
}

class SearchRelatedUserListRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('SearchRelatedUserListRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOS(1, 'searchTerms')
    ..aOM<$19.Pagination>(2, 'pagination', subBuilder: $19.Pagination.create)
    ..pc<$12.UserRelation>(3, 'relationFilter', $pb.PbFieldType.PE, valueOf: $12.UserRelation.valueOf, enumValues: $12.UserRelation.values)
    ..hasRequiredFields = false
  ;

  SearchRelatedUserListRequest._() : super();
  factory SearchRelatedUserListRequest() => create();
  factory SearchRelatedUserListRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SearchRelatedUserListRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  SearchRelatedUserListRequest clone() => SearchRelatedUserListRequest()..mergeFromMessage(this);
  SearchRelatedUserListRequest copyWith(void Function(SearchRelatedUserListRequest) updates) => super.copyWith((message) => updates(message as SearchRelatedUserListRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SearchRelatedUserListRequest create() => SearchRelatedUserListRequest._();
  SearchRelatedUserListRequest createEmptyInstance() => create();
  static $pb.PbList<SearchRelatedUserListRequest> createRepeated() => $pb.PbList<SearchRelatedUserListRequest>();
  @$core.pragma('dart2js:noInline')
  static SearchRelatedUserListRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SearchRelatedUserListRequest>(create);
  static SearchRelatedUserListRequest _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get searchTerms => $_getSZ(0);
  @$pb.TagNumber(1)
  set searchTerms($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSearchTerms() => $_has(0);
  @$pb.TagNumber(1)
  void clearSearchTerms() => clearField(1);

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

  @$pb.TagNumber(3)
  $core.List<$12.UserRelation> get relationFilter => $_getList(2);
}

class SearchRelatedUserListResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('SearchRelatedUserListResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$13.UserPublicProfile>(1, 'userProfiles', $pb.PbFieldType.PM, subBuilder: $13.UserPublicProfile.create)
    ..hasRequiredFields = false
  ;

  SearchRelatedUserListResponse._() : super();
  factory SearchRelatedUserListResponse() => create();
  factory SearchRelatedUserListResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SearchRelatedUserListResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  SearchRelatedUserListResponse clone() => SearchRelatedUserListResponse()..mergeFromMessage(this);
  SearchRelatedUserListResponse copyWith(void Function(SearchRelatedUserListResponse) updates) => super.copyWith((message) => updates(message as SearchRelatedUserListResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SearchRelatedUserListResponse create() => SearchRelatedUserListResponse._();
  SearchRelatedUserListResponse createEmptyInstance() => create();
  static $pb.PbList<SearchRelatedUserListResponse> createRepeated() => $pb.PbList<SearchRelatedUserListResponse>();
  @$core.pragma('dart2js:noInline')
  static SearchRelatedUserListResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SearchRelatedUserListResponse>(create);
  static SearchRelatedUserListResponse _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$13.UserPublicProfile> get userProfiles => $_getList(0);
}

