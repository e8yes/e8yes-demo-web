///
//  Generated code. Do not modify.
//  source: service_socialnetwork.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'pagination.pbjson.dart' as $5;
import 'user_profile.pbjson.dart' as $4;
import 'file.pbjson.dart' as $1;

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

const SocialNetworkServiceBase$json = const {
  '1': 'SocialNetworkService',
  '2': const [
    const {'1': 'SendInvitation', '2': '.e8.SendInvitationRequest', '3': '.e8.SendInvitationResponse'},
    const {'1': 'DeleteInvitation', '2': '.e8.DeleteInvitationRequest', '3': '.e8.DeleteInvitationResponse'},
    const {'1': 'AcceptInvitation', '2': '.e8.AcceptInvitationRequest', '3': '.e8.AcceptInvitationResponse'},
    const {'1': 'RejectInvitation', '2': '.e8.RejectInvitationRequest', '3': '.e8.RejectInvitationResponse'},
    const {'1': 'GetInvitationList', '2': '.e8.GetInvitationListRequest', '3': '.e8.GetInvitationListResponse'},
    const {'1': 'DeleteFriend', '2': '.e8.DeleteFriendRequest', '3': '.e8.DeleteFriendResponse'},
    const {'1': 'GetFriendList', '2': '.e8.GetFriendListRequest', '3': '.e8.GetFriendListResponse'},
  ],
};

const SocialNetworkServiceBase$messageJson = const {
  '.e8.SendInvitationRequest': SendInvitationRequest$json,
  '.e8.SendInvitationResponse': SendInvitationResponse$json,
  '.e8.DeleteInvitationRequest': DeleteInvitationRequest$json,
  '.e8.DeleteInvitationResponse': DeleteInvitationResponse$json,
  '.e8.AcceptInvitationRequest': AcceptInvitationRequest$json,
  '.e8.AcceptInvitationResponse': AcceptInvitationResponse$json,
  '.e8.RejectInvitationRequest': RejectInvitationRequest$json,
  '.e8.RejectInvitationResponse': RejectInvitationResponse$json,
  '.e8.GetInvitationListRequest': GetInvitationListRequest$json,
  '.e8.Pagination': $5.Pagination$json,
  '.e8.GetInvitationListResponse': GetInvitationListResponse$json,
  '.e8.UserPublicProfile': $4.UserPublicProfile$json,
  '.e8.FileTokenAccess': $1.FileTokenAccess$json,
  '.e8.DeleteFriendRequest': DeleteFriendRequest$json,
  '.e8.DeleteFriendResponse': DeleteFriendResponse$json,
  '.e8.GetFriendListRequest': GetFriendListRequest$json,
  '.e8.GetFriendListResponse': GetFriendListResponse$json,
};

