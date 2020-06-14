import 'package:angular/angular.dart';
import 'package:demoweb_app/src/account.dart';

import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/profile_component.dart';
import 'package:demoweb_app/src/user_service_interface.dart';

@Component(
  selector: "account",
  templateUrl: "account_component.html",
  directives: [coreDirectives, ProfileComponent],
)
class AccountComponent {
  AccountInfo accountInfo = AccountInfo();
  final UserServiceInterface _user_service;

  AccountComponent(this._user_service) {
    accountInfo.setSignedInStateAndGrabProfile(
        identityStorage.loadUserId(), _user_service);
  }
}
