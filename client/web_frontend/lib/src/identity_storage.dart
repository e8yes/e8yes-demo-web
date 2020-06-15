import 'dart:convert';

import 'package:fixnum/fixnum.dart';
import 'package:webstorage/webstorage.dart';

class IdentityStorage {
  final LocalStorage _storage = LocalStorage();
  final JsonEncoder _jsonEncoder = JsonEncoder();
  final JsonDecoder _jsonDecoder = JsonDecoder();

  Int64 loadUserId() {
    String userIdJson = _storage.get("userId", null);
    if (userIdJson == null) {
      return null;
    }
    return Int64.parseInt(userIdJson);
  }

  List<int> loadSecurityKey() {
    String securityKeyJson = _storage.get("securityKey", null);
    if (securityKeyJson == null) {
      return null;
    }
    List<dynamic> it = _jsonDecoder.convert(securityKeyJson);
    return List<int>.from(it);
  }

  void save(Int64 userId, List<int> securityKey) {
    _storage.set("userId", userId.toString());
    _storage.set("securityKey", _jsonEncoder.convert(securityKey));
  }

  bool hasIdentity() {
    return loadUserId() != null;
  }

  void clear() {
    _storage.remove("userId");
    _storage.remove("securityKey");
  }
}
