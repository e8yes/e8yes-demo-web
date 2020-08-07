///
//  Generated code. Do not modify.
//  source: service_node_state.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

import 'node.pb.dart' as $13;

class UpdateNodeStateRequest extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UpdateNodeStateRequest', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..pc<$13.NodeState>(1, 'nodes', $pb.PbFieldType.PM, subBuilder: $13.NodeState.create)
    ..hasRequiredFields = false
  ;

  UpdateNodeStateRequest._() : super();
  factory UpdateNodeStateRequest() => create();
  factory UpdateNodeStateRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UpdateNodeStateRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  UpdateNodeStateRequest clone() => UpdateNodeStateRequest()..mergeFromMessage(this);
  UpdateNodeStateRequest copyWith(void Function(UpdateNodeStateRequest) updates) => super.copyWith((message) => updates(message as UpdateNodeStateRequest));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static UpdateNodeStateRequest create() => UpdateNodeStateRequest._();
  UpdateNodeStateRequest createEmptyInstance() => create();
  static $pb.PbList<UpdateNodeStateRequest> createRepeated() => $pb.PbList<UpdateNodeStateRequest>();
  @$core.pragma('dart2js:noInline')
  static UpdateNodeStateRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UpdateNodeStateRequest>(create);
  static UpdateNodeStateRequest _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$13.NodeState> get nodes => $_getList(0);
}

class UpdateNodeStateResponse extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo('UpdateNodeStateResponse', package: const $pb.PackageName('e8'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  UpdateNodeStateResponse._() : super();
  factory UpdateNodeStateResponse() => create();
  factory UpdateNodeStateResponse.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UpdateNodeStateResponse.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  UpdateNodeStateResponse clone() => UpdateNodeStateResponse()..mergeFromMessage(this);
  UpdateNodeStateResponse copyWith(void Function(UpdateNodeStateResponse) updates) => super.copyWith((message) => updates(message as UpdateNodeStateResponse));
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static UpdateNodeStateResponse create() => UpdateNodeStateResponse._();
  UpdateNodeStateResponse createEmptyInstance() => create();
  static $pb.PbList<UpdateNodeStateResponse> createRepeated() => $pb.PbList<UpdateNodeStateResponse>();
  @$core.pragma('dart2js:noInline')
  static UpdateNodeStateResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UpdateNodeStateResponse>(create);
  static UpdateNodeStateResponse _defaultInstance;
}

