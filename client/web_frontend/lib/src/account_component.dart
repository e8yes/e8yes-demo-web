import 'package:angular/angular.dart';

import 'profile_component.dart';
import 'sync_account_component.dart';

enum AccountState {
  ACCOUNTLESS,
  SIGNED_IN,
}

@Component(
  selector: "account",
  templateUrl: "account_component.html",
  directives: [coreDirectives, 
               ProfileComponent,
               SyncAccountComponent]
)
class AccountComponent {
  AccountState accountState =  AccountState.ACCOUNTLESS;
  String accountSummaryTag = "Davis Wen";

  bool displaySyncAccountComponent = false;
  bool displayAccountProfileComponent = false;

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
    accountState = AccountState.SIGNED_IN;
  }
}
