import 'package:angular/angular.dart';
import 'package:demoweb_app/src/footer_component.dart';

enum WMMode { SEARCH_MESSAGE_CHANNEL, VIEW_MESSAGE_CHANNEL }

@Component(
    selector: "wmchat",
    templateUrl: "wmchat_component.html",
    directives: [coreDirectives, FooterComponent],
    exports: [WMMode])
class WMComponent {
  WMMode mode = WMMode.SEARCH_MESSAGE_CHANNEL;
}
