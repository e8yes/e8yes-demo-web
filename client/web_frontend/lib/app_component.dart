import 'package:angular/angular.dart';
import 'package:demoweb_app/src/authorization_service.dart';
import 'package:demoweb_app/src/authorization_service_interface.dart';
import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/navigation_component.dart';
import 'package:demoweb_app/src/user_service.dart';
import 'package:demoweb_app/src/user_service_interface.dart';

@Component(
  selector: 'app',
  templateUrl: 'app_component.html',
  directives: [NavigationComponent],
  providers: [
    ClassProvider(UserServiceInterface, useClass: UserServiceImpl),
    ClassProvider(AuthorizationServiceInterface, useClass: AuthorizationServiceImpl),
  ],
)
class AppComponent {
  AppComponent() {
    identityStorage.load();
  }
}
