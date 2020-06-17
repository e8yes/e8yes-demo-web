import "dart:async";

import "package:demoweb_app/src/context.dart";
import "package:demoweb_app/src/proto_dart/service_socialnetwork.pb.dart";
import "package:demoweb_app/src/socialnetwork_service_interface.dart";
import 'package:grpc/src/client/web_channel.dart';

class SocialNetworkServiceImpl implements SocialNetworkServiceInterface {
  @override
  Future<GetUserRelationResponse> getUserRelation(
      GetUserRelationRequest request) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    GetUserRelationResponse res =
        await SocialNetworkServiceClient(channel).getUserRelation(request);
    demowebServiceConnections.put(channel);
    return res;
  }
}
