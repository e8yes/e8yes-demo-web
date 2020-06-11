import 'package:angular/angular.dart';
import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/user_service.dart';
import 'package:demoweb_app/src/user_service_interface.dart';

import 'src/account_component.dart';

@Component(
  selector: 'app',
  templateUrl: 'app_component.html',
  directives: [AccountComponent],
  providers: [ClassProvider(UserServiceInterface, useClass: UserServiceImpl)],
)
class AppComponent {
  AppComponent() {
    identityStorage.load();
  }
}
