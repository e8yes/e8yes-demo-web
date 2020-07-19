///
//  Generated code. Do not modify.
//  source: service_message_channel.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

const CreateMessageChannelRequest$json = const {
  '1': 'CreateMessageChannelRequest',
  '2': const [
    const {'1': 'encrypted', '3': 1, '4': 1, '5': 8, '10': 'encrypted'},
    const {'1': 'close_group_channel', '3': 2, '4': 1, '5': 8, '10': 'closeGroupChannel'},
    const {'1': 'title', '3': 3, '4': 1, '5': 11, '6': '.e8.NullableString', '10': 'title'},
    const {'1': 'description', '3': 4, '4': 1, '5': 11, '6': '.e8.NullableString', '10': 'description'},
  ],
};

const CreateMessageChannelResponse$json = const {
  '1': 'CreateMessageChannelResponse',
  '2': const [
    const {'1': 'channel_id', '3': 1, '4': 1, '5': 3, '10': 'channelId'},
  ],
};

const AddMemberToMessageChannelRequest$json = const {
  '1': 'AddMemberToMessageChannelRequest',
  '2': const [
    const {'1': 'channel_id', '3': 1, '4': 1, '5': 3, '10': 'channelId'},
    const {'1': 'user_id', '3': 2, '4': 1, '5': 3, '10': 'userId'},
  ],
};

const AddMemberToMessageChannelResponse$json = const {
  '1': 'AddMemberToMessageChannelResponse',
};

const GetMessageChannelKeyRequest$json = const {
  '1': 'GetMessageChannelKeyRequest',
  '2': const [
    const {'1': 'channel_id', '3': 1, '4': 1, '5': 3, '10': 'channelId'},
    const {'1': 'timeout_secs', '3': 2, '4': 1, '5': 5, '10': 'timeoutSecs'},
  ],
};

const GetMessageChannelKeyResponse$json = const {
  '1': 'GetMessageChannelKeyResponse',
  '2': const [
    const {'1': 'channel_key', '3': 3, '4': 1, '5': 9, '10': 'channelKey'},
  ],
};

const LeaveMessageChannelRequest$json = const {
  '1': 'LeaveMessageChannelRequest',
  '2': const [
    const {'1': 'channel_id', '3': 1, '4': 1, '5': 3, '10': 'channelId'},
  ],
};

const LeaveMessageChannelResponse$json = const {
  '1': 'LeaveMessageChannelResponse',
};

const GetJoinedInMessageChannelsRequest$json = const {
  '1': 'GetJoinedInMessageChannelsRequest',
  '2': const [
    const {'1': 'pagination', '3': 1, '4': 1, '5': 11, '6': '.e8.Pagination', '10': 'pagination'},
  ],
};

const GetJoinedInMessageChannelsResponse$json = const {
  '1': 'GetJoinedInMessageChannelsResponse',
  '2': const [
    const {'1': 'channels', '3': 1, '4': 3, '5': 11, '6': '.e8.MessageChannel', '10': 'channels'},
  ],
};

const GetMessageChannelMembersRequest$json = const {
  '1': 'GetMessageChannelMembersRequest',
  '2': const [
    const {'1': 'channel_id', '3': 1, '4': 1, '5': 3, '10': 'channelId'},
  ],
};

const GetMessageChannelMembersResponse$json = const {
  '1': 'GetMessageChannelMembersResponse',
  '2': const [
    const {'1': 'user_profiles', '3': 1, '4': 3, '5': 11, '6': '.e8.UserPublicProfile', '10': 'userProfiles'},
    const {'1': 'member_types', '3': 2, '4': 3, '5': 11, '6': '.e8.GetMessageChannelMembersResponse.MemberTypesEntry', '10': 'memberTypes'},
  ],
  '3': const [GetMessageChannelMembersResponse_MemberTypesEntry$json],
};

const GetMessageChannelMembersResponse_MemberTypesEntry$json = const {
  '1': 'MemberTypesEntry',
  '2': const [
    const {'1': 'key', '3': 1, '4': 1, '5': 3, '10': 'key'},
    const {'1': 'value', '3': 2, '4': 1, '5': 14, '6': '.e8.MessageChannelMemberType', '10': 'value'},
  ],
  '7': const {'7': true},
};

