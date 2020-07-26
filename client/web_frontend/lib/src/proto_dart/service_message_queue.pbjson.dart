///
//  Generated code. Do not modify.
//  source: service_message_queue.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

const EnqueueMessageRequest$json = const {
  '1': 'EnqueueMessageRequest',
  '2': const [
    const {'1': 'user_id', '3': 1, '4': 1, '5': 3, '10': 'userId'},
    const {'1': 'messages', '3': 2, '4': 3, '5': 11, '6': '.e8.RealTimeMessage', '10': 'messages'},
  ],
};

const EnqueueMessageResponse$json = const {
  '1': 'EnqueueMessageResponse',
};

const DequeueMessageRequest$json = const {
  '1': 'DequeueMessageRequest',
  '2': const [
    const {'1': 'user_id', '3': 1, '4': 1, '5': 3, '10': 'userId'},
  ],
};

const DequeueMessageResponse$json = const {
  '1': 'DequeueMessageResponse',
  '2': const [
    const {'1': 'messages', '3': 2, '4': 3, '5': 11, '6': '.e8.RealTimeMessage', '10': 'messages'},
  ],
};

