import 'dart:core';

import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/footer_component.dart';
import 'package:demoweb_app/src/message_channel_component.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';
import 'package:demoweb_app/src/routes.dart';
import 'package:fixnum/fixnum.dart';

@Component(
  selector: "wmchat",
  templateUrl: "wmchat_component.html",
  styleUrls: ["wmchat_component.css"],
  directives: [
    coreDirectives,
    formDirectives,
    FooterComponent,
    MessageChannelComponent
  ],
)
class WMComponent implements OnActivate {
  Int64 targetMemberId = null;

  MessageChannelOveriew _currentMessageChannel;

  WMComponent();

  void onActivate(_, RouterState current) async {
    targetMemberId = getIdPathVariable(current.parameters);
  }

  bool searchMessageChnnalMode() {
    return _currentMessageChannel == null;
  }

  bool viewMessageChannelMode() {
    return _currentMessageChannel != null;
  }

  void onCurrentMessageChannelChange(
      MessageChannelOveriew currentMessageChannel) {
    _currentMessageChannel = currentMessageChannel;
  }
}
