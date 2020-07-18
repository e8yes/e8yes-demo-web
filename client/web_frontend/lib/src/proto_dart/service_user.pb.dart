///
//  Generated code. Do not modify.
//  source: service_user.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'identity.pb.dart' as $10;
import 'user_profile.pb.dart' as $11;
import 'nullable_primitives.pb.dart' as $7;
import 'pagination.pb.dart' as $12;
import 'file.pb.dart' as $8;

import 'service_user.pbenum.dart';
import 'file.pbenum.dart' as $8;

export 'service_user.pbenum.dart';

class RegistrationRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('RegistrationRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, 'securityKey', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  RegistrationRequest._() : super();
  factory RegistrationRequest() => create();
  factory RegistrationRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RegistrationRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  RegistrationRequest clone() => RegistrationRequest()..mergeFromMessage(this);
  RegistrationRequest copyWith(void Function(RegistrationRequest) updates) => super.copyWith((message) => updates(message as RegistrationRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static RegistrationRequest create() => RegistrationRequest._();
  RegistrationRequest createEmptyInstance() => create();
  static $pb.PbList<RegistrationRequest> createRepeated() => $pb.PbList<RegistrationRequest>();
  @$core.pragma('dart2js:noInline')
  static RegistrationRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RegistrationRequest>(create);
  static RegistrationRequest _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get securityKey => $_getN(0);
  @$pb.TagNumber(1)
  set securityKey($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSecurityKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearSecurityKey() => clearField(1);
}

class RegistrationResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('RegistrationResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..e<RegistrationResponse_RegistrationErrorType>(1, 'errorType', $pb.PbFieldType.OE, defaultOrMaker: RegistrationResponse_RegistrationErrorType.RET_NoError, valueOf: RegistrationResponse_RegistrationErrorType.valueOf, enumValues: RegistrationResponse_RegistrationErrorType.values)
    ..aInt64(3, 'userId')
    ..hasRequiredFields = false
  ;

  RegistrationResponse._() : super();
  factory RegistrationResponse() => create();
  factory RegistrationResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RegistrationResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  RegistrationResponse clone() => RegistrationResponse()..mergeFromMessage(this);
  RegistrationResponse copyWith(void Function(RegistrationResponse) updates) => super.copyWith((message) => updates(message as RegistrationResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static RegistrationResponse create() => RegistrationResponse._();
  RegistrationResponse createEmptyInstance() => create();
  static $pb.PbList<RegistrationResponse> createRepeated() => $pb.PbList<RegistrationResponse>();
  @$core.pragma('dart2js:noInline')
  static RegistrationResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RegistrationResponse>(create);
  static RegistrationResponse _defaultInstance;

  @$pb.TagNumber(1)
  RegistrationResponse_RegistrationErrorType get errorType => $_getN(0);
  @$pb.TagNumber(1)
  set errorType(RegistrationResponse_RegistrationErrorType v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasErrorType() => $_has(0);
  @$pb.TagNumber(1)
  void clearErrorType() => clearField(1);

  @$pb.TagNumber(3)
  $fixnum.Int64 get userId => $_getI64(1);
  @$pb.TagNumber(3)
  set userId($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(3)
  $core.bool hasUserId() => $_has(1);
  @$pb.TagNumber(3)
  void clearUserId() => clearField(3);
}

class AuthorizationRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('AuthorizationRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'userId')
    ..a<$core.List<$core.int>>(2, 'securityKey', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  AuthorizationRequest._() : super();
  factory AuthorizationRequest() => create();
  factory AuthorizationRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AuthorizationRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  AuthorizationRequest clone() => AuthorizationRequest()..mergeFromMessage(this);
  AuthorizationRequest copyWith(void Function(AuthorizationRequest) updates) => super.copyWith((message) => updates(message as AuthorizationRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static AuthorizationRequest create() => AuthorizationRequest._();
  AuthorizationRequest createEmptyInstance() => create();
  static $pb.PbList<AuthorizationRequest> createRepeated() => $pb.PbList<AuthorizationRequest>();
  @$core.pragma('dart2js:noInline')
  static AuthorizationRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AuthorizationRequest>(create);
  static AuthorizationRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get userId => $_getI64(0);
  @$pb.TagNumber(1)
  set userId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearUserId() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get securityKey => $_getN(1);
  @$pb.TagNumber(2)
  set securityKey($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSecurityKey() => $_has(1);
  @$pb.TagNumber(2)
  void clearSecurityKey() => clearField(2);
}

class AuthorizationResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('AuthorizationResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$10.IdentitySignature>(1, 'signedIdentity', subBuilder: $10.IdentitySignature.create)
    ..hasRequiredFields = false
  ;

  AuthorizationResponse._() : super();
  factory AuthorizationResponse() => create();
  factory AuthorizationResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AuthorizationResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  AuthorizationResponse clone() => AuthorizationResponse()..mergeFromMessage(this);
  AuthorizationResponse copyWith(void Function(AuthorizationResponse) updates) => super.copyWith((message) => updates(message as AuthorizationResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static AuthorizationResponse create() => AuthorizationResponse._();
  AuthorizationResponse createEmptyInstance() => create();
  static $pb.PbList<AuthorizationResponse> createRepeated() => $pb.PbList<AuthorizationResponse>();
  @$core.pragma('dart2js:noInline')
  static AuthorizationResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AuthorizationResponse>(create);
  static AuthorizationResponse _defaultInstance;

  @$pb.TagNumber(1)
  $10.IdentitySignature get signedIdentity => $_getN(0);
  @$pb.TagNumber(1)
  set signedIdentity($10.IdentitySignature v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasSignedIdentity() => $_has(0);
  @$pb.TagNumber(1)
  void clearSignedIdentity() => clearField(1);
  @$pb.TagNumber(1)
  $10.IdentitySignature ensureSignedIdentity() => $_ensure(0);
}

class GetPublicProfileRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetPublicProfileRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'userId')
    ..hasRequiredFields = false
  ;

  GetPublicProfileRequest._() : super();
  factory GetPublicProfileRequest() => create();
  factory GetPublicProfileRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetPublicProfileRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetPublicProfileRequest clone() => GetPublicProfileRequest()..mergeFromMessage(this);
  GetPublicProfileRequest copyWith(void Function(GetPublicProfileRequest) updates) => super.copyWith((message) => updates(message as GetPublicProfileRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetPublicProfileRequest create() => GetPublicProfileRequest._();
  GetPublicProfileRequest createEmptyInstance() => create();
  static $pb.PbList<GetPublicProfileRequest> createRepeated() => $pb.PbList<GetPublicProfileRequest>();
  @$core.pragma('dart2js:noInline')
  static GetPublicProfileRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetPublicProfileRequest>(create);
  static GetPublicProfileRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get userId => $_getI64(0);
  @$pb.TagNumber(1)
  set userId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearUserId() => clearField(1);
}

class GetPublicProfileResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetPublicProfileResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$11.UserPublicProfile>(1, 'profile', subBuilder: $11.UserPublicProfile.create)
    ..hasRequiredFields = false
  ;

  GetPublicProfileResponse._() : super();
  factory GetPublicProfileResponse() => create();
  factory GetPublicProfileResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetPublicProfileResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetPublicProfileResponse clone() => GetPublicProfileResponse()..mergeFromMessage(this);
  GetPublicProfileResponse copyWith(void Function(GetPublicProfileResponse) updates) => super.copyWith((message) => updates(message as GetPublicProfileResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetPublicProfileResponse create() => GetPublicProfileResponse._();
  GetPublicProfileResponse createEmptyInstance() => create();
  static $pb.PbList<GetPublicProfileResponse> createRepeated() => $pb.PbList<GetPublicProfileResponse>();
  @$core.pragma('dart2js:noInline')
  static GetPublicProfileResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetPublicProfileResponse>(create);
  static GetPublicProfileResponse _defaultInstance;

  @$pb.TagNumber(1)
  $11.UserPublicProfile get profile => $_getN(0);
  @$pb.TagNumber(1)
  set profile($11.UserPublicProfile v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasProfile() => $_has(0);
  @$pb.TagNumber(1)
  void clearProfile() => clearField(1);
  @$pb.TagNumber(1)
  $11.UserPublicProfile ensureProfile() => $_ensure(0);
}

class UpdatePublicProfileRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UpdatePublicProfileRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$7.NullableString>(1, 'alias', subBuilder: $7.NullableString.create)
    ..aOM<$7.NullableString>(2, 'biography', subBuilder: $7.NullableString.create)
    ..hasRequiredFields = false
  ;

  UpdatePublicProfileRequest._() : super();
  factory UpdatePublicProfileRequest() => create();
  factory UpdatePublicProfileRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UpdatePublicProfileRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  UpdatePublicProfileRequest clone() => UpdatePublicProfileRequest()..mergeFromMessage(this);
  UpdatePublicProfileRequest copyWith(void Function(UpdatePublicProfileRequest) updates) => super.copyWith((message) => updates(message as UpdatePublicProfileRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static UpdatePublicProfileRequest create() => UpdatePublicProfileRequest._();
  UpdatePublicProfileRequest createEmptyInstance() => create();
  static $pb.PbList<UpdatePublicProfileRequest> createRepeated() => $pb.PbList<UpdatePublicProfileRequest>();
  @$core.pragma('dart2js:noInline')
  static UpdatePublicProfileRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UpdatePublicProfileRequest>(create);
  static UpdatePublicProfileRequest _defaultInstance;

  @$pb.TagNumber(1)
  $7.NullableString get alias => $_getN(0);
  @$pb.TagNumber(1)
  set alias($7.NullableString v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasAlias() => $_has(0);
  @$pb.TagNumber(1)
  void clearAlias() => clearField(1);
  @$pb.TagNumber(1)
  $7.NullableString ensureAlias() => $_ensure(0);

  @$pb.TagNumber(2)
  $7.NullableString get biography => $_getN(1);
  @$pb.TagNumber(2)
  set biography($7.NullableString v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasBiography() => $_has(1);
  @$pb.TagNumber(2)
  void clearBiography() => clearField(2);
  @$pb.TagNumber(2)
  $7.NullableString ensureBiography() => $_ensure(1);
}

class UpdatePublicProfileResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UpdatePublicProfileResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$11.UserPublicProfile>(1, 'profile', subBuilder: $11.UserPublicProfile.create)
    ..hasRequiredFields = false
  ;

  UpdatePublicProfileResponse._() : super();
  factory UpdatePublicProfileResponse() => create();
  factory UpdatePublicProfileResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UpdatePublicProfileResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  UpdatePublicProfileResponse clone() => UpdatePublicProfileResponse()..mergeFromMessage(this);
  UpdatePublicProfileResponse copyWith(void Function(UpdatePublicProfileResponse) updates) => super.copyWith((message) => updates(message as UpdatePublicProfileResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static UpdatePublicProfileResponse create() => UpdatePublicProfileResponse._();
  UpdatePublicProfileResponse createEmptyInstance() => create();
  static $pb.PbList<UpdatePublicProfileResponse> createRepeated() => $pb.PbList<UpdatePublicProfileResponse>();
  @$core.pragma('dart2js:noInline')
  static UpdatePublicProfileResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UpdatePublicProfileResponse>(create);
  static UpdatePublicProfileResponse _defaultInstance;

  @$pb.TagNumber(1)
  $11.UserPublicProfile get profile => $_getN(0);
  @$pb.TagNumber(1)
  set profile($11.UserPublicProfile v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasProfile() => $_has(0);
  @$pb.TagNumber(1)
  void clearProfile() => clearField(1);
  @$pb.TagNumber(1)
  $11.UserPublicProfile ensureProfile() => $_ensure(0);
}

class SearchUserRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('SearchUserRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$7.NullableString>(1, 'alias', subBuilder: $7.NullableString.create)
    ..aOM<$7.NullableInt64>(2, 'userId', subBuilder: $7.NullableInt64.create)
    ..aOM<$12.Pagination>(3, 'pagination', subBuilder: $12.Pagination.create)
    ..hasRequiredFields = false
  ;

  SearchUserRequest._() : super();
  factory SearchUserRequest() => create();
  factory SearchUserRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SearchUserRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  SearchUserRequest clone() => SearchUserRequest()..mergeFromMessage(this);
  SearchUserRequest copyWith(void Function(SearchUserRequest) updates) => super.copyWith((message) => updates(message as SearchUserRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SearchUserRequest create() => SearchUserRequest._();
  SearchUserRequest createEmptyInstance() => create();
  static $pb.PbList<SearchUserRequest> createRepeated() => $pb.PbList<SearchUserRequest>();
  @$core.pragma('dart2js:noInline')
  static SearchUserRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SearchUserRequest>(create);
  static SearchUserRequest _defaultInstance;

  @$pb.TagNumber(1)
  $7.NullableString get alias => $_getN(0);
  @$pb.TagNumber(1)
  set alias($7.NullableString v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasAlias() => $_has(0);
  @$pb.TagNumber(1)
  void clearAlias() => clearField(1);
  @$pb.TagNumber(1)
  $7.NullableString ensureAlias() => $_ensure(0);

  @$pb.TagNumber(2)
  $7.NullableInt64 get userId => $_getN(1);
  @$pb.TagNumber(2)
  set userId($7.NullableInt64 v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasUserId() => $_has(1);
  @$pb.TagNumber(2)
  void clearUserId() => clearField(2);
  @$pb.TagNumber(2)
  $7.NullableInt64 ensureUserId() => $_ensure(1);

  @$pb.TagNumber(3)
  $12.Pagination get pagination => $_getN(2);
  @$pb.TagNumber(3)
  set pagination($12.Pagination v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasPagination() => $_has(2);
  @$pb.TagNumber(3)
  void clearPagination() => clearField(3);
  @$pb.TagNumber(3)
  $12.Pagination ensurePagination() => $_ensure(2);
}

class SearchUserResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('SearchUserResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$11.UserPublicProfile>(1, 'userProfiles', $pb.PbFieldType.PM, subBuilder: $11.UserPublicProfile.create)
    ..hasRequiredFields = false
  ;

  SearchUserResponse._() : super();
  factory SearchUserResponse() => create();
  factory SearchUserResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SearchUserResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  SearchUserResponse clone() => SearchUserResponse()..mergeFromMessage(this);
  SearchUserResponse copyWith(void Function(SearchUserResponse) updates) => super.copyWith((message) => updates(message as SearchUserResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SearchUserResponse create() => SearchUserResponse._();
  SearchUserResponse createEmptyInstance() => create();
  static $pb.PbList<SearchUserResponse> createRepeated() => $pb.PbList<SearchUserResponse>();
  @$core.pragma('dart2js:noInline')
  static SearchUserResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SearchUserResponse>(create);
  static SearchUserResponse _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$11.UserPublicProfile> get userProfiles => $_getList(0);
}

class PrepareNewAvatarRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('PrepareNewAvatarRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..e<$8.FileFormat>(1, 'fileFormat', $pb.PbFieldType.OE, defaultOrMaker: $8.FileFormat.FFMT_UNKNOWN, valueOf: $8.FileFormat.valueOf, enumValues: $8.FileFormat.values)
    ..hasRequiredFields = false
  ;

  PrepareNewAvatarRequest._() : super();
  factory PrepareNewAvatarRequest() => create();
  factory PrepareNewAvatarRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PrepareNewAvatarRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  PrepareNewAvatarRequest clone() => PrepareNewAvatarRequest()..mergeFromMessage(this);
  PrepareNewAvatarRequest copyWith(void Function(PrepareNewAvatarRequest) updates) => super.copyWith((message) => updates(message as PrepareNewAvatarRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static PrepareNewAvatarRequest create() => PrepareNewAvatarRequest._();
  PrepareNewAvatarRequest createEmptyInstance() => create();
  static $pb.PbList<PrepareNewAvatarRequest> createRepeated() => $pb.PbList<PrepareNewAvatarRequest>();
  @$core.pragma('dart2js:noInline')
  static PrepareNewAvatarRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PrepareNewAvatarRequest>(create);
  static PrepareNewAvatarRequest _defaultInstance;

  @$pb.TagNumber(1)
  $8.FileFormat get fileFormat => $_getN(0);
  @$pb.TagNumber(1)
  set fileFormat($8.FileFormat v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasFileFormat() => $_has(0);
  @$pb.TagNumber(1)
  void clearFileFormat() => clearField(1);
}

class PrepareNewAvatarResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('PrepareNewAvatarResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$8.FileTokenAccess>(1, 'avatarReadwriteAccess', subBuilder: $8.FileTokenAccess.create)
    ..hasRequiredFields = false
  ;

  PrepareNewAvatarResponse._() : super();
  factory PrepareNewAvatarResponse() => create();
  factory PrepareNewAvatarResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PrepareNewAvatarResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  PrepareNewAvatarResponse clone() => PrepareNewAvatarResponse()..mergeFromMessage(this);
  PrepareNewAvatarResponse copyWith(void Function(PrepareNewAvatarResponse) updates) => super.copyWith((message) => updates(message as PrepareNewAvatarResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static PrepareNewAvatarResponse create() => PrepareNewAvatarResponse._();
  PrepareNewAvatarResponse createEmptyInstance() => create();
  static $pb.PbList<PrepareNewAvatarResponse> createRepeated() => $pb.PbList<PrepareNewAvatarResponse>();
  @$core.pragma('dart2js:noInline')
  static PrepareNewAvatarResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PrepareNewAvatarResponse>(create);
  static PrepareNewAvatarResponse _defaultInstance;

  @$pb.TagNumber(1)
  $8.FileTokenAccess get avatarReadwriteAccess => $_getN(0);
  @$pb.TagNumber(1)
  set avatarReadwriteAccess($8.FileTokenAccess v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasAvatarReadwriteAccess() => $_has(0);
  @$pb.TagNumber(1)
  void clearAvatarReadwriteAccess() => clearField(1);
  @$pb.TagNumber(1)
  $8.FileTokenAccess ensureAvatarReadwriteAccess() => $_ensure(0);
}

