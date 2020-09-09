///
//  Generated code. Do not modify.
//  source: service_chat_message.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

const CreateChatMessageThreadRequest$json = const {
  '1': 'CreateChatMessageThreadRequest',
  '2': const [
    const {'1': 'channel_id', '3': 1, '4': 1, '5': 3, '10': 'channelId'},
    const {'1': 'thread_title', '3': 2, '4': 1, '5': 9, '10': 'threadTitle'},
  ],
};

const CreateChatMessageThreadResponse$json = const {
  '1': 'CreateChatMessageThreadResponse',
  '2': const [
    const {'1': 'thread_id', '3': 1, '4': 1, '5': 3, '10': 'threadId'},
  ],
};

const SendChatMessageRequest$json = const {
  '1': 'SendChatMessageRequest',
  '2': const [
    const {'1': 'thread_id', '3': 1, '4': 1, '5': 3, '10': 'threadId'},
    const {'1': 'message', '3': 2, '4': 1, '5': 11, '6': '.e8.ChatMessageEntry', '10': 'message'},
  ],
};

const SendChatMessageResponse$json = const {
  '1': 'SendChatMessageResponse',
  '2': const [
    const {'1': 'updated_thread', '3': 1, '4': 1, '5': 11, '6': '.e8.ChatMessageThread', '10': 'updatedThread'},
  ],
};

const GetChatMessageRequest$json = const {
  '1': 'GetChatMessageRequest',
  '2': const [
    const {'1': 'channel_id', '3': 1, '4': 1, '5': 3, '10': 'channelId'},
    const {'1': 'pagination', '3': 2, '4': 1, '5': 11, '6': '.e8.Pagination', '10': 'pagination'},
  ],
};

const GetChatMessageResponse$json = const {
  '1': 'GetChatMessageResponse',
  '2': const [
    const {'1': 'threads', '3': 1, '4': 3, '5': 11, '6': '.e8.ChatMessageThread', '10': 'threads'},
  ],
};

