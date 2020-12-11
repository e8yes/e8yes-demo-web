import 'dart:core';

import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/chat_component.dart';
import 'package:demoweb_app/src/footer_component.dart';
import 'package:demoweb_app/src/message_channel_manager_component.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';
import 'package:demoweb_app/src/routers/routes.dart';
import 'package:fixnum/fixnum.dart';

@Component(
  selector: "wmchat",
  templateUrl: "wmchat_component.html",
  styleUrls: ["wmchat_component.css"],
  directives: [
    coreDirectives,
    formDirectives,
    FooterComponent,
    ChatComponent,
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
