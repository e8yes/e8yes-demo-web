import "dart:async";

import "package:demoweb_app/src/context.dart";
import 'package:demoweb_app/src/proto_dart/service_socialnetwork.pbgrpc.dart';
import "package:demoweb_app/src/socialnetwork_service_interface.dart";
import 'package:grpc/src/client/web_channel.dart';

class SocialNetworkServiceImpl implements SocialNetworkServiceInterface {
  @override
  Future<GetUserRelationsResponse> getUserRelations(
      GetUserRelationsRequest request) async {
    GrpcWebClientChannel channel = demowebServiceConnections.take();
    GetUserRelationsResponse res =
        await SocialNetworkServiceClient(channel).getUserRelations(request);
    demowebServiceConnections.put(channel);
    return res;
  }
}
