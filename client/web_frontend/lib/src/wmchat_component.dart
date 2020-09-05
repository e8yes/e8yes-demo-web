import 'package:angular/angular.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/footer_component.dart';
import 'package:demoweb_app/src/message_channel_service_interface.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_message_channel.pbgrpc.dart';
import 'package:demoweb_app/src/routes.dart';
import 'package:fixnum/fixnum.dart';

enum WMMode { SEARCH_MESSAGE_CHANNEL, VIEW_MESSAGE_CHANNEL }

@Component(
    selector: "wmchat",
    templateUrl: "wmchat_component.html",
    styleUrls: ["wmchat_component.css"],
    directives: [coreDirectives, FooterComponent],
    exports: [WMMode])
class WMComponent implements OnActivate {
  WMMode mode = WMMode.SEARCH_MESSAGE_CHANNEL;

  List<MessageChannel> messageChannels = List<MessageChannel>();
  Map<Int64, MessageChannelAuxiliaries> messageChannelAuxiliaries =
      Map<Int64, MessageChannelAuxiliaries>();
  bool onLoadingMessageChannels = false;

  MessageChannel currentMessageChannel = null;

  static const int _kActiveUsersFetchLimit = 5;

  Int64 targetMemberId = null;

  final MessageChannelServiceInterface messageChannelService_;

  WMComponent(this.messageChannelService_);

  void onActivate(_, RouterState current) async {
    targetMemberId = getIdPathVariable(current.parameters);
    this._fetchMessageChannelList();
  }

  void onKeyDownSearchMessageChannel(String searchTerms) {}

  void onClickMessageChannel(MessageChannel channel) {
    currentMessageChannel = channel;
    mode = WMMode.VIEW_MESSAGE_CHANNEL;
  }

  String dateToString(Int64 timestamp) {
    return DateTime.fromMillisecondsSinceEpoch(timestamp.toInt() * 1000)
        .toLocal()
        .toString();
  }

  MessageChannelAuxiliaries channelAuxiliaryInfo(Int64 channelId) {
    return messageChannelAuxiliaries[channelId];
  }

  void _fetchMessageChannelList() {
    GetJoinedInMessageChannelsRequest request =
        GetJoinedInMessageChannelsRequest();
    if (targetMemberId != null) {
      request.withMemberIds.add(targetMemberId);
    }
    request.activeMemberFetchLimit = _kActiveUsersFetchLimit;

    onLoadingMessageChannels = true;
    messageChannelService_
        .getJoinedInMessageChannels(request, credentialStorage.loadSignature())
        .then((GetJoinedInMessageChannelsResponse res) {
      messageChannels = res.channels;
      messageChannelAuxiliaries = res.auxiliaries;
      onLoadingMessageChannels = false;
    });
  }
}
