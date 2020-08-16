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
    ..pc<$14.NodeStateRevision>(1, 'revisions', $pb.PbFieldType.PM, subBuilder: $14.NodeStateRevision.create)
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
  $core.List<$14.NodeStateRevision> get revisions => $_getList(0);
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

class GetCurrentRevisionEpochRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetCurrentRevisionEpochRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  GetCurrentRevisionEpochRequest._() : super();
  factory GetCurrentRevisionEpochRequest() => create();
  factory GetCurrentRevisionEpochRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetCurrentRevisionEpochRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetCurrentRevisionEpochRequest clone() => GetCurrentRevisionEpochRequest()..mergeFromMessage(this);
  GetCurrentRevisionEpochRequest copyWith(void Function(GetCurrentRevisionEpochRequest) updates) => super.copyWith((message) => updates(message as GetCurrentRevisionEpochRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetCurrentRevisionEpochRequest create() => GetCurrentRevisionEpochRequest._();
  GetCurrentRevisionEpochRequest createEmptyInstance() => create();
  static $pb.PbList<GetCurrentRevisionEpochRequest> createRepeated() => $pb.PbList<GetCurrentRevisionEpochRequest>();
  @$core.pragma('dart2js:noInline')
  static GetCurrentRevisionEpochRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetCurrentRevisionEpochRequest>(create);
  static GetCurrentRevisionEpochRequest _defaultInstance;
}

class GetCurrentRevisionEpochResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('GetCurrentRevisionEpochResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..aInt64(1, 'revisionEpoch')
    ..hasRequiredFields = false
  ;

  GetCurrentRevisionEpochResponse._() : super();
  factory GetCurrentRevisionEpochResponse() => create();
  factory GetCurrentRevisionEpochResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GetCurrentRevisionEpochResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  GetCurrentRevisionEpochResponse clone() => GetCurrentRevisionEpochResponse()..mergeFromMessage(this);
  GetCurrentRevisionEpochResponse copyWith(void Function(GetCurrentRevisionEpochResponse) updates) => super.copyWith((message) => updates(message as GetCurrentRevisionEpochResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GetCurrentRevisionEpochResponse create() => GetCurrentRevisionEpochResponse._();
  GetCurrentRevisionEpochResponse createEmptyInstance() => create();
  static $pb.PbList<GetCurrentRevisionEpochResponse> createRepeated() => $pb.PbList<GetCurrentRevisionEpochResponse>();
  @$core.pragma('dart2js:noInline')
  static GetCurrentRevisionEpochResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GetCurrentRevisionEpochResponse>(create);
  static GetCurrentRevisionEpochResponse _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get revisionEpoch => $_getI64(0);
  @$pb.TagNumber(1)
  set revisionEpoch($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasRevisionEpoch() => $_has(0);
  @$pb.TagNumber(1)
  void clearRevisionEpoch() => clearField(1);
}

class AddPeersRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('AddPeersRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$14.NodeState>(1, 'nodes', $pb.PbFieldType.PM, subBuilder: $14.NodeState.create)
    ..hasRequiredFields = false
  ;

  AddPeersRequest._() : super();
  factory AddPeersRequest() => create();
  factory AddPeersRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AddPeersRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  AddPeersRequest clone() => AddPeersRequest()..mergeFromMessage(this);
  AddPeersRequest copyWith(void Function(AddPeersRequest) updates) => super.copyWith((message) => updates(message as AddPeersRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static AddPeersRequest create() => AddPeersRequest._();
  AddPeersRequest createEmptyInstance() => create();
  static $pb.PbList<AddPeersRequest> createRepeated() => $pb.PbList<AddPeersRequest>();
  @$core.pragma('dart2js:noInline')
  static AddPeersRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AddPeersRequest>(create);
  static AddPeersRequest _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$14.NodeState> get nodes => $_getList(0);
}

class AddPeersResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('AddPeersResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  AddPeersResponse._() : super();
  factory AddPeersResponse() => create();
  factory AddPeersResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AddPeersResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  AddPeersResponse clone() => AddPeersResponse()..mergeFromMessage(this);
  AddPeersResponse copyWith(void Function(AddPeersResponse) updates) => super.copyWith((message) => updates(message as AddPeersResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static AddPeersResponse create() => AddPeersResponse._();
  AddPeersResponse createEmptyInstance() => create();
  static $pb.PbList<AddPeersResponse> createRepeated() => $pb.PbList<AddPeersResponse>();
  @$core.pragma('dart2js:noInline')
  static AddPeersResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AddPeersResponse>(create);
  static AddPeersResponse _defaultInstance;
}

class DeletePeersRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DeletePeersRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pPS(1, 'nodeNames')
    ..hasRequiredFields = false
  ;

  DeletePeersRequest._() : super();
  factory DeletePeersRequest() => create();
  factory DeletePeersRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeletePeersRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DeletePeersRequest clone() => DeletePeersRequest()..mergeFromMessage(this);
  DeletePeersRequest copyWith(void Function(DeletePeersRequest) updates) => super.copyWith((message) => updates(message as DeletePeersRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DeletePeersRequest create() => DeletePeersRequest._();
  DeletePeersRequest createEmptyInstance() => create();
  static $pb.PbList<DeletePeersRequest> createRepeated() => $pb.PbList<DeletePeersRequest>();
  @$core.pragma('dart2js:noInline')
  static DeletePeersRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeletePeersRequest>(create);
  static DeletePeersRequest _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.String> get nodeNames => $_getList(0);
}

class DeletePeersResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('DeletePeersResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  DeletePeersResponse._() : super();
  factory DeletePeersResponse() => create();
  factory DeletePeersResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeletePeersResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  DeletePeersResponse clone() => DeletePeersResponse()..mergeFromMessage(this);
  DeletePeersResponse copyWith(void Function(DeletePeersResponse) updates) => super.copyWith((message) => updates(message as DeletePeersResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DeletePeersResponse create() => DeletePeersResponse._();
  DeletePeersResponse createEmptyInstance() => create();
  static $pb.PbList<DeletePeersResponse> createRepeated() => $pb.PbList<DeletePeersResponse>();
  @$core.pragma('dart2js:noInline')
  static DeletePeersResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeletePeersResponse>(create);
  static DeletePeersResponse _defaultInstance;
}

