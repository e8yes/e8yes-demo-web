import 'package:angular/angular.dart';
import 'package:demoweb_app/src/user_service_interface.dart';

import 'profile_component.dart';
import 'sync_account_component.dart';
import "proto_dart/service_user.pb.dart";

enum AccountState {
  ACCOUNTLESS,
  SIGNED_IN,
}

@Component(
  selector: "account",
  templateUrl: "account_component.html",
  directives: [coreDirectives, 
               ProfileComponent,
               SyncAccountComponent],
)
class AccountComponent {
  AccountState accountState =  AccountState.ACCOUNTLESS;
  String accountSummaryTag = "Davis Wen";

  bool displaySyncAccountComponent = false;
  bool displayAccountProfileComponent = false;

  final UserServiceInterface user_service_;

  AccountComponent(this.user_service_);

  bool displayAccountlessUi() {
    return accountState == AccountState.ACCOUNTLESS;
  }

  bool displaySignedInUi() {
    return accountState == AccountState.SIGNED_IN;
  }

  void onClickSyncAccount() {
    displaySyncAccountComponent = !displaySyncAccountComponent;
  }

  void onClickAccountSummaryTag() {
    displayAccountProfileComponent = !displayAccountProfileComponent;
  }

  void onClickSignUp() {
    RegistrationRequest req = RegistrationRequest();
    user_service_
      .register(req)
      .then((RegistrationResponse res) {
        accountState = AccountState.SIGNED_IN;
      });
  }
}
