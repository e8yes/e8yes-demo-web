import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';
import 'package:fixnum/fixnum.dart';

@Component(
  selector: "message-channel-overview",
  templateUrl: "message_channel_overview_component.html",
  styleUrls: ["message_channel_overview_component.css"],
  directives: [
    coreDirectives,
    formDirectives,
  ],
)
class MessageChannelOverviewComponent {
  @Input()
  MessageChannelOveriew messageChannel;

  String dateToString(Int64 timestamp) {
    return DateTime.fromMillisecondsSinceEpoch(timestamp.toInt() * 1000)
        .toLocal()
        .toString();
  }
}
