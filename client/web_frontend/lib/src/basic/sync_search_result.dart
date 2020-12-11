import "dart:async";

class SearchResultSync<ResponseType> {
  Future<ResponseType> _lastest;

  Future<ResponseType> takeLatestFuture(
      Future<ResponseType> current, Function(ResponseType) handler) {
    _lastest = current;
    return current.then((ResponseType res) {
      if (current != _lastest) {
        return null;
      }
      handler(res);
      return res;
    });
  }
}
