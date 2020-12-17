import 'dart:async';

import 'package:angular/angular.dart';
import 'package:angular/core.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:demoweb_app/src/basic/context.dart';
import 'package:demoweb_app/src/component/footer/footer_component.dart';
import 'package:demoweb_app/src/component/message_channel/message_channel_editor_component.dart';
import 'package:demoweb_app/src/component/message_channel/message_channel_overview_component.dart';
import 'package:demoweb_app/src/service/message_channel_service_interface.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_message_channel.pb.dart';
import 'package:fixnum/fixnum.dart';

class MessageChannelStructuralControl {
  List<MessageChannelOverview> messageChannelOverviews =
      List<MessageChannelOverview>();
  bool onLoadingMessageChannels = false;
  bool showAddMessageChannelPopup = false;
}

@Component(
    selector: "message-channel-manager",
    templateUrl: "message_channel_manager_component.html",
    styleUrls: [
      "message_channel_manager_component.css"
    ],
    directives: [
      coreDirectives,
      formDirectives,
      FooterComponent,
      MessageChannelOverviewComponent,
      MessageChannelEditorComponent
    ],
    exports: [
      MessageChannelStructuralControl,
    ])
class MessageChannelManagerComponent implements OnInit {
  MessageChannelStructuralControl structuralControl =
      MessageChannelStructuralControl();

  @Input()
  Int64 targetMemberId;

  StreamController<MessageChannelOverview>
      currentMessageChannelStreamController =
      StreamController<MessageChannelOverview>();
  @Output()
  Stream<MessageChannelOverview> get currentMessageChannel =>
      currentMessageChannelStreamController.stream;

  final MessageChannelServiceInterface _messageChannelService;

  static const int _kActiveUsersFetchLimit = 5;

  MessageChannelManagerComponent(this._messageChannelService) {}

  @override
  void ngOnInit() {
    this._fetchMessageChannelList();
  }

  void onKeyDownSearchMessageChannel(String searchTerms) {}

  void onClickCreateMessageChannel() {
    structuralControl.showAddMessageChannelPopup = true;
  }

  void onCompleteCreatingMessageChannel(bool successful) {
    structuralControl.showAddMessageChannelPopup = false;
    if (successful) {
      this._fetchMessageChannelList();
    }
  }

  void onClickMessageChannel(MessageChannelOverview channel) {
    currentMessageChannelStreamController.add(channel);
  }

  void _fetchMessageChannelList() {
    SearchMessageChannelsRequest request = SearchMessageChannelsRequest();
    if (targetMemberId != null) {
      request.withMemberIds.add(targetMemberId);
    }
    request.activeMemberFetchLimit = _kActiveUsersFetchLimit;

    structuralControl.onLoadingMessageChannels = true;
    _messageChannelService
        .searchMessageChannels(request, credentialStorage.loadSignature())
        .then((SearchMessageChannelsResponse res) {
      structuralControl.messageChannelOverviews = res.channels;
      structuralControl.onLoadingMessageChannels = false;
    });
  }
}
