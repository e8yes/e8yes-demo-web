///
//  Generated code. Do not modify.
//  source: service_node_state.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'node.pb.dart' as $14;

class ReviseNodeStateRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('ReviseNodeStateRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$14.NodeStateRevision>(1, 'revision', subBuilder: $14.NodeStateRevision.create)
    ..hasRequiredFields = false
  ;

  ReviseNodeStateRequest._() : super();
  factory ReviseNodeStateRequest() => create();
  factory ReviseNodeStateRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ReviseNodeStateRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  ReviseNodeStateRequest clone() => ReviseNodeStateRequest()..mergeFromMessage(this);
  ReviseNodeStateRequest copyWith(void Function(ReviseNodeStateRequest) updates) => super.copyWith((message) => updates(message as ReviseNodeStateRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ReviseNodeStateRequest create() => ReviseNodeStateRequest._();
  ReviseNodeStateRequest createEmptyInstance() => create();
  static $pb.PbList<ReviseNodeStateRequest> createRepeated() => $pb.PbList<ReviseNodeStateRequest>();
  @$core.pragma('dart2js:noInline')
  static ReviseNodeStateRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ReviseNodeStateRequest>(create);
  static ReviseNodeStateRequest _defaultInstance;

  @$pb.TagNumber(1)
  $14.NodeStateRevision get revision => $_getN(0);
  @$pb.TagNumber(1)
  set revision($14.NodeStateRevision v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasRevision() => $_has(0);
  @$pb.TagNumber(1)
  void clearRevision() => clearField(1);
  @$pb.TagNumber(1)
  $14.NodeStateRevision ensureRevision() => $_ensure(0);
}

class ReviseNodeStateResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('ReviseNodeStateResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  ReviseNodeStateResponse._() : super();
  factory ReviseNodeStateResponse() => create();
  factory ReviseNodeStateResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ReviseNodeStateResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  ReviseNodeStateResponse clone() => ReviseNodeStateResponse()..mergeFromMessage(this);
  ReviseNodeStateResponse copyWith(void Function(ReviseNodeStateResponse) updates) => super.copyWith((message) => updates(message as ReviseNodeStateResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ReviseNodeStateResponse create() => ReviseNodeStateResponse._();
  ReviseNodeStateResponse createEmptyInstance() => create();
  static $pb.PbList<ReviseNodeStateResponse> createRepeated() => $pb.PbList<ReviseNodeStateResponse>();
  @$core.pragma('dart2js:noInline')
  static ReviseNodeStateResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ReviseNodeStateResponse>(create);
  static ReviseNodeStateResponse _defaultInstance;
}

class GetLatestRevisionEpochRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetLatestRevisionEpochRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  GetLatestRevisionEpochRequest._() : super();
  factory GetLatestRevisionEpochRequest() => create();
  factory GetLatestRevisionEpochRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetLatestRevisionEpochRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetLatestRevisionEpochRequest clone() => GetLatestRevisionEpochRequest()..mergeFromMessage(this);
  GetLatestRevisionEpochRequest copyWith(void Function(GetLatestRevisionEpochRequest) updates) => super.copyWith((message) => updates(message as GetLatestRevisionEpochRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetLatestRevisionEpochRequest create() => GetLatestRevisionEpochRequest._();
  GetLatestRevisionEpochRequest createEmptyInstance() => create();
  static $pb.PbList<GetLatestRevisionEpochRequest> createRepeated() => $pb.PbList<GetLatestRevisionEpochRequest>();
  @$core.pragma('dart2js:noInline')
  static GetLatestRevisionEpochRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetLatestRevisionEpochRequest>(create);
  static GetLatestRevisionEpochRequest _defaultInstance;
}

class GetLatestRevisionEpochResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetLatestRevisionEpochResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'revisionEpoch')
    ..hasRequiredFields = false
  ;

  GetLatestRevisionEpochResponse._() : super();
  factory GetLatestRevisionEpochResponse() => create();
  factory GetLatestRevisionEpochResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetLatestRevisionEpochResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetLatestRevisionEpochResponse clone() => GetLatestRevisionEpochResponse()..mergeFromMessage(this);
  GetLatestRevisionEpochResponse copyWith(void Function(GetLatestRevisionEpochResponse) updates) => super.copyWith((message) => updates(message as GetLatestRevisionEpochResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetLatestRevisionEpochResponse create() => GetLatestRevisionEpochResponse._();
  GetLatestRevisionEpochResponse createEmptyInstance() => create();
  static $pb.PbList<GetLatestRevisionEpochResponse> createRepeated() => $pb.PbList<GetLatestRevisionEpochResponse>();
  @$core.pragma('dart2js:noInline')
  static GetLatestRevisionEpochResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetLatestRevisionEpochResponse>(create);
  static GetLatestRevisionEpochResponse _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get revisionEpoch => $_getI64(0);
  @$pb.TagNumber(1)
  set revisionEpoch($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasRevisionEpoch() => $_has(0);
  @$pb.TagNumber(1)
  void clearRevisionEpoch() => clearField(1);
}

class AddPeerRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('AddPeerRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOM<$14.NodeState>(1, 'node', subBuilder: $14.NodeState.create)
    ..hasRequiredFields = false
  ;

  AddPeerRequest._() : super();
  factory AddPeerRequest() => create();
  factory AddPeerRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AddPeerRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  AddPeerRequest clone() => AddPeerRequest()..mergeFromMessage(this);
  AddPeerRequest copyWith(void Function(AddPeerRequest) updates) => super.copyWith((message) => updates(message as AddPeerRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static AddPeerRequest create() => AddPeerRequest._();
  AddPeerRequest createEmptyInstance() => create();
  static $pb.PbList<AddPeerRequest> createRepeated() => $pb.PbList<AddPeerRequest>();
  @$core.pragma('dart2js:noInline')
  static AddPeerRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AddPeerRequest>(create);
  static AddPeerRequest _defaultInstance;

  @$pb.TagNumber(1)
  $14.NodeState get node => $_getN(0);
  @$pb.TagNumber(1)
  set node($14.NodeState v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasNode() => $_has(0);
  @$pb.TagNumber(1)
  void clearNode() => clearField(1);
  @$pb.TagNumber(1)
  $14.NodeState ensureNode() => $_ensure(0);
}

class AddPeerResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('AddPeerResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  AddPeerResponse._() : super();
  factory AddPeerResponse() => create();
  factory AddPeerResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AddPeerResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  AddPeerResponse clone() => AddPeerResponse()..mergeFromMessage(this);
  AddPeerResponse copyWith(void Function(AddPeerResponse) updates) => super.copyWith((message) => updates(message as AddPeerResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static AddPeerResponse create() => AddPeerResponse._();
  AddPeerResponse createEmptyInstance() => create();
  static $pb.PbList<AddPeerResponse> createRepeated() => $pb.PbList<AddPeerResponse>();
  @$core.pragma('dart2js:noInline')
  static AddPeerResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AddPeerResponse>(create);
  static AddPeerResponse _defaultInstance;
}

class DeletePeerRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DeletePeerRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aOS(1, 'nodeName')
    ..hasRequiredFields = false
  ;

  DeletePeerRequest._() : super();
  factory DeletePeerRequest() => create();
  factory DeletePeerRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeletePeerRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DeletePeerRequest clone() => DeletePeerRequest()..mergeFromMessage(this);
  DeletePeerRequest copyWith(void Function(DeletePeerRequest) updates) => super.copyWith((message) => updates(message as DeletePeerRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DeletePeerRequest create() => DeletePeerRequest._();
  DeletePeerRequest createEmptyInstance() => create();
  static $pb.PbList<DeletePeerRequest> createRepeated() => $pb.PbList<DeletePeerRequest>();
  @$core.pragma('dart2js:noInline')
  static DeletePeerRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeletePeerRequest>(create);
  static DeletePeerRequest _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get nodeName => $_getSZ(0);
  @$pb.TagNumber(1)
  set nodeName($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasNodeName() => $_has(0);
  @$pb.TagNumber(1)
  void clearNodeName() => clearField(1);
}

class DeletePeerResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DeletePeerResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  DeletePeerResponse._() : super();
  factory DeletePeerResponse() => create();
  factory DeletePeerResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeletePeerResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DeletePeerResponse clone() => DeletePeerResponse()..mergeFromMessage(this);
  DeletePeerResponse copyWith(void Function(DeletePeerResponse) updates) => super.copyWith((message) => updates(message as DeletePeerResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DeletePeerResponse create() => DeletePeerResponse._();
  DeletePeerResponse createEmptyInstance() => create();
  static $pb.PbList<DeletePeerResponse> createRepeated() => $pb.PbList<DeletePeerResponse>();
  @$core.pragma('dart2js:noInline')
  static DeletePeerResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeletePeerResponse>(create);
  static DeletePeerResponse _defaultInstance;
}

