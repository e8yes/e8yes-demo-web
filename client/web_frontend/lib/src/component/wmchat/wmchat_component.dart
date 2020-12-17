import 'dart:core';

import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/component/chat/chat_room_component.dart';
import 'package:demoweb_app/src/component/footer/footer_component.dart';
import 'package:demoweb_app/src/component/message_channel/message_channel_manager_component.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';
import 'package:demoweb_app/src/routes/routes.dart';
import 'package:fixnum/fixnum.dart';

@Component(
  selector: "wmchat",
  templateUrl: "wmchat_component.html",
  styleUrls: ["wmchat_component.css"],
  directives: [
    coreDirectives,
    formDirectives,
    FooterComponent,
    ChatRoomComponent,
    MessageChannelManagerComponent
  ],
)
class WMComponent implements OnActivate {
  Int64 targetMemberId = null;

  MessageChannelOverview currentMessageChannel;

  WMComponent();

  void onActivate(_, RouterState current) async {
    targetMemberId = getIdPathVariable(current.parameters);
  }

  bool searchMessageChannelMode() {
    return currentMessageChannel == null;
  }

  bool viewMessageChannelMode() {
    return currentMessageChannel != null;
  }

  void onCurrentMessageChannelChange(
      MessageChannelOverview currentMessageChannel) {
    this.currentMessageChannel = currentMessageChannel;
  }
}
