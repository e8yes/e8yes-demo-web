import 'dart:convert';

import 'package:webstorage/webstorage.dart';

class CredentialStorage {
  final SessionStorage _sessionStorage = SessionStorage();
  final JsonEncoder _jsonEncoder = JsonEncoder();
  final JsonDecoder _jsonDecoder = JsonDecoder();

  void saveSignature(List<int> signature) {
    _sessionStorage.set("sig", _jsonEncoder.convert(signature));
  }

  List<int> loadSignature() {
    String sigJson = _sessionStorage.get("sig");
    if (sigJson != null) {
      List<dynamic> it = _jsonDecoder.convert(sigJson);
      List<int> signature = List<int>.from(it);
      return signature;
    } else {
      return null;
    }
  }
}
