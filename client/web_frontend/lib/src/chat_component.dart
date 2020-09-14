import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:demoweb_app/src/message_channel_overview_component.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';

@Component(
  selector: "chat",
  templateUrl: "chat_component.html",
  styleUrls: ["chat_component.css"],
  directives: [
    coreDirectives,
    formDirectives,
    MessageChannelOverviewComponent,
  ],
)
class ChatComponent {
  @Input()
  MessageChannelOverview messageChannel;
}
