import "package:grpc/grpc_web.dart";

abstract class ConnectionManagerInterface {
  GrpcWebClientChannel take();
  void put(GrpcWebClientChannel channel);
}