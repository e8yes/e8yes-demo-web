import 'dart:async';
import 'package:demoweb_app/src/service/authorization_service_interface.dart';
import 'package:demoweb_app/src/proto_dart/service_user.pb.dart';
import 'package:fixnum/fixnum.dart';
import 'package:webstorage/webstorage.dart';

class CredentialStorage {
  final SessionStorage _sessionStorage = SessionStorage();

  Future<AuthorizationResponse> authorize(
      AuthorizationServiceInterface auth_service,
      Int64 userId,
      List<int> securityKey) async {
    return auth_service
        .authorize(AuthorizationRequest()
          ..userId = userId
          ..securityKey = securityKey)
        .then((AuthorizationResponse res) {
      _sessionStorage.set("sig", res.signedIdentity.signature);
      return res;
    });
  }

  String loadSignature() {
    return _sessionStorage.get("sig");
  }
}
