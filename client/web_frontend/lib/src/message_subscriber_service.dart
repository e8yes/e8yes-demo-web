import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/message_subscriber_service_interface.dart';
import 'package:demoweb_app/src/proto_dart/service_message_subscriber.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_message_subscriber.pbgrpc.dart';

class MessageSubscriberServiceImpl extends MessageSubscriberServiceInterface {
  @override
  Future<SubscribeRealTimeMessageQueueResponse> SubscribeRealTimeMessageQueue(
      SubscribeRealTimeMessageQueueRequest request, String signature) {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return MessageSubscriberServiceClient(channel)
          .subscribeRealTimeMessageQueue(request, options: call_opts);
    });
  }
}
