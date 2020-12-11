import 'dart:async';

import 'package:demoweb_app/src/basic/context.dart';
import 'package:demoweb_app/src/service/message_subscriber_service_interface.dart';
import 'package:demoweb_app/src/proto_dart/service_message_subscriber.pb.dart';

void fetchRealTimeMessages(
    int interval, MessageSubscriberServiceInterface service) {
  Timer(Duration(seconds: interval), () {
    SubscribeRealTimeMessageQueueRequest request =
        SubscribeRealTimeMessageQueueRequest();
    request.waitDurationSecs = 60;

    Stream<SubscribeRealTimeMessageQueueResponse> stream =
        service.subscribeRealTimeMessageQueue(
            request, credentialStorage.loadSignature());

    print("Begin reading from the stream");

    stream.forEach((SubscribeRealTimeMessageQueueResponse res) {
      print("Reading a message");
      print(res.message);
    }).whenComplete(() {
      print("Complete");
      fetchRealTimeMessages(1, service);
    });
  });
}
