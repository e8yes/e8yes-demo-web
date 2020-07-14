///
//  Generated code. Do not modify.
//  source: chat_message.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

const ChatMessageEntry$json = const {
  '1': 'ChatMessageEntry',
  '2': const [
    const {'1': 'thread_id', '3': 1, '4': 1, '5': 3, '10': 'threadId'},
    const {'1': 'message_id', '3': 2, '4': 1, '5': 3, '10': 'messageId'},
    const {'1': 'sender_id', '3': 3, '4': 1, '5': 3, '10': 'senderId'},
    const {'1': 'messages', '3': 4, '4': 3, '5': 9, '10': 'messages'},
    const {'1': 'media_file_accesses', '3': 5, '4': 3, '5': 11, '6': '.e8.FileTokenAccess', '10': 'mediaFileAccesses'},
    const {'1': 'created_at', '3': 6, '4': 1, '5': 3, '10': 'createdAt'},
  ],
};

const ChatMessageThread$json = const {
  '1': 'ChatMessageThread',
  '2': const [
    const {'1': 'thread_id', '3': 1, '4': 1, '5': 3, '10': 'threadId'},
    const {'1': 'thread_title', '3': 2, '4': 1, '5': 9, '10': 'threadTitle'},
    const {'1': 'created_at', '3': 3, '4': 1, '5': 3, '10': 'createdAt'},
    const {'1': 'messages', '3': 4, '4': 3, '5': 11, '6': '.e8.ChatMessageEntry', '10': 'messages'},
  ],
};

