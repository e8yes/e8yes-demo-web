import 'package:angular/angular.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/footer_component.dart';
import 'package:demoweb_app/src/message_channel_service_interface.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_message_channel.pbgrpc.dart';
import 'package:demoweb_app/src/routes.dart';
import 'package:fixnum/fixnum.dart';

enum WMMode {
  SEARCH_MESSAGE_CHANNEL,
  VIEW_MESSAGE_CHANNEL,
}

class StructuralControl {
  WMMode mode = WMMode.SEARCH_MESSAGE_CHANNEL;
  List<MessageChannelOveriew> messageChannelOverviews =
      List<MessageChannelOveriew>();
  bool onLoadingMessageChannels = false;
  bool showAddMessageChannelPopup = false;
}

@Component(
    selector: "wmchat",
    templateUrl: "wmchat_component.html",
    styleUrls: ["wmchat_component.css"],
    directives: [coreDirectives, FooterComponent],
    exports: [WMMode, StructuralControl])
class WMComponent implements OnActivate {
  StructuralControl structuralControl = StructuralControl();

  MessageChannelOveriew _currentMessageChannel = null;
  Int64 _targetMemberId = null;

  final MessageChannelServiceInterface _messageChannelService;

  static const int _kActiveUsersFetchLimit = 5;

  WMComponent(this._messageChannelService);

  void onActivate(_, RouterState current) async {
    _targetMemberId = getIdPathVariable(current.parameters);
    this._fetchMessageChannelList();
  }

  void onKeyDownSearchMessageChannel(String searchTerms) {}

  void onClickCreateMessageChannel() {
    structuralControl.showAddMessageChannelPopup = true;
  }

  void onClickMessageChannel(MessageChannelOveriew channel) {
    _currentMessageChannel = channel;
    structuralControl.mode = WMMode.VIEW_MESSAGE_CHANNEL;
  }

  String dateToString(Int64 timestamp) {
    return DateTime.fromMillisecondsSinceEpoch(timestamp.toInt() * 1000)
        .toLocal()
        .toString();
  }

  void _fetchMessageChannelList() {
    GetJoinedInMessageChannelsRequest request =
        GetJoinedInMessageChannelsRequest();
    if (_targetMemberId != null) {
      request.withMemberIds.add(_targetMemberId);
    }
    request.activeMemberFetchLimit = _kActiveUsersFetchLimit;

    structuralControl.onLoadingMessageChannels = true;
    _messageChannelService
        .getJoinedInMessageChannels(request, credentialStorage.loadSignature())
        .then((GetJoinedInMessageChannelsResponse res) {
      structuralControl.messageChannelOverviews = res.channels;
      structuralControl.onLoadingMessageChannels = false;
    });
  }
}
