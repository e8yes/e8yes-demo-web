import 'dart:async';

import 'package:demoweb_app/src/service/authorization_service_interface.dart';
import 'package:fixnum/fixnum.dart';

import 'package:demoweb_app/src/basic/context.dart';
import "package:demoweb_app/src/proto_dart/service_user.pb.dart";
import 'package:demoweb_app/src/proto_dart/user_profile.pb.dart';
import 'package:demoweb_app/src/basic/security_key_generator.dart';
import 'package:demoweb_app/src/service/user_service_interface.dart';
import 'package:grpc/grpc_web.dart';

enum AccountState {
  ACCOUNTLESS,
  SIGNED_IN,
}

class AccountInfo {
  AccountState accountState = AccountState.ACCOUNTLESS;
  UserPublicProfile profile = null;

  void setSignedInStateAndGrabProfile(
      Int64 userId, String signature, UserServiceInterface service) {
    GetPublicProfileRequest req = GetPublicProfileRequest();
    req.userId = userId;
    service
        .getPublicProfile(req, signature)
        .then((GetPublicProfileResponse res) {
      profile = res.profile;
      accountState = AccountState.SIGNED_IN;
    });
  }
}

void signIn(AccountInfo accountInfo, UserServiceInterface user_service,
    AuthorizationServiceInterface auth_service) {
  Int64 userId = identityStorage.loadUserId();
  List<int> securityKey = identityStorage.loadSecurityKey();

  if (userId != null) {
    assert(securityKey != null);
    credentialStorage
        .authorize(auth_service, userId, securityKey)
        .then((AuthorizationResponse res) {
      if (accountInfo == null) {
        return res;
      }
      accountInfo.setSignedInStateAndGrabProfile(
          userId, res.signedIdentity.signature, user_service);
      return res;
    }).catchError((err) {
      if (err.code == GrpcError.notFound().code ||
          err.code == GrpcError.unauthenticated().code) {
        identityStorage.clear();
      }
      return Future.value(null);
    });
  }
}

void signUp(AccountInfo accountInfo, UserServiceInterface user_service,
    AuthorizationServiceInterface auth_service) {
  List<int> securityKey = generateSecurityKey(kSecurityKeyLength);

  user_service
      .register(RegistrationRequest()..securityKey = securityKey)
      .then((RegistrationResponse regRes) {
    identityStorage.save(regRes.userId, securityKey);

    credentialStorage
        .authorize(auth_service, regRes.userId, securityKey)
        .then((AuthorizationResponse authRes) {
      if (accountInfo != null) {
        accountInfo.setSignedInStateAndGrabProfile(
            regRes.userId, authRes.signedIdentity.signature, user_service);
      }
    });
  });
}
