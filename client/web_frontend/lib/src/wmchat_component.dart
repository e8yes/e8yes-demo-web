import 'package:angular/angular.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/footer_component.dart';
import 'package:demoweb_app/src/message_channel_service_interface.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_message_channel.pbgrpc.dart';
import 'package:demoweb_app/src/routes.dart';

enum WMMode { SEARCH_MESSAGE_CHANNEL, VIEW_MESSAGE_CHANNEL }

@Component(
    selector: "wmchat",
    templateUrl: "wmchat_component.html",
    directives: [coreDirectives, FooterComponent],
    exports: [WMMode])
class WMComponent implements OnActivate {
  WMMode mode = WMMode.SEARCH_MESSAGE_CHANNEL;

  List<MessageChannel> message_channels = List<MessageChannel>();
  bool onLoadingMessageChannels = false;

  final MessageChannelServiceInterface message_channel_service_;

  WMComponent(this.message_channel_service_);

  void onActivate(_, RouterState current) async {
    final memberId = getIdPathVariable(current.parameters);

    GetJoinedInMessageChannelsRequest request =
        GetJoinedInMessageChannelsRequest();
    if (memberId != null) {
      request.withMemberIds.add(memberId);
    }

    onLoadingMessageChannels = true;
    message_channel_service_
        .getJoinedInMessageChannels(request, credentialStorage.loadSignature())
        .then((GetJoinedInMessageChannelsResponse res) {
      message_channels = res.channels;
      onLoadingMessageChannels = false;
    });
  }
}
