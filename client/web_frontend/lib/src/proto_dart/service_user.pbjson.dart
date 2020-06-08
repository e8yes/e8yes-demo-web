///
//  Generated code. Do not modify.
//  source: service_user.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'identity.pbjson.dart' as $2;
import 'nullable_primitives.pbjson.dart' as $3;
import 'user_profile.pbjson.dart' as $4;
import 'file.pbjson.dart' as $1;
import 'pagination.pbjson.dart' as $5;

const RegistrationRequest$json = const {
  '1': 'RegistrationRequest',
  '2': const [
    const {'1': 'security_key', '3': 1, '4': 1, '5': 12, '10': 'securityKey'},
  ],
};

const RegistrationReponse$json = const {
  '1': 'RegistrationReponse',
  '2': const [
    const {'1': 'error_type', '3': 1, '4': 1, '5': 14, '6': '.e8.RegistrationReponse.RegistrationErrorType', '10': 'errorType'},
    const {'1': 'user_id', '3': 3, '4': 1, '5': 3, '10': 'userId'},
  ],
  '4': const [RegistrationReponse_RegistrationErrorType$json],
};

const RegistrationReponse_RegistrationErrorType$json = const {
  '1': 'RegistrationErrorType',
  '2': const [
    const {'1': 'RET_NoError', '2': 0},
    const {'1': 'RET_BadSecurityKey', '2': 1},
    const {'1': 'RET_BotDetected', '2': 2},
  ],
};

const AuthorizationRequest$json = const {
  '1': 'AuthorizationRequest',
  '2': const [
    const {'1': 'user_id', '3': 1, '4': 1, '5': 3, '10': 'userId'},
    const {'1': 'security_key', '3': 2, '4': 1, '5': 12, '10': 'securityKey'},
  ],
};

const AuthorizationResponse$json = const {
  '1': 'AuthorizationResponse',
  '2': const [
    const {'1': 'signed_identity', '3': 1, '4': 1, '5': 11, '6': '.e8.IdentitySignature', '10': 'signedIdentity'},
  ],
};

const GetPublicProfileRequest$json = const {
  '1': 'GetPublicProfileRequest',
  '2': const [
    const {'1': 'user_id', '3': 1, '4': 1, '5': 11, '6': '.e8.NullableInt64', '10': 'userId'},
  ],
};

const GetPublicProfileResponse$json = const {
  '1': 'GetPublicProfileResponse',
  '2': const [
    const {'1': 'profile', '3': 1, '4': 1, '5': 11, '6': '.e8.UserPublicProfile', '10': 'profile'},
  ],
};

const UpdatePublicProfileRequest$json = const {
  '1': 'UpdatePublicProfileRequest',
  '2': const [
    const {'1': 'alias', '3': 1, '4': 1, '5': 11, '6': '.e8.NullableString', '10': 'alias'},
  ],
};

const UpdatePublicProfileResponse$json = const {
  '1': 'UpdatePublicProfileResponse',
  '2': const [
    const {'1': 'profile', '3': 1, '4': 1, '5': 11, '6': '.e8.UserPublicProfile', '10': 'profile'},
  ],
};

const SearchUserRequest$json = const {
  '1': 'SearchUserRequest',
  '2': const [
    const {'1': 'alias', '3': 1, '4': 1, '5': 11, '6': '.e8.NullableString', '10': 'alias'},
    const {'1': 'user_id', '3': 2, '4': 1, '5': 11, '6': '.e8.NullableInt64', '10': 'userId'},
    const {'1': 'pagination', '3': 3, '4': 1, '5': 11, '6': '.e8.Pagination', '10': 'pagination'},
  ],
};

const SearchUserResponse$json = const {
  '1': 'SearchUserResponse',
  '2': const [
    const {'1': 'user_profiles', '3': 1, '4': 3, '5': 11, '6': '.e8.UserPublicProfile', '10': 'userProfiles'},
  ],
};

const PrepareNewAvatarRequest$json = const {
  '1': 'PrepareNewAvatarRequest',
  '2': const [
    const {'1': 'file_format', '3': 1, '4': 1, '5': 14, '6': '.e8.FileFormat', '10': 'fileFormat'},
  ],
};

const PrepareNewAvatarResponse$json = const {
  '1': 'PrepareNewAvatarResponse',
  '2': const [
    const {'1': 'avatar_readwrite_access', '3': 1, '4': 1, '5': 11, '6': '.e8.FileTokenAccess', '10': 'avatarReadwriteAccess'},
  ],
};

const UserServiceBase$json = const {
  '1': 'UserService',
  '2': const [
    const {'1': 'Register', '2': '.e8.RegistrationRequest', '3': '.e8.RegistrationReponse'},
    const {'1': 'Authorize', '2': '.e8.AuthorizationRequest', '3': '.e8.AuthorizationResponse'},
    const {'1': 'GetPublicProfile', '2': '.e8.GetPublicProfileRequest', '3': '.e8.GetPublicProfileResponse'},
    const {'1': 'UpdatePublicProfile', '2': '.e8.UpdatePublicProfileRequest', '3': '.e8.UpdatePublicProfileResponse'},
    const {'1': 'Search', '2': '.e8.SearchUserRequest', '3': '.e8.SearchUserResponse'},
    const {'1': 'PrepareNewAvatar', '2': '.e8.PrepareNewAvatarRequest', '3': '.e8.PrepareNewAvatarResponse'},
  ],
};

const UserServiceBase$messageJson = const {
  '.e8.RegistrationRequest': RegistrationRequest$json,
  '.e8.RegistrationReponse': RegistrationReponse$json,
  '.e8.AuthorizationRequest': AuthorizationRequest$json,
  '.e8.AuthorizationResponse': AuthorizationResponse$json,
  '.e8.IdentitySignature': $2.IdentitySignature$json,
  '.e8.GetPublicProfileRequest': GetPublicProfileRequest$json,
  '.e8.NullableInt64': $3.NullableInt64$json,
  '.e8.GetPublicProfileResponse': GetPublicProfileResponse$json,
  '.e8.UserPublicProfile': $4.UserPublicProfile$json,
  '.e8.FileTokenAccess': $1.FileTokenAccess$json,
  '.e8.UpdatePublicProfileRequest': UpdatePublicProfileRequest$json,
  '.e8.NullableString': $3.NullableString$json,
  '.e8.UpdatePublicProfileResponse': UpdatePublicProfileResponse$json,
  '.e8.SearchUserRequest': SearchUserRequest$json,
  '.e8.Pagination': $5.Pagination$json,
  '.e8.SearchUserResponse': SearchUserResponse$json,
  '.e8.PrepareNewAvatarRequest': PrepareNewAvatarRequest$json,
  '.e8.PrepareNewAvatarResponse': PrepareNewAvatarResponse$json,
};

