///
//  Generated code. Do not modify.
//  source: message_queue_stats.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

class MessageQueueStats extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('MessageQueueStats', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..a<$core.int>(1, 'totalNumQueues', $pb.PbFieldType.O3)
    ..a<$core.int>(2, 'numQueuesLength0', $pb.PbFieldType.O3, protoName: 'num_queues_length_0')
    ..a<$core.int>(3, 'numQueuesLength110', $pb.PbFieldType.O3, protoName: 'num_queues_length_1_10')
    ..a<$core.int>(4, 'numQueuesLength11100', $pb.PbFieldType.O3, protoName: 'num_queues_length_11_100')
    ..a<$core.int>(5, 'numQueuesLength1011000', $pb.PbFieldType.O3, protoName: 'num_queues_length_101_1000')
    ..a<$core.int>(6, 'numQueuesLengthGte1001', $pb.PbFieldType.O3, protoName: 'num_queues_length_gte_1001')
    ..hasRequiredFields = false
  ;

  MessageQueueStats._() : super();
  factory MessageQueueStats() => create();
  factory MessageQueueStats.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MessageQueueStats.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  MessageQueueStats clone() => MessageQueueStats()..mergeFromMessage(this);
  MessageQueueStats copyWith(void Function(MessageQueueStats) updates) => super.copyWith((message) => updates(message as MessageQueueStats));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static MessageQueueStats create() => MessageQueueStats._();
  MessageQueueStats createEmptyInstance() => create();
  static $pb.PbList<MessageQueueStats> createRepeated() => $pb.PbList<MessageQueueStats>();
  @$core.pragma('dart2js:noInline')
  static MessageQueueStats getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MessageQueueStats>(create);
  static MessageQueueStats _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get totalNumQueues => $_getIZ(0);
  @$pb.TagNumber(1)
  set totalNumQueues($core.int v) { $_setSignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTotalNumQueues() => $_has(0);
  @$pb.TagNumber(1)
  void clearTotalNumQueues() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get numQueuesLength0 => $_getIZ(1);
  @$pb.TagNumber(2)
  set numQueuesLength0($core.int v) { $_setSignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasNumQueuesLength0() => $_has(1);
  @$pb.TagNumber(2)
  void clearNumQueuesLength0() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get numQueuesLength110 => $_getIZ(2);
  @$pb.TagNumber(3)
  set numQueuesLength110($core.int v) { $_setSignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasNumQueuesLength110() => $_has(2);
  @$pb.TagNumber(3)
  void clearNumQueuesLength110() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get numQueuesLength11100 => $_getIZ(3);
  @$pb.TagNumber(4)
  set numQueuesLength11100($core.int v) { $_setSignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasNumQueuesLength11100() => $_has(3);
  @$pb.TagNumber(4)
  void clearNumQueuesLength11100() => clearField(4);

  @$pb.TagNumber(5)
  $core.int get numQueuesLength1011000 => $_getIZ(4);
  @$pb.TagNumber(5)
  set numQueuesLength1011000($core.int v) { $_setSignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasNumQueuesLength1011000() => $_has(4);
  @$pb.TagNumber(5)
  void clearNumQueuesLength1011000() => clearField(5);

  @$pb.TagNumber(6)
  $core.int get numQueuesLengthGte1001 => $_getIZ(5);
  @$pb.TagNumber(6)
  set numQueuesLengthGte1001($core.int v) { $_setSignedInt32(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasNumQueuesLengthGte1001() => $_has(5);
  @$pb.TagNumber(6)
  void clearNumQueuesLengthGte1001() => clearField(6);
}

