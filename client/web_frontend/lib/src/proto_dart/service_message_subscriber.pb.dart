///
//  Generated code. Do not modify.
//  source: service_message_subscriber.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

import 'real_time_message.pb.dart' as $12;

class SubscribeRealTimeMessageQueueRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('SubscribeRealTimeMessageQueueRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  SubscribeRealTimeMessageQueueRequest._() : super();
  factory SubscribeRealTimeMessageQueueRequest() => create();
  factory SubscribeRealTimeMessageQueueRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SubscribeRealTimeMessageQueueRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  SubscribeRealTimeMessageQueueRequest clone() => SubscribeRealTimeMessageQueueRequest()..mergeFromMessage(this);
  SubscribeRealTimeMessageQueueRequest copyWith(void Function(SubscribeRealTimeMessageQueueRequest) updates) => super.copyWith((message) => updates(message as SubscribeRealTimeMessageQueueRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SubscribeRealTimeMessageQueueRequest create() => SubscribeRealTimeMessageQueueRequest._();
  SubscribeRealTimeMessageQueueRequest createEmptyInstance() => create();
  static $pb.PbList<SubscribeRealTimeMessageQueueRequest> createRepeated() => $pb.PbList<SubscribeRealTimeMessageQueueRequest>();
  @$core.pragma('dart2js:noInline')
  static SubscribeRealTimeMessageQueueRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SubscribeRealTimeMessageQueueRequest>(create);
  static SubscribeRealTimeMessageQueueRequest _defaultInstance;
}

class SubscribeRealTimeMessageQueueResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('SubscribeRealTimeMessageQueueResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$12.RealTimeMessage>(1, 'message', subBuilder: $12.RealTimeMessage.create)
    ..hasRequiredFields = false
  ;

  SubscribeRealTimeMessageQueueResponse._() : super();
  factory SubscribeRealTimeMessageQueueResponse() => create();
  factory SubscribeRealTimeMessageQueueResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SubscribeRealTimeMessageQueueResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  SubscribeRealTimeMessageQueueResponse clone() => SubscribeRealTimeMessageQueueResponse()..mergeFromMessage(this);
  SubscribeRealTimeMessageQueueResponse copyWith(void Function(SubscribeRealTimeMessageQueueResponse) updates) => super.copyWith((message) => updates(message as SubscribeRealTimeMessageQueueResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SubscribeRealTimeMessageQueueResponse create() => SubscribeRealTimeMessageQueueResponse._();
  SubscribeRealTimeMessageQueueResponse createEmptyInstance() => create();
  static $pb.PbList<SubscribeRealTimeMessageQueueResponse> createRepeated() => $pb.PbList<SubscribeRealTimeMessageQueueResponse>();
  @$core.pragma('dart2js:noInline')
  static SubscribeRealTimeMessageQueueResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SubscribeRealTimeMessageQueueResponse>(create);
  static SubscribeRealTimeMessageQueueResponse _defaultInstance;

  @$pb.TagNumber(1)
  $12.RealTimeMessage get message => $_getN(0);
  @$pb.TagNumber(1)
  set message($12.RealTimeMessage v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasMessage() => $_has(0);
  @$pb.TagNumber(1)
  void clearMessage() => clearField(1);
  @$pb.TagNumber(1)
  $12.RealTimeMessage ensureMessage() => $_ensure(0);
}

