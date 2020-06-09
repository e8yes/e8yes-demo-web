///
//  Generated code. Do not modify.
//  source: service_socialnetwork.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

const SendInvitationRequest$json = const {
  '1': 'SendInvitationRequest',
  '2': const [
    const {'1': 'invitee_user_id', '3': 1, '4': 1, '5': 3, '10': 'inviteeUserId'},
  ],
};

const SendInvitationResponse$json = const {
  '1': 'SendInvitationResponse',
};

const DeleteInvitationRequest$json = const {
  '1': 'DeleteInvitationRequest',
  '2': const [
    const {'1': 'invitee_user_id', '3': 1, '4': 1, '5': 3, '10': 'inviteeUserId'},
  ],
};

const DeleteInvitationResponse$json = const {
  '1': 'DeleteInvitationResponse',
};

const AcceptInvitationRequest$json = const {
  '1': 'AcceptInvitationRequest',
  '2': const [
    const {'1': 'inviter_user_id', '3': 1, '4': 1, '5': 3, '10': 'inviterUserId'},
  ],
};

const AcceptInvitationResponse$json = const {
  '1': 'AcceptInvitationResponse',
};

const RejectInvitationRequest$json = const {
  '1': 'RejectInvitationRequest',
  '2': const [
    const {'1': 'inviter_user_id', '3': 1, '4': 1, '5': 3, '10': 'inviterUserId'},
  ],
};

const RejectInvitationResponse$json = const {
  '1': 'RejectInvitationResponse',
};

const GetInvitationListRequest$json = const {
  '1': 'GetInvitationListRequest',
  '2': const [
    const {'1': 'pagination', '3': 1, '4': 1, '5': 11, '6': '.e8.Pagination', '10': 'pagination'},
  ],
};

const GetInvitationListResponse$json = const {
  '1': 'GetInvitationListResponse',
  '2': const [
    const {'1': 'user_profiles', '3': 1, '4': 3, '5': 11, '6': '.e8.UserPublicProfile', '10': 'userProfiles'},
  ],
};

const DeleteFriendRequest$json = const {
  '1': 'DeleteFriendRequest',
  '2': const [
    const {'1': 'friend_user_id', '3': 1, '4': 1, '5': 3, '10': 'friendUserId'},
  ],
};

const DeleteFriendResponse$json = const {
  '1': 'DeleteFriendResponse',
};

const GetFriendListRequest$json = const {
  '1': 'GetFriendListRequest',
  '2': const [
    const {'1': 'pagination', '3': 1, '4': 1, '5': 11, '6': '.e8.Pagination', '10': 'pagination'},
  ],
};

const GetFriendListResponse$json = const {
  '1': 'GetFriendListResponse',
  '2': const [
    const {'1': 'user_profiles', '3': 1, '4': 3, '5': 11, '6': '.e8.UserPublicProfile', '10': 'userProfiles'},
  ],
};

