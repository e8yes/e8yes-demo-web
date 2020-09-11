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
    const {'1': 'previous_message_delivered', '3': 2, '4': 1, '5': 8, '10': 'previousMessageDelivered'},
    const {'1': 'end_operation', '3': 3, '4': 1, '5': 8, '10': 'endOperation'},
    const {'1': 'wait_duration_secs', '3': 4, '4': 1, '5': 5, '10': 'waitDurationSecs'},
  ],
};

const DequeueMessageResponse$json = const {
  '1': 'DequeueMessageResponse',
  '2': const [
    const {'1': 'message', '3': 1, '4': 1, '5': 11, '6': '.e8.RealTimeMessage', '10': 'message'},
  ],
};

const ListQueueMessageRequest$json = const {
  '1': 'ListQueueMessageRequest',
  '2': const [
    const {'1': 'user_id', '3': 1, '4': 1, '5': 3, '10': 'userId'},
  ],
};

const ListQueueMessageResponse$json = const {
  '1': 'ListQueueMessageResponse',
  '2': const [
    const {'1': 'messages', '3': 1, '4': 3, '5': 11, '6': '.e8.RealTimeMessage', '10': 'messages'},
  ],
};

