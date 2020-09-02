///
//  Generated code. Do not modify.
//  source: real_time_message.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

const InvitationReceivedMessage$json = const {
  '1': 'InvitationReceivedMessage',
  '2': const [
    const {'1': 'inviter', '3': 1, '4': 1, '5': 11, '6': '.e8.UserPublicProfile', '10': 'inviter'},
  ],
};

const InvitationAcceptedMessage$json = const {
  '1': 'InvitationAcceptedMessage',
  '2': const [
    const {'1': 'invitee', '3': 1, '4': 1, '5': 11, '6': '.e8.UserPublicProfile', '10': 'invitee'},
  ],
};

const UnreadChatMessage$json = const {
  '1': 'UnreadChatMessage',
  '2': const [
    const {'1': 'message_threads', '3': 1, '4': 3, '5': 11, '6': '.e8.ChatMessageThread', '10': 'messageThreads'},
  ],
};

const RealTimeMessageContent$json = const {
  '1': 'RealTimeMessageContent',
  '2': const [
    const {'1': 'invitation_received', '3': 1, '4': 1, '5': 11, '6': '.e8.InvitationReceivedMessage', '9': 0, '10': 'invitationReceived'},
    const {'1': 'invitation_accepted', '3': 2, '4': 1, '5': 11, '6': '.e8.InvitationAcceptedMessage', '9': 0, '10': 'invitationAccepted'},
    const {'1': 'unread_chat', '3': 3, '4': 1, '5': 11, '6': '.e8.UnreadChatMessage', '9': 0, '10': 'unreadChat'},
  ],
  '8': const [
    const {'1': 'notification_content'},
  ],
};

const RealTimeMessage$json = const {
  '1': 'RealTimeMessage',
  '2': const [
    const {'1': 'real_time_message_id', '3': 1, '4': 1, '5': 3, '10': 'realTimeMessageId'},
    const {'1': 'created_at', '3': 2, '4': 1, '5': 3, '10': 'createdAt'},
    const {'1': 'target_user_id', '3': 3, '4': 1, '5': 3, '10': 'targetUserId'},
    const {'1': 'content', '3': 4, '4': 1, '5': 11, '6': '.e8.RealTimeMessageContent', '10': 'content'},
  ],
};

