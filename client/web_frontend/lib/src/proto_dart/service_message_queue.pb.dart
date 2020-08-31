///
//  Generated code. Do not modify.
//  source: service_message_queue.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'real_time_message.pb.dart' as $12;

class EnqueueMessageRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('EnqueueMessageRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'userId')
    ..pc<$12.RealTimeMessage>(2, 'messages', $pb.PbFieldType.PM, subBuilder: $12.RealTimeMessage.create)
    ..hasRequiredFields = false
  ;

  EnqueueMessageRequest._() : super();
  factory EnqueueMessageRequest() => create();
  factory EnqueueMessageRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory EnqueueMessageRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  EnqueueMessageRequest clone() => EnqueueMessageRequest()..mergeFromMessage(this);
  EnqueueMessageRequest copyWith(void Function(EnqueueMessageRequest) updates) => super.copyWith((message) => updates(message as EnqueueMessageRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static EnqueueMessageRequest create() => EnqueueMessageRequest._();
  EnqueueMessageRequest createEmptyInstance() => create();
  static $pb.PbList<EnqueueMessageRequest> createRepeated() => $pb.PbList<EnqueueMessageRequest>();
  @$core.pragma('dart2js:noInline')
  static EnqueueMessageRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<EnqueueMessageRequest>(create);
  static EnqueueMessageRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get userId => $_getI64(0);
  @$pb.TagNumber(1)
  set userId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearUserId() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$12.RealTimeMessage> get messages => $_getList(1);
}

class EnqueueMessageResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('EnqueueMessageResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  EnqueueMessageResponse._() : super();
  factory EnqueueMessageResponse() => create();
  factory EnqueueMessageResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory EnqueueMessageResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  EnqueueMessageResponse clone() => EnqueueMessageResponse()..mergeFromMessage(this);
  EnqueueMessageResponse copyWith(void Function(EnqueueMessageResponse) updates) => super.copyWith((message) => updates(message as EnqueueMessageResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static EnqueueMessageResponse create() => EnqueueMessageResponse._();
  EnqueueMessageResponse createEmptyInstance() => create();
  static $pb.PbList<EnqueueMessageResponse> createRepeated() => $pb.PbList<EnqueueMessageResponse>();
  @$core.pragma('dart2js:noInline')
  static EnqueueMessageResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<EnqueueMessageResponse>(create);
  static EnqueueMessageResponse _defaultInstance;
}

class DequeueMessageRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DequeueMessageRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'userId')
    ..aOB(2, 'previousMessageDelivered')
    ..aOB(3, 'endOperation')
    ..hasRequiredFields = false
  ;

  DequeueMessageRequest._() : super();
  factory DequeueMessageRequest() => create();
  factory DequeueMessageRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DequeueMessageRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DequeueMessageRequest clone() => DequeueMessageRequest()..mergeFromMessage(this);
  DequeueMessageRequest copyWith(void Function(DequeueMessageRequest) updates) => super.copyWith((message) => updates(message as DequeueMessageRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DequeueMessageRequest create() => DequeueMessageRequest._();
  DequeueMessageRequest createEmptyInstance() => create();
  static $pb.PbList<DequeueMessageRequest> createRepeated() => $pb.PbList<DequeueMessageRequest>();
  @$core.pragma('dart2js:noInline')
  static DequeueMessageRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DequeueMessageRequest>(create);
  static DequeueMessageRequest _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get userId => $_getI64(0);
  @$pb.TagNumber(1)
  set userId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasUserId() => $_has(0);
  @$pb.TagNumber(1)
  void clearUserId() => clearField(1);

  @$pb.TagNumber(2)
  $core.bool get previousMessageDelivered => $_getBF(1);
  @$pb.TagNumber(2)
  set previousMessageDelivered($core.bool v) { $_setBool(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPreviousMessageDelivered() => $_has(1);
  @$pb.TagNumber(2)
  void clearPreviousMessageDelivered() => clearField(2);

  @$pb.TagNumber(3)
  $core.bool get endOperation => $_getBF(2);
  @$pb.TagNumber(3)
  set endOperation($core.bool v) { $_setBool(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasEndOperation() => $_has(2);
  @$pb.TagNumber(3)
  void clearEndOperation() => clearField(3);
}

class DequeueMessageResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DequeueMessageResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$12.RealTimeMessage>(2, 'message', subBuilder: $12.RealTimeMessage.create)
    ..hasRequiredFields = false
  ;

  DequeueMessageResponse._() : super();
  factory DequeueMessageResponse() => create();
  factory DequeueMessageResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DequeueMessageResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DequeueMessageResponse clone() => DequeueMessageResponse()..mergeFromMessage(this);
  DequeueMessageResponse copyWith(void Function(DequeueMessageResponse) updates) => super.copyWith((message) => updates(message as DequeueMessageResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DequeueMessageResponse create() => DequeueMessageResponse._();
  DequeueMessageResponse createEmptyInstance() => create();
  static $pb.PbList<DequeueMessageResponse> createRepeated() => $pb.PbList<DequeueMessageResponse>();
  @$core.pragma('dart2js:noInline')
  static DequeueMessageResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DequeueMessageResponse>(create);
  static DequeueMessageResponse _defaultInstance;

  @$pb.TagNumber(2)
  $12.RealTimeMessage get message => $_getN(0);
  @$pb.TagNumber(2)
  set message($12.RealTimeMessage v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasMessage() => $_has(0);
  @$pb.TagNumber(2)
  void clearMessage() => clearField(2);
  @$pb.TagNumber(2)
  $12.RealTimeMessage ensureMessage() => $_ensure(0);
}

