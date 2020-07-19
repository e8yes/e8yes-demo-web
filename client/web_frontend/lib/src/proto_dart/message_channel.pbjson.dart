///
//  Generated code. Do not modify.
//  source: message_channel.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

const MessageChannelMemberType$json = const {
  '1': 'MessageChannelMemberType',
  '2': const [
    const {'1': 'MCMT_INVALID', '2': 0},
    const {'1': 'MCMT_MEMBER', '2': 1},
    const {'1': 'MCMT_ADMIN', '2': 2},
  ],
};

const MessageChannelRelation$json = const {
  '1': 'MessageChannelRelation',
  '2': const [
    const {'1': 'member_type', '3': 1, '4': 1, '5': 14, '6': '.e8.MessageChannelMemberType', '10': 'memberType'},
    const {'1': 'join_at', '3': 2, '4': 1, '5': 3, '10': 'joinAt'},
  ],
};

const MessageChannel$json = const {
  '1': 'MessageChannel',
  '2': const [
    const {'1': 'channel_id', '3': 1, '4': 1, '5': 3, '10': 'channelId'},
    const {'1': 'title', '3': 2, '4': 1, '5': 9, '10': 'title'},
    const {'1': 'description', '3': 3, '4': 1, '5': 9, '10': 'description'},
    const {'1': 'avatar_readonly_access', '3': 4, '4': 1, '5': 11, '6': '.e8.FileTokenAccess', '10': 'avatarReadonlyAccess'},
    const {'1': 'avatar_preview_readonly_access', '3': 5, '4': 1, '5': 11, '6': '.e8.FileTokenAccess', '10': 'avatarPreviewReadonlyAccess'},
    const {'1': 'created_at', '3': 6, '4': 1, '5': 3, '10': 'createdAt'},
    const {'1': 'relation', '3': 7, '4': 1, '5': 11, '6': '.e8.MessageChannelRelation', '10': 'relation'},
  ],
};

