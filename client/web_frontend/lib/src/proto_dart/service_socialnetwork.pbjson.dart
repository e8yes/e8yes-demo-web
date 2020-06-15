///
//  Generated code. Do not modify.
//  source: service_socialnetwork.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

const GetUserRelationRequest$json = const {
  '1': 'GetUserRelationRequest',
  '2': const [
    const {'1': 'target_user_id', '3': 1, '4': 1, '5': 3, '10': 'targetUserId'},
  ],
};

const GetUserRelationResponse$json = const {
  '1': 'GetUserRelationResponse',
  '2': const [
    const {'1': 'source_user_id', '3': 1, '4': 1, '5': 3, '10': 'sourceUserId'},
    const {'1': 'target_user_id', '3': 2, '4': 1, '5': 3, '10': 'targetUserId'},
    const {'1': 'user_relation', '3': 3, '4': 1, '5': 14, '6': '.UserRelation', '10': 'userRelation'},
  ],
};

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

const DeleteContactRequest$json = const {
  '1': 'DeleteContactRequest',
  '2': const [
    const {'1': 'deleted_contact_user_id', '3': 1, '4': 1, '5': 3, '10': 'deletedContactUserId'},
  ],
};

const DeleteContactResponse$json = const {
  '1': 'DeleteContactResponse',
};

const GetContactListRequest$json = const {
  '1': 'GetContactListRequest',
  '2': const [
    const {'1': 'pagination', '3': 1, '4': 1, '5': 11, '6': '.e8.Pagination', '10': 'pagination'},
  ],
};

const GetContactListResponse$json = const {
  '1': 'GetContactListResponse',
  '2': const [
    const {'1': 'user_profiles', '3': 1, '4': 3, '5': 11, '6': '.e8.UserPublicProfile', '10': 'userProfiles'},
  ],
};

