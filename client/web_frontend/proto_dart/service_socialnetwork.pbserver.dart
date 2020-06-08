///
//  Generated code. Do not modify.
//  source: service_socialnetwork.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'dart:async' as $async;

import 'package:protobuf/protobuf.dart' as $pb;

import 'dart:core' as $core;
import 'service_socialnetwork.pb.dart' as $9;
import 'service_socialnetwork.pbjson.dart';

export 'service_socialnetwork.pb.dart';

abstract class SocialNetworkServiceBase extends $pb.GeneratedService {
  $async.Future<$9.SendInvitationResponse> sendInvitation($pb.ServerContext ctx, $9.SendInvitationRequest request);
  $async.Future<$9.DeleteInvitationResponse> deleteInvitation($pb.ServerContext ctx, $9.DeleteInvitationRequest request);
  $async.Future<$9.AcceptInvitationResponse> acceptInvitation($pb.ServerContext ctx, $9.AcceptInvitationRequest request);
  $async.Future<$9.RejectInvitationResponse> rejectInvitation($pb.ServerContext ctx, $9.RejectInvitationRequest request);
  $async.Future<$9.GetInvitationListResponse> getInvitationList($pb.ServerContext ctx, $9.GetInvitationListRequest request);
  $async.Future<$9.DeleteFriendResponse> deleteFriend($pb.ServerContext ctx, $9.DeleteFriendRequest request);
  $async.Future<$9.GetFriendListResponse> getFriendList($pb.ServerContext ctx, $9.GetFriendListRequest request);

  $pb.GeneratedMessage createRequest($core.String method) {
    switch (method) {
      case 'SendInvitation': return $9.SendInvitationRequest();
      case 'DeleteInvitation': return $9.DeleteInvitationRequest();
      case 'AcceptInvitation': return $9.AcceptInvitationRequest();
      case 'RejectInvitation': return $9.RejectInvitationRequest();
      case 'GetInvitationList': return $9.GetInvitationListRequest();
      case 'DeleteFriend': return $9.DeleteFriendRequest();
      case 'GetFriendList': return $9.GetFriendListRequest();
      default: throw $core.ArgumentError('Unknown method: $method');
    }
  }

  $async.Future<$pb.GeneratedMessage> handleCall($pb.ServerContext ctx, $core.String method, $pb.GeneratedMessage request) {
    switch (method) {
      case 'SendInvitation': return this.sendInvitation(ctx, request);
      case 'DeleteInvitation': return this.deleteInvitation(ctx, request);
      case 'AcceptInvitation': return this.acceptInvitation(ctx, request);
      case 'RejectInvitation': return this.rejectInvitation(ctx, request);
      case 'GetInvitationList': return this.getInvitationList(ctx, request);
      case 'DeleteFriend': return this.deleteFriend(ctx, request);
      case 'GetFriendList': return this.getFriendList(ctx, request);
      default: throw $core.ArgumentError('Unknown method: $method');
    }
  }

  $core.Map<$core.String, $core.dynamic> get $json => SocialNetworkServiceBase$json;
  $core.Map<$core.String, $core.Map<$core.String, $core.dynamic>> get $messageJson => SocialNetworkServiceBase$messageJson;
}

