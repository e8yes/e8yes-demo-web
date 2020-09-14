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
    const {'1': 'member_ids', '3': 5, '4': 3, '5': 3, '10': 'memberIds'},
  ],
};

const CreateMessageChannelResponse$json = const {
  '1': 'CreateMessageChannelResponse',
  '2': const [
    const {'1': 'channel_id', '3': 1, '4': 1, '5': 3, '10': 'channelId'},
  ],
};

const UpdateMessageChannelRequest$json = const {
  '1': 'UpdateMessageChannelRequest',
  '2': const [
    const {'1': 'channel_id', '3': 1, '4': 1, '5': 3, '10': 'channelId'},
    const {'1': 'title', '3': 2, '4': 1, '5': 11, '6': '.e8.NullableString', '10': 'title'},
    const {'1': 'description', '3': 3, '4': 1, '5': 11, '6': '.e8.NullableString', '10': 'description'},
    const {'1': 'member_ids', '3': 4, '4': 3, '5': 3, '10': 'memberIds'},
  ],
};

const UpdateMessageChannelResponse$json = const {
  '1': 'UpdateMessageChannelResponse',
};

const AddUserToMessageChannelRequest$json = const {
  '1': 'AddUserToMessageChannelRequest',
  '2': const [
    const {'1': 'channel_id', '3': 1, '4': 1, '5': 3, '10': 'channelId'},
    const {'1': 'user_id', '3': 2, '4': 1, '5': 3, '10': 'userId'},
    const {'1': 'member_type', '3': 3, '4': 1, '5': 14, '6': '.e8.MessageChannelMemberType', '10': 'memberType'},
  ],
};

const AddUserToMessageChannelResponse$json = const {
  '1': 'AddUserToMessageChannelResponse',
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

const SearchMessageChannelsRequest$json = const {
  '1': 'SearchMessageChannelsRequest',
  '2': const [
    const {'1': 'pagination', '3': 1, '4': 1, '5': 11, '6': '.e8.Pagination', '10': 'pagination'},
    const {'1': 'with_member_ids', '3': 2, '4': 3, '5': 3, '10': 'withMemberIds'},
    const {'1': 'active_member_fetch_limit', '3': 3, '4': 1, '5': 5, '10': 'activeMemberFetchLimit'},
    const {'1': 'channel_ids', '3': 4, '4': 3, '5': 3, '10': 'channelIds'},
    const {'1': 'search_text', '3': 5, '4': 1, '5': 11, '6': '.e8.NullableString', '10': 'searchText'},
  ],
};

const SearchMessageChannelsResponse$json = const {
  '1': 'SearchMessageChannelsResponse',
  '2': const [
    const {'1': 'channels', '3': 1, '4': 3, '5': 11, '6': '.e8.MessageChannelOverview', '10': 'channels'},
  ],
};

const GetMessageChannelMembersRequest$json = const {
  '1': 'GetMessageChannelMembersRequest',
  '2': const [
    const {'1': 'channel_id', '3': 1, '4': 1, '5': 3, '10': 'channelId'},
    const {'1': 'pagination', '3': 2, '4': 1, '5': 11, '6': '.e8.Pagination', '10': 'pagination'},
  ],
};

const GetMessageChannelMembersResponse$json = const {
  '1': 'GetMessageChannelMembersResponse',
  '2': const [
    const {'1': 'user_profiles', '3': 1, '4': 3, '5': 11, '6': '.e8.UserPublicProfile', '10': 'userProfiles'},
    const {'1': 'channel_relations', '3': 2, '4': 3, '5': 11, '6': '.e8.MessageChannelRelation', '10': 'channelRelations'},
  ],
};

