import 'package:angular/angular.dart';

enum AccountStatus {
  ACCOUNTLESS,
  SIGNED_IN,
}

class AccountState {
  AccountStatus status = AccountStatus.ACCOUNTLESS;
}

@Component(
  selector: 'account',
  templateUrl: 'account_component.html',
  directives: [coreDirectives]
)
class AccountComponent {
  AccountState accountState = AccountState();
  String userAlias = 'Davis Wen';

  bool displayAccountlessUi() {
    return accountState.status == AccountStatus.ACCOUNTLESS;
  }

  bool displayLoggedInUi() {
    return accountState.status == AccountStatus.SIGNED_IN;
  }

  void onClickSignIn() {
    accountState.status = AccountStatus.SIGNED_IN;
  }
}
