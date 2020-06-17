import 'package:demoweb_app/src/connection_manager_interface.dart';
import 'package:grpc/src/client/web_channel.dart';

class ConnectionPool implements ConnectionManagerInterface {
  static const int _POOL_SIZE = 10;

  final Uri _uri;

  ConnectionPool(this._uri);

  @override
  GrpcWebClientChannel take() {
    // TODO: implement take
    throw UnimplementedError();
  }

  @override
  void put(GrpcWebClientChannel channel) {
    // TODO: implement Put
  }
}
