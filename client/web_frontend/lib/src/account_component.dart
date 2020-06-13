import 'package:angular/angular.dart';
import 'package:demoweb_app/src/authorization_service_interface.dart';
import 'package:fixnum/fixnum.dart';

import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/profile_component.dart';
import 'package:demoweb_app/src/proto_dart/nullable_primitives.pb.dart';
import "package:demoweb_app/src/proto_dart/service_user.pb.dart";
import 'package:demoweb_app/src/proto_dart/user_profile.pb.dart';
import 'package:demoweb_app/src/security_key_generator.dart';
import 'package:demoweb_app/src/sync_account_component.dart';
import 'package:demoweb_app/src/user_service_interface.dart';

enum AccountState {
  ACCOUNTLESS,
  SIGNED_IN,
}

class AccountInfo {
  AccountState accountState = AccountState.ACCOUNTLESS;
  UserPublicProfile profile = null;

  void setSignedInStateAndGrabProfile(
      Int64 userId, UserServiceInterface service) {
    GetPublicProfileRequest req = GetPublicProfileRequest();
    req.userId = (NullableInt64()..value = userId);
    service.getPublicProfile(req).then((GetPublicProfileResponse res) {
      profile = res.profile;
      accountState = AccountState.SIGNED_IN;
    });
  }
}

@Component(
  selector: "account",
  templateUrl: "account_component.html",
  styleUrls: ["navigation_component.css"],
  directives: [coreDirectives, ProfileComponent, SyncAccountComponent],
)
class AccountComponent {
  AccountInfo accountInfo = AccountInfo();

  bool displaySyncAccountComponent = false;
  bool displayAccountProfileComponent = false;

  final UserServiceInterface _user_service;
  final AuthorizationServiceInterface _auth_service;

  AccountComponent(this._user_service, this._auth_service) {
    if (identityStorage.userId != null) {
      assert(identityStorage.securityKey != null);
      credentialStorage
          .authorize(_auth_service, identityStorage.userId,
              identityStorage.securityKey)
          .then((AuthorizationResponse res) {
        accountInfo.setSignedInStateAndGrabProfile(
            identityStorage.userId, _user_service);
      });
    }
  }

  bool displayAccountlessUi() {
    return accountInfo.accountState == AccountState.ACCOUNTLESS;
  }

  bool displaySignedInUi() {
    return accountInfo.accountState == AccountState.SIGNED_IN;
  }

  void onClickSyncAccount() {
    displaySyncAccountComponent = !displaySyncAccountComponent;
  }

  void onClickAccount() {
    displayAccountProfileComponent = !displayAccountProfileComponent;
  }

  void onClickSignUp() {
    List<int> securityKey = generateSecurityKey(kSecurityKeyLength);

    _user_service
        .register(RegistrationRequest()..securityKey = securityKey)
        .then((RegistrationResponse res) {
      credentialStorage
          .authorize(_auth_service, res.userId, securityKey);
      accountInfo.setSignedInStateAndGrabProfile(res.userId, _user_service);

      identityStorage.userId = res.userId;
      identityStorage.securityKey = securityKey;
      identityStorage.save();
    });
  }
}
