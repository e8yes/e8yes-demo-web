import 'package:angular/angular.dart';

import 'package:demoweb_app/src/route_paths.dart';
import 'package:demoweb_app/src/routes.dart';

@Component(
  selector: "demo-list",
  templateUrl: "demo_list_component.html",
  directives: [coreDirectives],
  exports: [RoutePaths, Routes],
)
class DemoListComponent {
}