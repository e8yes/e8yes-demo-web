import 'package:angular/angular.dart';
import 'package:demoweb_app/src/proto_dart/nullable_primitives.pb.dart';
import 'package:demoweb_app/src/proto_dart/user_profile.pb.dart';
import 'package:demoweb_app/src/user_service_interface.dart';

import 'profile_component.dart';
import 'sync_account_component.dart';
import "proto_dart/service_user.pb.dart";

enum AccountState {
  ACCOUNTLESS,
  SIGNED_IN,
}

class AccountInfo {
  AccountState accountState =  AccountState.ACCOUNTLESS;
  String accountSummaryTag = "";
  UserPublicProfile profile = null;

  void _updateAccountSummaryTag(UserPublicProfile profile) {
    if (profile.hasAlias()) {
      accountSummaryTag = profile.alias;
    } else {
      accountSummaryTag = profile.userId.toString();
    }
  }

  void setSignedInStateAndGrabProfile(var userId, 
                                      UserServiceInterface service) {
    GetPublicProfileRequest req = GetPublicProfileRequest();
    req.userId = (NullableInt64()..value = userId);
    service
      .getPublicProfile(req)
      .then((GetPublicProfileResponse res) {
        _updateAccountSummaryTag(res.profile);
        profile = res.profile;
        accountState = AccountState.SIGNED_IN;
      });
  }
}

@Component(
  selector: "account",
  templateUrl: "account_component.html",
  directives: [coreDirectives, 
               ProfileComponent,
               SyncAccountComponent],
)
class AccountComponent {
  AccountInfo accountInfo = AccountInfo();

  bool displaySyncAccountComponent = false;
  bool displayAccountProfileComponent = false;

  final UserServiceInterface _user_service;

  AccountComponent(this._user_service);

  bool displayAccountlessUi() {
    return accountInfo.accountState == AccountState.ACCOUNTLESS;
  }

  bool displaySignedInUi() {
    return accountInfo.accountState == AccountState.SIGNED_IN;
  }

  void onClickSyncAccount() {
    displaySyncAccountComponent = !displaySyncAccountComponent;
  }

  void onClickAccountSummaryTag() {
    displayAccountProfileComponent = !displayAccountProfileComponent;
  }

  void onClickSignUp() {
    RegistrationRequest req = RegistrationRequest();
    req.securityKey = [1, 2, 3];
    _user_service
      .register(req)
      .then((RegistrationResponse res) {
        accountInfo.setSignedInStateAndGrabProfile(res.userId, _user_service);
      });
  }
}
