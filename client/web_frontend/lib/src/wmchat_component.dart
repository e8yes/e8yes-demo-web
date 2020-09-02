import 'package:angular/angular.dart';
import 'package:demoweb_app/src/footer_component.dart';

@Component(
  selector: "wmchat",
  templateUrl: "wmchat_component.html",
  directives: [coreDirectives, FooterComponent],
)
class WMComponent {}
