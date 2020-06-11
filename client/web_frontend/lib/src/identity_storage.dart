import 'dart:convert';

import 'package:fixnum/fixnum.dart';
import 'package:webstorage/webstorage.dart';

class IdentityStorage {
  Int64 userId = null;
  List<int> securityKey = null;
  
  final LocalStorage _storage = LocalStorage();
  final JsonEncoder _jsonEncoder = JsonEncoder();
  final JsonDecoder _jsonDecoder = JsonDecoder();

  void load() {
    String userIdJson = _storage.get("userId", null);
    if (userIdJson != null) {
      userId = Int64.parseInt(userIdJson);
    }

    String securityKeyJson = _storage.get("securityKey", null);
    if (securityKeyJson != null) {
      List<dynamic> it = _jsonDecoder.convert(securityKeyJson);
      securityKey = List<int>.from(it);
    }
  }

  void save() {
    _storage.set("userId", userId.toString());
    _storage.set("securityKey", _jsonEncoder.convert(securityKey));
  }
}