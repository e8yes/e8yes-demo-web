import "package:random_string/random_string.dart";

List<int> generateSecurityKey(int keyLen) {
  String securityKeyStr = randomString(keyLen);
  List<int> result = List<int>();
  for (int i = 0; i < securityKeyStr.length; i ++) {
    result.add(securityKeyStr.codeUnitAt(i));
  }
  return result;
}