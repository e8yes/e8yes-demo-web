///
//  Generated code. Do not modify.
//  source: service_user.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

const RegistrationRequest$json = const {
  '1': 'RegistrationRequest',
  '2': const [
    const {'1': 'security_key', '3': 1, '4': 1, '5': 12, '10': 'securityKey'},
  ],
};

const RegistrationResponse$json = const {
  '1': 'RegistrationResponse',
  '2': const [
    const {'1': 'error_type', '3': 1, '4': 1, '5': 14, '6': '.e8.RegistrationResponse.RegistrationErrorType', '10': 'errorType'},
    const {'1': 'user_id', '3': 3, '4': 1, '5': 3, '10': 'userId'},
  ],
  '4': const [RegistrationResponse_RegistrationErrorType$json],
};

const RegistrationResponse_RegistrationErrorType$json = const {
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
    const {'1': 'user_id', '3': 1, '4': 1, '5': 3, '10': 'userId'},
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

