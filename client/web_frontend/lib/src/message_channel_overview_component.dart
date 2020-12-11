import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:demoweb_app/src/date_component.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';

@Component(
  selector: "message-channel-overview",
  templateUrl: "message_channel_overview_component.html",
  styleUrls: ["message_channel_overview_component.css"],
  directives: [coreDirectives, formDirectives, DateComponent],
)
class MessageChannelOverviewComponent {
  @Input()
  MessageChannelOverview messageChannel;

  @Input()
  bool showDates = true;
}
