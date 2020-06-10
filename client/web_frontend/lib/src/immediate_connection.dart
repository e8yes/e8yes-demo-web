import 'package:grpc/src/client/web_channel.dart';

import "connection_manager_interface.dart";

class ImmediateConnection implements ConnectionManagerInterface {
  final Uri _uri;

  ImmediateConnection(this._uri);

  @override
  GrpcWebClientChannel take() {
    return GrpcWebClientChannel.xhr(_uri);
  }

  @override
  void put(GrpcWebClientChannel channel) {
    channel.shutdown();
  }
}