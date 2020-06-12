import 'package:angular/angular.dart';
import 'package:demoweb_app/src/account_component.dart';

@Component(
  selector: "navigation-sidebar",
  templateUrl: "navigation_component.html",
  styleUrls: ["navigation_component.css"],
  directives: [coreDirectives, AccountComponent],
)
class NavigationComponent {
}