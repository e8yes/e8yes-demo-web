import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:demoweb_app/src/message_channel_overview_component.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';

@Component(
  selector: "chat-room",
  templateUrl: "chat_room_component.html",
  styleUrls: ["chat_room_component.css"],
  directives: [
    coreDirectives,
    formDirectives,
    MessageChannelOverviewComponent,
  ],
)
class ChatRoomComponent {
  @Input()
  MessageChannelOverview messageChannel;
}
