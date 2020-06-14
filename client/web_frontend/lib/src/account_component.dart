import 'package:angular/angular.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/account.dart';
import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/profile_component.dart';
import 'package:demoweb_app/src/routes.dart';
import 'package:demoweb_app/src/user_service_interface.dart';
import 'package:fixnum/fixnum.dart';

@Component(
  selector: "account",
  templateUrl: "account_component.html",
  directives: [coreDirectives, ProfileComponent],
)
class AccountComponent implements OnActivate {
  AccountInfo accountInfo = AccountInfo();
  final UserServiceInterface _user_service;
  Int64 currentUserId;

  AccountComponent(this._user_service);

  @override
  void onActivate(_, RouterState current) async {
    final userId = getIdPathVariable(current.parameters);
    if (userId != null) {
      currentUserId = userId;
    } else {
      currentUserId = identityStorage.loadUserId();
    }
    accountInfo.setSignedInStateAndGrabProfile(currentUserId, _user_service);
  }

  bool owner() {
    return currentUserId == identityStorage.loadUserId();
  }

  void onClickAddContact() {
  }
}
