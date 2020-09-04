import 'package:demoweb_app/src/proto_dart/service_message_subscriber.pb.dart';

abstract class MessageSubscriberServiceInterface {
  Future<SubscribeRealTimeMessageQueueResponse> SubscribeRealTimeMessageQueue(
      SubscribeRealTimeMessageQueueRequest request, String signature);
}
